🇵🇱 Polski | [🇬🇧 English](04_silnik_z_wykrywaniem_awarii.en.md)

# 7.4 `Engine` z wykrywaniem awarii

## Problem

Zablokowany dywerter, licznik w `Plant` i dwuetapowe wyznaczanie `Mode` są już gotowe. Trzeba teraz
połączyć je w `Engine`, aby awaria wpływała na działanie całej symulacji.

## Nowe elementy C++

Usterki czujników obsługują teraz metody `injectSensorFault()` i `clearSensorFault()`. Korzystają z
typów `SensorFaultKind` oraz `SensorTarget`. Ta zmiana jest już wprowadzona i nie wymaga Twojej
pracy.

Do obsługi usterki dywertera służą osobne metody:

```cpp
void injectDiverterFault(DiverterFaultKind kind);
void clearDiverterFault();
```

Nie potrzebują parametru wskazującego cel, ponieważ w układzie jest tylko jeden dywerter.

## Rozszerzenie `step()`

Większość kodu z modułu 6 pozostaje bez zmian. Nadal korzystasz z flag wejściowych, `latch_`,
`decision`, czujników i `ControllerState`. Wprowadź cztery zmiany, w podanej kolejności:

1. Na początku `step()` wywołaj `modeStep(...)` i zapisz wynik w lokalnej zmiennej `modeForTick`.
   Nie przypisuj go jeszcze do `mode_`. Ta wartość określa tryb pracy w bieżącym ticku.
2. Przy sterowaniu dywerterem i taśmą korzystaj z `modeForTick`, a nie z `mode_`. Do
   `diverter_.resolve(...)` przekaż dodatkowo `diverterFault_`, tak jak w module 6 przekazujesz
   usterki do czujników.
3. Wywołuj `psm::advance(...)` pod tym samym warunkiem co wcześniej, czyli gdy taśma rzeczywiście
   jest w stanie `Running`. Tym razem zapisz wartość zwróconą przez funkcję. Informuje ona o
   zdarzeniu, które wystąpiło w bieżącym ticku.
4. Po wywołaniu `advance()` ustaw `mode_` za pomocą
   `reactToSystemEvent(modeForTick, event)`. To jedyne przypisanie nowej wartości do `mode_` w tym
   ticku.

Dodaj też `event` do zwracanego `TickResult`.

## Zatrzymanie taśmy rozpoczyna się tick później

Zdarzenie `RoutingDeadlineMissed` zostaje wykryte pod koniec ticku. Elementy wykonawcze działają
wtedy jeszcze na podstawie `modeForTick == Running`, ponieważ bez aktywnej próby ustawienia
dywertera nie dałoby się wykryć przekroczenia limitu. Dlatego wynik tego ticku może zawierać
`mode = Fault` i jednocześnie `beltActual = Running`.

Taśma zacznie zwalniać w następnym ticku. Wtedy `modeForTick` będzie już miało wartość `Fault`, a
napęd przejdzie do `RampingDown`.

Przycisk awaryjny działa inaczej. `decision.overrideActive` powoduje natychmiastowe wywołanie
`beltMotor_.forceStop()`, niezależnie od `Mode`. Tryb `Fault` sygnalizuje tutaj problem z wyborem
trasy i nie korzysta z tej ścieżki awaryjnego zatrzymania. Opóźnienie o jeden tick jest więc
zamierzonym zachowaniem modelu.

## Przykładowy scenariusz usunięcia awarii

```text
krok 1: paczka trafia do Infeed, mode=Running, belt=RampingUp. Taśma dopiero się rozpędza.
krok 2: paczka przechodzi do PresenceCheck, belt=Running.
krok 3: paczka przechodzi do Weighing.
krok 4: paczka przechodzi do Diverting, a klasyfikacja jest już w ControllerState.
krok 5: event=DiverterNotReady. To pierwsza próba przy zablokowanym dywerterze.
krok 6: event=RoutingDeadlineMissed, mode=Fault, belt=Running.
krok 7: mode=Fault, belt=RampingDown. Taśma zaczyna zwalniać.
krok 8: mode=Fault, belt=Stopped.

wywołaj clearDiverterFault()

krok 9: mode=Fault. Usunięcie blokady nie zmienia trybu pracy.

wywołaj requestReset()

krok 10: mode=Idle. Reset kończy tryb Fault, a paczka pozostaje w Diverting.

wywołaj requestStart()

krok 11: mode=Running, belt=RampingUp. Taśma ponownie się rozpędza.
krok 12: belt=Running, event=brak. Dywerter zdążył się ustawić i paczka trafia do wyjścia.
```

