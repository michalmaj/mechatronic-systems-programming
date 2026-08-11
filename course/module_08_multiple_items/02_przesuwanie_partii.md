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

## Dokładny algorytm

```cpp
AdvanceResult advance(Plant& plant, const Diverter& diverter, bool routingReady) {
    AdvanceResult result;

    // Diverting: rutuj / czekaj / przekrocz termin -- dokładnie reguła Misji 26, teraz na
    // własnym divertingWaitTicks tej konkretnej paczki.
    if (plant.diverting.has_value()) {
        if (!routingReady) {
            plant.diverting->divertingWaitTicks = 0;
        } else if (!diverter.isSettled()) {
            ++plant.diverting->divertingWaitTicks;
            result.event = plant.diverting->divertingWaitTicks <= 1
                                ? SystemEventKind::DiverterNotReady
                                : SystemEventKind::RoutingDeadlineMissed;
        } else {
            const Zone destination = diverter.actualPosition() == DiverterPosition::Straight
                                          ? Zone::OutputLight
                                          : Zone::OutputHeavy;
            result.departure = ItemDeparture{plant.diverting->id, destination};
            plant.diverting.reset();
        }
    }

    // Weighing -> Diverting: dozwolone do slotu zwolnionego wcześniej W TYM SAMYM ticku.
    if (!plant.diverting.has_value() && plant.weighing.has_value()) {
        plant.diverting = std::move(plant.weighing);
        plant.weighing.reset();
        plant.diverting->divertingWaitTicks = 0;
    }

    // PresenceCheck -> Weighing: ten sam wzorzec.
    if (!plant.weighing.has_value() && plant.presenceCheck.has_value()) {
        plant.weighing = std::move(plant.presenceCheck);
        plant.presenceCheck.reset();
    }

    // Infeed -> PresenceCheck: ten sam wzorzec.
    if (!plant.presenceCheck.has_value() && plant.infeed.has_value()) {
        plant.presenceCheck = std::move(plant.infeed);
        plant.infeed.reset();
    }

    return result;
}
```

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
przed tym wywołaniem. Zweryfikowane na osobnym prototypie przed zamrożeniem tej lekcji — zobacz
zweryfikowany ślad niżej.

## Zweryfikowany ślad: trzy paczki, Light/Heavy/Light

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

Uzupełnij ciało `advance()` zgodnie z dokładnym algorytmem powyżej.

## Self-check

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
zachowanie z zamrożonego śladu powyżej by się zepsuło, i na czym konkretnie by to polegało?

**Dalej:** [Misja 31: korelacja per paczka](./03_korelacja_per_paczka.md).
