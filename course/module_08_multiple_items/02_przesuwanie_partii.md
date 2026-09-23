# 8.2 Przesuwanie partii

To najważniejsza misja tego modułu pod względem teorii. Przeczytaj ją całą, zanim zaczniesz pisać kod.

## Problem

Skoro `Plant` może teraz mieścić kilka paczek naraz, `advance()` musi przesunąć wszystkie we
właściwym porządku — w jednym wywołaniu, bez przesuwania którejkolwiek paczki więcej niż raz, ale też
bez sztucznego opóźnienia, gdy zwolniona strefa mogłaby od razu przyjąć kolejną paczkę z tyłu.

## Nowe elementy C++

```cpp
struct ItemDeparture {
    ItemId id;
    Zone destination;
};

struct AdvanceResult {
    std::optional<SystemEventKind> event;
    std::optional<ItemDeparture> departure;
};

AdvanceResult advance(Plant& plant, const Diverter& diverter, bool routingReady = true);
```

`event` dotyczy wyłącznie paczki w `diverting`, jeśli nie odjechała w tym ticku. `departure` jest
ustawiane tylko przy udanym rutowaniu — z konstrukcji obu tych pól nigdy nie ustawia się jednocześnie.

## Reguły przejść, w wymaganej kolejności

`advance()` musi wykonać cztery przejścia, **każde dokładnie raz na wywołanie**, w tej ustalonej
kolejności downstream-to-upstream:

**1. Rozstrzygnięcie w `diverting`** (tylko gdy slot jest zajęty). To dokładnie reguła terminu
rutowania z Misji 26 — z tą różnicą, że licznik `divertingWaitTicks` żyje teraz na samej paczce
(`Item::divertingWaitTicks`), nie gdzieś obok w `Engine`:
- gdy `routingReady` jest `false`, licznik paczki wraca do zera i na tym przejście się kończy w tym
  ticku (paczka zostaje w `diverting`, bez zdarzenia);
- gdy `routingReady` jest `true`, ale dywerter jeszcze nie ustawił się na zadanej pozycji
  (`!diverter.isSettled()`), licznik paczki rośnie o jeden; zdarzenie w wyniku to
  `SystemEventKind::DiverterNotReady` przy pierwszym takim ticku licznika, a
  `SystemEventKind::RoutingDeadlineMissed` przy każdym kolejnym;
- gdy dywerter jest ustawiony, paczka odjeżdża: `result.departure` dostaje jej id i miejsce docelowe
  (`Zone::OutputLight` dla pozycji `Straight`, `Zone::OutputHeavy` w przeciwnym razie), a slot
  `diverting` zostaje zwolniony.

**2–4. Trzy przejścia między sąsiednimi strefami** — `weighing`→`diverting`,
`presenceCheck`→`weighing`, `infeed`→`presenceCheck`, w tej właśnie kolejności. Każde z nich stosuje
ten sam wzorzec: jeśli strefa docelowa jest teraz pusta (co mogło być efektem przejścia wykonanego
przed chwilą, w tym samym wywołaniu) i strefa źródłowa jest zajęta, paczka przenosi się do strefy
docelowej, a strefa źródłowa zostaje zwolniona. Jedyny wyjątek: paczka wchodząca do `diverting`
zaczyna z wyzerowanym `divertingWaitTicks` — to świeże wejście do strefy, nie kontynuacja czyjegoś
odliczania.

## Dlaczego to gwarantuje "co najwyżej jeden ruch na paczkę na tick"

Nie dlatego, że każdy *slot* jest dotknięty tylko raz — slot jest odczytywany jako źródło przez jedno
przejście i sprawdzany jako cel przez sąsiednie, więc jest odwoływany więcej niż raz. Gwarancja bierze
się z tego, że każde z czterech *przejść* (rozstrzygnięcie w `Diverting`, `Weighing`→`Diverting`,
`PresenceCheck`→`Weighing`, `Infeed`→`PresenceCheck`) jest wykonywane **dokładnie raz na wywołanie, w
tej ustalonej kolejności downstream-to-upstream**. Paczka, która trafia do strefy przez jedno
przejście, nie może zostać podjęta przez *wcześniejsze* przejście w tym samym wywołaniu — bo to
wcześniejsze przejście już się wykonało. Jednocześnie każde przejście sprawdza zajętość swojego celu w
chwili, gdy samo się wykonuje — co może już odzwierciedlać efekt wcześniejszego przejścia z tego
samego ticku — więc zwolnione miejsce jest natychmiast widoczne dla przejścia za nim, pozwalając kilku
paczkom przesunąć się razem bez sztucznej przerwy.

