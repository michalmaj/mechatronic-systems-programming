🇵🇱 Polski | [🇬🇧 English](03_tryb_awarii.en.md)

# 7.3 Tryb awarii

## Problem

`Mode` nie wie jeszcze, że termin rutowania może zostać przekroczony, i nie ma sposobu, żeby się z
tego wycofać. Ale jest tu coś głębszego niż tylko brakująca reguła: `Mode` musi teraz odpowiadać na
dwa **różne** pytania, dostępne w dwóch **różnych** momentach tego samego ticku.

Wcześniejszy szkic tego projektu próbował dwóch skrótów. Pierwszy: przesunąć `Plant::advance()`
przed liczenie `Mode`. Odrzucony — cichaczem przesunąłby o jeden tick każdy istniejący test
sprawdzający dokładny moment ruchu paczki, łącznie z już istniejącymi testami z modułu 4. Drugi:
wywoływać dotychczasowy `modeStep()` dwukrotnie w jednym ticku, z częścią flag ręcznie wygaszonych na
`false`. To technicznie działa — ale zaciera fakt, że pojawiły się dwie naprawdę różne
odpowiedzialności pod jedną nazwą funkcji.

## Nowe elementy C++

```cpp
enum class Mode { Idle, Running, EStopped, Fault };

// Krok 1: poprzedni Mode + wejścia operatora/e-stopu/resetu z tego ticku -> Mode, którego aktuatory
// użyją do sterowania W TYM ticku. Zasada z modułu 5 pozostaje bez zmian;
// dochodzi jeden parametr i jedna reguła.
Mode modeStep(Mode current, bool startRequested, bool stopRequested,
              EStopLatchState latch = EStopLatchState::Released,
              bool resetRequested = false);

// Krok 2: modeForTick + to, co Plant::advance() właśnie zgłosił -> Mode faktycznie zapisany i
// zwrócony w TickResult. Zupełnie nowa funkcja.
Mode reactToSystemEvent(Mode modeForTick, std::optional<SystemEventKind> event);
```

`modeStep` **w ogóle nie wie** o `SystemEventKind` — nie ma go w żadnym parametrze. `reactToSystemEvent`
**w ogóle nie wie** o operatorze, zatrzasku e-stopu ani resecie — ma dwa parametry i jedno
zadanie. Nazwy pokazują podział odpowiedzialności; funkcje nie powielają swojej pracy.

## Poprawiona kolejność reguł w `modeStep`

Łańcuch reguł z modułu 5 zostaje w tej samej kolejności i bez zmian: bezwarunkowy priorytet zatrzasku
e-stopu, potem powrót z `EStopped` do `Idle`, potem `stopRequested`, potem `startRequested`, a na
końcu `current` bez zmian, gdy nic z powyższego nie pasuje. Nowa reguła wchodzi jako **trzecia** w tej
kolejności — zaraz po powrocie z `EStopped`, a przed `stopRequested` — i dotyczy wyłącznie przypadku
`current == Mode::Fault`. Jej dokładne zachowanie (kiedy wraca do `Idle`, kiedy zostaje w `Fault`)
opisuje sekcja „`Fault` jest zatrzaskowe” niżej.

Zwróć uwagę, czego tu **nie ma**: żadnego parametru `routingDeadlineMissed`, żadnej reguły o nim
wspominającej. Ta odpowiedzialność w całości należy teraz do `reactToSystemEvent`.

## Cała reguła `reactToSystemEvent`

Jedyny przypadek, w którym funkcja zwraca `Fault`, to jednoczesne spełnienie dwóch warunków:
`modeForTick == Mode::Running` oraz `event == SystemEventKind::RoutingDeadlineMissed`. W każdej innej
sytuacji — inny `modeForTick`, brak zdarzenia, albo `event == SystemEventKind::DiverterNotReady` —
funkcja zwraca `modeForTick` bez żadnej zmiany.

Funkcja nie ma dostępu do `start`/`stop`/`reset`/`latch`, więc nie może ponownie
wywołać żadnej reguły `modeStep`. Może odpalić wyłącznie z `Running` — a skoro `latch != Released`
zawsze wymusza `EStopped` już w kroku 1 `modeStep`, `modeForTick` nigdy nie jest jednocześnie
`Running` i pod aktywnym e-stopem w tym samym ticku. Żadna kombinacja wejść nie potrafi odpalić obu
ścieżek naraz.

## `Fault` jest zatrzaskowe

Reguła 3 obsługuje każdy przypadek `current == Fault` i jest sprawdzana **przed** regułami 4 i 5.
Skoro `current == Fault`
zawsze zwraca albo `Idle` (przy resecie), albo `Fault`, żadna z reguł 4/5 nigdy nie zostanie
osiągnięta, gdy trwa `Fault`. Ani `stopRequested`, ani `startRequested` nie mają żadnego efektu.
Jedyne wyjście to jawny `resetRequested` — i nawet wtedy trafiasz do `Idle`, nigdy prosto do
`Running`. Powrót zawsze przechodzi przez `Idle`, tak jak wznowienie pracy po awaryjnym stopie w
module 5.

## Dlaczego e-stop wciąż ma priorytet nad `Fault`

Reguła 1 (`latch != Released -> EStopped`) jest sprawdzana bezwarunkowo, zanim `modeStep` w ogóle
spojrzy na `current`. Nawet jeśli `current == Fault`, wciśnięty e-stop natychmiast wymusza
`EStopped`. To zamierzone: e-stop pozostaje najpilniejszą ścieżką w systemie, ponad wszystkim innym.

## Co już masz gotowe

[`include/psm/mode.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/include/psm/mode.hpp) — `Mode::Fault`, obie sygnatury już obecne.

[`src/mode.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/src/mode.cpp) — istniejące reguły `latch`/`EStopped`/`stopRequested`/
`startRequested` kompletne i w tej samej kolejności; brakuje tylko reguły 3 (`// TODO`) i całego
ciała `reactToSystemEvent` (`// TODO`).

## Co masz napisać

Dodaj regułę 3 do `modeStep`, we właściwym miejscu w łańcuchu. Uzupełnij `reactToSystemEvent`.

## Sprawdź się

```bash
ctest --preset test -L misja-27
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza całą regułę
`reactToSystemEvent`, zatrzaskowość `Fault` wobec `stopRequested`/`startRequested`, powrót przez
`Idle` przy resecie, i priorytet e-stopu nad `Fault`.

## Częste błędy

- **Umieszczenie reguły `Fault` po regułach `stopRequested`/`startRequested`** — wtedy oba mogłyby z
  niego wyjść, choć zatrzask ma tego zabraniać.
- **Próba przekazania `routingDeadlineMissed` do `modeStep`** — ta informacja nie jest jeszcze znana
  w momencie, w którym `modeStep` jest wołany; stąd druga funkcja.
- **`reactToSystemEvent` sprawdzające `current` zamiast `modeForTick`** — to dwie różne nazwy dla
  tego samego argumentu w tej sygnaturze, ale nazwa `modeForTick` jest tu specjalnie użyta, żeby
  podkreślić, że to wynik kroku 1, nie surowe pole `mode_`.

## Pytanie do zastanowienia

`reactToSystemEvent` nie przyjmuje `latch` ani `resetRequested`. Skoro `Fault` jest zatrzaskowe tylko
dzięki regule 3 w `modeStep`. Co zepsułoby się, gdyby `reactToSystemEvent` też mogła
zwrócić `Fault` z dowolnego stanu, nie tylko z `Running`?

**Dalej:** [Misja 28: silnik z wykrywaniem awarii](./04_silnik_z_wykrywaniem_awarii.md).