Po usunięciu blokady dywerter otrzymuje polecenia w krokach 11 i 12. W tym samym czasie taśma
rozpędza się od zera. Gdy `advance()` ponownie sprawdza położenie dywertera, mechanizm jest już
ustawiony. Dlatego po wznowieniu pracy nie pojawia się kolejne `DiverterNotReady`.

Gdyby nie wywołano `clearDiverterFault()`, program ponownie zgłosiłby `DiverterNotReady`, następnie
`RoutingDeadlineMissed` i wrócił do `Fault`. `requestReset()` kończy tryb awarii, ale nie usuwa jej
przyczyny.

## Co już masz gotowe

W `include/psm/engine.hpp` znajdują się wszystkie potrzebne pola i deklaracje. W
`src/engine.cpp` gotowe są metody `injectSensorFault()` i `clearSensorFault()`. Znajdziesz tam też
puste szkielety `injectDiverterFault()` i `clearDiverterFault()` oraz ciało `step()` z modułu 6.

## Co masz napisać

- W `Engine::injectDiverterFault(DiverterFaultKind)` zapisz `kind` w `diverterFault_`.
- W `Engine::clearDiverterFault()` ustaw `diverterFault_` na `std::nullopt`.
- Rozszerz `Engine::step()` zgodnie z opisem powyżej. Do sterowania elementami wykonawczymi użyj
  `modeForTick`, przekaż `diverterFault_` do `diverter_.resolve()`, a na końcu ustaw `mode_` za pomocą
  `reactToSystemEvent()`. Umieść też `event` w `TickResult`.
- W `apps/simulator_cli/main.cpp` zaimplementuj opisany scenariusz usunięcia awarii.

## Sprawdź się

```bash
ctest --preset test -L misja-28
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test przechodzi krok po kroku przez
cały scenariusz usunięcia awarii.

Zbuduj i uruchom także program:

```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

## Koniec modułu: pełny zestaw testów

```bash
ctest --preset test
```

Oczekiwany wynik: przechodzą wszystkie testy od `misja-1` do `misja-4` oraz od `misja-6` do
`misja-28`.

## Zapisz swoją pracę

```bash
git status
git add <pliki które zmieniłeś>
git commit -m "..."
```

## Częste błędy

- **Sterowanie elementami wykonawczymi na podstawie `mode_` zamiast `modeForTick`**: pole `mode_`
  otrzymuje nową wartość dopiero po wywołaniu `reactToSystemEvent()`.
- **Wywołanie `reactToSystemEvent()` przed `Plant::advance()`**: zdarzenie nie jest wtedy jeszcze
  znane. Najpierw zapisz wynik `advance()`, a dopiero później wyznacz końcowy tryb.
- **Natychmiastowe wymuszenie zatrzymania taśmy po przejściu do `Fault`**: w tym modelu taśma zaczyna
  zwalniać w następnym ticku.

## Pytanie do zastanowienia

W opisanym scenariuszu taśma i dywerter kończą ruch w tym samym kroku. Załóż, że rozpędzanie taśmy
trwa o jeden tick dłużej niż ustawianie dywertera. Co zawierałby `TickResult` w chwili, gdy dywerter
jest już ustawiony, ale taśma nie osiągnęła jeszcze stanu `Running`?

## Koniec modułu 7

Tryb `Mode::Fault` ma teraz konkretną, przetestowaną przyczynę. `modeStep()` obsługuje informacje
dostępne na początku ticku, a `reactToSystemEvent()` zdarzenie zgłoszone po wykonaniu kroku
symulacji. Dzięki temu każda funkcja odpowiada za jedną decyzję, podejmowaną we właściwym momencie.