## Skąd bierze się polecenie dywertera

Polecenie dywertera decydowane w danym `Engine::step()` zawsze pochodzi z klasyfikacji paczki, która
**już jest** w `diverting` — nigdy z paczki właśnie sklasyfikowanej w `weighing` w tym samym ticku, bo
ta paczka nie może dotrzeć do `diverting` przed wywołaniem `advance()`, a polecenie jest decydowane
przed tym wywołaniem. Zobacz przykładowy przebieg niżej — dobrze to pokazuje.

## Przykładowy przebieg: trzy paczki, Light/Heavy/Light

```text
tick 0: infeed=2 presenceCheck=1 weighing=- diverting=-  cmd=Hold(for -)      event=-                 departure=-
tick 1: infeed=3 presenceCheck=2 weighing=1 diverting=-  cmd=Hold(for -)      event=-                 departure=-
tick 2: infeed=- presenceCheck=3 weighing=2 diverting=1  cmd=Hold(for -)      event=-                 departure=-
tick 3: infeed=- presenceCheck=- weighing=3 diverting=2  cmd=Hold(for 1)      event=-                 departure=1->Light
tick 4: infeed=- presenceCheck=- weighing=3 diverting=2  cmd=Divert(for 2)    event=DiverterNotReady  departure=-
tick 5: infeed=- presenceCheck=- weighing=- diverting=3  cmd=Divert(for 2)    event=-                 departure=2->Heavy
tick 6: infeed=- presenceCheck=- weighing=- diverting=3  cmd=Hold(for 3)      event=DiverterNotReady  departure=-
tick 7: infeed=- presenceCheck=- weighing=- diverting=-  cmd=Hold(for 3)      event=-                 departure=3->Light
```

(Ten ślad pochodzi z gołego `Plant`/`advance()`, wołanego bezpośrednio, bez `Engine` — bez rozruchu
pasa. Przez prawdziwy `Engine` numery ticków będą inne, bo pas potrzebuje własnego czasu na rozpęd; sam
wzorzec zdarzeń pozostaje ten sam.)

Zwróć uwagę na tick 3: paczka 1 odjeżdża, a paczka 2 w tym samym wywołaniu `advance()` wchodzi do
właśnie zwolnionego `diverting` — a mimo to polecenie decydowane w tym ticku wciąż dotyczyło paczki 1
(zdecydowane przed `advance()`). To bezpośrednio widoczna ilustracja reguły z sekcji wyżej.

## Co już masz gotowe

[`include/psm/plant.hpp`](../../include/psm/plant.hpp) — `ItemDeparture`, `AdvanceResult`, sygnatura
`advance()` — wszystko gotowe. `spawnItem` z Misji 29 jest już Twoje.

## Co masz napisać

Zaimplementuj ciało `advance()` w [`src/plant.cpp`](../../src/plant.cpp) zgodnie z regułami przejść
opisanymi wyżej: cztery przejścia, w podanej kolejności, każde wykonane dokładnie raz na wywołanie.

## Sprawdź się

```bash
ctest --preset test -L misja-30
```

Oczekiwany wynik: `100% tests passed`. Testy sprawdzają: przejście przez wszystkie cztery strefy z
poleceniem dywertera zadanym późno (`plant_diverter_test`); pełny termin rutowania na własnym
`divertingWaitTicks` paczki, w tym reset przy `!routingReady` (`plant_deadline_test`).

## Częste błędy

- **Przetwarzanie stref w kolejności upstream-to-downstream** — wtedy paczka mogłaby przesunąć się
  dwukrotnie w jednym wywołaniu (raz jako "wejście do kolejnej strefy", raz jako "wyjście z niej chwilę
  później w tym samym przebiegu).
- **Zapominanie o zerowaniu `divertingWaitTicks` przy wejściu do `diverting`** — kolejna paczka
  odziedziczyłaby cudzy, częściowo zużyty licznik.
- **Wnioskowanie polecenia dywertera z paczki w `weighing`** — złam to świadomie w prototypie, żeby
  zobaczyć, dlaczego test tego nie akceptuje.

## Pytanie do zastanowienia

Gdyby `advance()` przetwarzał strefy w odwrotnej kolejności (upstream-to-downstream), które dokładnie
zachowanie z przebiegu powyżej by się zepsuło, i na czym konkretnie by to polegało?

**Dalej:** [Misja 31: korelacja per paczka](./03_korelacja_per_paczka.md).
