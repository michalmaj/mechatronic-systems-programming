🇵🇱 Polski | [🇬🇧 English](03_tryb_awarii.en.md)

# 7.3 Tryb awarii

## Problem

`Mode` nie reaguje jeszcze na przekroczenie czasu oczekiwania na dywerter. Dodanie tej reakcji
wymaga rozdzielenia dwóch decyzji podejmowanych w różnych momentach ticku.

Najpierw trzeba uwzględnić polecenia operatora, stan przycisku awaryjnego i żądanie resetu. Wynik
tej decyzji określa, czy w bieżącym ticku mogą działać elementy wykonawcze. Dopiero później
`Plant::advance()` może zgłosić zdarzenie, na podstawie którego program przejdzie do trybu `Fault`.

Nie należy w tym celu przenosić `Plant::advance()` na początek ticku. Zmieniłoby to moment ruchu
paczki i wynik istniejących testów. Dwukrotne wywołanie `modeStep()` również nie jest dobrym
rozwiązaniem, bo ta funkcja musiałaby obsługiwać dwie różne grupy informacji. Zamiast tego każda
decyzja otrzyma własną funkcję.

## Nowe elementy C++

```cpp
enum class Mode { Idle, Running, EStopped, Fault };

// Krok 1: wyznacz tryb używany przez elementy wykonawcze w bieżącym ticku.
Mode modeStep(Mode current, bool startRequested, bool stopRequested,
              EStopLatchState latch = EStopLatchState::Released,
              bool resetRequested = false);

// Krok 2: uwzględnij zdarzenie zgłoszone przez Plant::advance().
Mode reactToSystemEvent(Mode modeForTick, std::optional<SystemEventKind> event);
```

`modeStep()` nie otrzymuje `SystemEventKind`, więc odpowiada wyłącznie za wejścia dostępne na
początku ticku. Z kolei `reactToSystemEvent()` nie otrzymuje poleceń operatora, stanu przycisku
awaryjnego ani żądania resetu. Jej jedynym zadaniem jest reakcja na zdarzenie z `Plant`.

## Kolejność reguł w `modeStep()`

Dotychczasowe reguły zachowują swoją kolejność:

1. Aktywny przycisk awaryjny wymusza `EStopped`.
2. Po zwolnieniu przycisku tryb `EStopped` przechodzi do `Idle`.
3. Tryb `Fault` pozostaje aktywny albo po resecie przechodzi do `Idle`.
4. `stopRequested` przełącza program do `Idle`.
5. `startRequested` przełącza program do `Running`.
6. Jeśli żaden warunek nie został spełniony, tryb się nie zmienia.

Nową regułę dotyczącą `Fault` umieść po obsłudze `EStopped`, ale przed poleceniami zatrzymania i
uruchomienia. W `modeStep()` nie ma natomiast reguły dotyczącej `RoutingDeadlineMissed`. Takie
zdarzenie jest znane dopiero później i obsługuje je `reactToSystemEvent()`.

## Reguła `reactToSystemEvent()`

Funkcja zwraca `Fault` tylko wtedy, gdy oba warunki są spełnione jednocześnie:

- `modeForTick == Mode::Running`,
- `event == SystemEventKind::RoutingDeadlineMissed`.

W każdym innym przypadku zwraca niezmienione `modeForTick`. Dotyczy to braku zdarzenia,
`DiverterNotReady` oraz każdego trybu innego niż `Running`.

Stan przycisku awaryjnego został już uwzględniony przez `modeStep()`. Jeśli przycisk jest aktywny,
`modeForTick` ma wartość `EStopped`, więc `reactToSystemEvent()` nie może w tym samym ticku przejść
do `Fault`.

## Z trybu `Fault` wychodzi się tylko przez reset

Reguła dotycząca `current == Mode::Fault` jest sprawdzana przed `stopRequested` i `startRequested`.
Bez żądania resetu zwraca `Fault`, dlatego polecenia zatrzymania i uruchomienia nie mają w tym trybie
wpływu na wynik. `resetRequested` powoduje przejście do `Idle`, nigdy bezpośrednio do `Running`.
Ponowne uruchomienie wymaga więc osobnego polecenia, podobnie jak po zatrzymaniu awaryjnym.

## Dlaczego `EStopped` ma pierwszeństwo przed `Fault`

Warunek `latch != Released` jest sprawdzany jako pierwszy, zanim funkcja sprawdzi `current`. Dzięki
temu aktywny przycisk awaryjny zawsze wymusza `EStopped`, również wtedy, gdy poprzednim trybem był
`Fault`.

## Co już masz gotowe

W pliku
[`include/psm/mode.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/include/psm/mode.hpp)
znajdziesz `Mode::Fault` oraz deklaracje obu funkcji.

W pliku
[`src/mode.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/src/mode.cpp)
są już dotychczasowe reguły dotyczące `latch`, `EStopped`, `stopRequested` i `startRequested`.
Brakuje reguły dla `Fault` oraz ciała `reactToSystemEvent()`. Oba miejsca oznaczono `// TODO`.

## Co masz napisać

Dodaj obsługę `Fault` we właściwym miejscu funkcji `modeStep()`. Następnie uzupełnij
`reactToSystemEvent()`.

## Sprawdź się

```bash
ctest --preset test -L misja-27
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza regułę
`reactToSystemEvent()`, pozostawanie w `Fault` aż do resetu, powrót przez `Idle` oraz pierwszeństwo
`EStopped` przed `Fault`.

## Częste błędy

- **Umieszczenie reguły `Fault` po obsłudze `stopRequested` i `startRequested`**: oba polecenia
  mogłyby wtedy zakończyć tryb awarii.
- **Dodanie `routingDeadlineMissed` do parametrów `modeStep()`**: ta informacja nie jest jeszcze
  dostępna podczas wywołania funkcji. Właśnie dlatego potrzebna jest `reactToSystemEvent()`.
- **Odczyt pola `mode_` wewnątrz `reactToSystemEvent()`**: funkcja powinna korzystać wyłącznie z
  parametru `modeForTick`, czyli wyniku pierwszego kroku.

## Pytanie do zastanowienia

`reactToSystemEvent()` nie otrzymuje stanu przycisku awaryjnego ani `resetRequested`. Co mogłoby się
zepsuć, gdyby ta funkcja mogła zwrócić `Fault` z dowolnego trybu, a nie tylko z `Running`?

**Dalej:** [Misja 28: `Engine` z wykrywaniem awarii](./04_silnik_z_wykrywaniem_awarii.md).
