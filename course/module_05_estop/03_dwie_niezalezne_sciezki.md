🇵🇱 Polski | [🇬🇧 English](03_dwie_niezalezne_sciezki.en.md)

# 5.3 Dwie niezależne ścieżki

## Problem

`EStopLatchState` i rozszerzony `Mode` istnieją, ale nic jeszcze nie przekłada ich na konkretne
zachowanie elementów wykonawczych.

## Nowe elementy C++

**Dwie czyste funkcje decyzyjne, które nie przechowują stanu:**

```cpp
struct SafetyDecision {
    bool overrideActive;
};

SafetyDecision checkEmergencyOverride(EStopLatchState latch);
bool diverterMayMove(Mode mode);
```

`checkEmergencyOverride` zwraca `true`, gdy `latch` ma wartość inną niż `Released`. Dotyczy to zarówno
`Engaged`, jak i `Armed`. `diverterMayMove` zwraca `true` wyłącznie dla `Mode::Running`. Obie są
zwykłymi, łatwymi do przetestowania funkcjami, podobnie jak `classify` i `toDiverterCommand`.

**`BeltMotor::forceStop()` ma inną rolę.** Jest operacją awaryjną zmieniającą stan. `forceStop()`
ustawia **jednocześnie** `command_` na `Stop` i `actual_` na `Stopped`. W tym uproszczonym modelu stan
silnika zmienia się natychmiast, z pominięciem `RampingDown`. Zmiana obu pól jest ważna: po
zatrzymaniu nie może pozostać wcześniejsze polecenie `Run`, które następny tick mógłby ponownie
wykonać.

## Dlaczego nie ma tu `filterRoutineBeltCommand`

Można byłoby dodać funkcję filtrującą polecenie dla silnika na podstawie `Mode`, podobnie jak
`diverterMayMove` zezwala na ruch dywertera. Na tym etapie polecenie silnika powstaje jednak
bezpośrednio z warunku `mode == Running ? Run : Stop`. Dodatkowa funkcja sprawdzałaby więc ten sam
warunek, który przed chwilą utworzył polecenie, i nie zmieniałaby wyniku. Zwykłe sterowanie
`Mode → BeltMotor` z modułu 4 pozostaje bez zmian. Osobne filtrowanie przyda się dopiero wtedy, gdy
pojawi się niezależne żądanie ruchu.

## Co już masz gotowe

[`include/psm/safety_supervisor.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-05-start/include/psm/safety_supervisor.hpp)
zawiera kompletne deklaracje pokazane wyżej.

W pliku
[`src/safety_supervisor.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-05-start/src/safety_supervisor.cpp)
znajdziesz puste szkielety obu funkcji.

`BeltMotor::forceStop()` została zadeklarowana w
[`include/psm/belt_motor.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-05-start/include/psm/belt_motor.hpp); jej pusty szkielet czeka w
[`src/belt_motor.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-05-start/src/belt_motor.cpp).

## Co masz napisać

- `checkEmergencyOverride` i `diverterMayMove`: napisz dwie proste funkcje decyzyjne.
- `BeltMotor::forceStop()`: wykonaj dwa przypisania opisane wyżej.

Żaden z tych elementów nie zmienia jeszcze `Engine`. Zrobisz to w misji 19.

## Sprawdź się

```bash
ctest --preset test -L misja-18
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza obie funkcje decyzyjne,
a także zachowanie `forceStop()`. Po jego wywołaniu kolejne `resolve()` **nie** może rozpocząć
ponownego rozruchu. Potwierdza to, że zmieniło się zarówno `command_`, jak i `actual_`.

## Częste błędy

- **Ustawienie przez `forceStop()` tylko `actual_`, bez zmiany `command_`:** kolejne `resolve()`,
  nadal mając polecenie `Run`, natychmiast przejdzie z powrotem do `RampingUp` i unieważni
  zatrzymanie.
- **`diverterMayMove` dopuszczające stan inny niż `Mode::Running`:** żadna inna wartość
  `Mode` nie pozwala na ruch dywertera.

## Pytanie do zastanowienia

`checkEmergencyOverride` i `diverterMayMove` tylko zwracają decyzje, natomiast `forceStop()` zmienia
stan silnika. Dlaczego ścieżka awaryjna potrzebuje operacji, która bezpośrednio wymusza zmianę
stanu?

**Dalej:** [Misja 19: obsługa zatrzymania awaryjnego w Engine](./04_silnik_pod_ochrona.md).
