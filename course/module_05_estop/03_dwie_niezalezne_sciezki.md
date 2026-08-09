# 5.3 Dwie niezależne ścieżki

## Problem

`EStopLatchState` i rozszerzony `Mode` istnieją, ale nic jeszcze nie przekłada ich na konkretne
skutki dla aktuatorów.

## Nowe elementy C++

**Dwie czyste, bezstanowe funkcje decyzyjne:**

```cpp
struct SafetyDecision {
    bool overrideActive;
};

SafetyDecision checkEmergencyOverride(EStopLatchState latch);
bool diverterMayMove(Mode mode);
```

`checkEmergencyOverride` — prawda, gdy `latch` nie jest `Released` (`Engaged` i `Armed` liczą się
oba). `diverterMayMove` — prawda wyłącznie dla `Mode::Running`. Obie są zwykłymi, testowalnymi
funkcjami, dokładnie w duchu `classify`/`toDiverterCommand` z Controllera.

**`BeltMotor::forceStop()` jest inne z natury.** To **celowo mutująca operacja awaryjna**, nie czysta
decyzja. Zamrożone zachowanie: `forceStop()` ustawia **jednocześnie** `command_` na `Stop` **i**
`actual_` na `Stopped`, w tym samym wywołaniu — żądana komenda i fizyczny stan zostają natychmiast
zgodne, z pominięciem `RampingDown`. To sedno ścieżki awaryjnej: nie czeka na rampę, i nie zostawia
"chce jechać" jako zaległej komendy, którą kolejny tick mógłby przypadkiem zrealizować.

## Dlaczego nie ma tu `filterRoutineBeltCommand`

Mogłoby się wydawać naturalne dodanie funkcji filtrującej "żądaną" komendę pasa według `Mode` —
podobnie jak `diverterMayMove` filtruje ruch dywertera. Ale jedyna "żądana" wartość, jaka na tym
etapie w ogóle istnieje, to dokładnie `mode == Running ? Run : Stop` — czyli funkcja filtrująca
porównywałaby wartość z warunkiem, który ją właśnie wyprodukował. To tautologia, nic by nie zmieniała.
Ścieżka rutynowa na tym etapie **jest** po prostu istniejącym sterowaniem `Mode → BeltMotor` z Modułu
4, bez zmian. Filtrowanie bezpieczeństwa wróci w późniejszym module, gdy pojawi się naprawdę
niezależne "żądanie", które będzie miało sens filtrować.

## Co już masz gotowe

[`include/psm/safety_supervisor.hpp`](../../include/psm/safety_supervisor.hpp) — deklaracje
kompletne, jak wyżej.

[`src/safety_supervisor.cpp`](../../src/safety_supervisor.cpp) — puste szkielety obu funkcji.

`BeltMotor::forceStop()` została zadeklarowana w
[`include/psm/belt_motor.hpp`](../../include/psm/belt_motor.hpp); jej pusty szkielet czeka w
[`src/belt_motor.cpp`](../../src/belt_motor.cpp).

## Co masz napisać

- `checkEmergencyOverride` i `diverterMayMove` — dwie proste, jednolinijkowe decyzje.
- `BeltMotor::forceStop()` — dwa przypisania, dokładnie jak w zamrożonym zachowaniu powyżej.

Żadna z tych trzech rzeczy jeszcze nie dotyka `Engine` — to Misja 19.

## Self-check

```bash
ctest --preset test -L misja-18
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza obie funkcje decyzyjne,
a także — co ważne — że po `forceStop()` kolejne `resolve()` **nie** zaczyna ponownie rampowania w
górę (co udowadnia, że `command_` naprawdę zostało zmienione, nie tylko `actual_`).

## Częste błędy

- **`forceStop()` ustawiające tylko `actual_`**, nie `command_` — wtedy kolejne `resolve()` (wciąż
  "chcąc" `Run`) natychmiast zacznie `RampingUp` z powrotem, co całkowicie unieważnia sens awaryjnego
  zatrzymania.
- **`diverterMayMove` sprawdzające coś innego niż dokładnie `Mode::Running`** — żadna inna wartość
  `Mode` nie pozwala na ruch dywertera.

## Pytanie do zastanowienia

`checkEmergencyOverride` i `diverterMayMove` są czyste; `forceStop()` celowo nie jest. Dlaczego to
rozróżnienie ma znaczenie akurat dla operacji **awaryjnej**, a nie przeszkadzało w żadnej z
wcześniejszych, "zwykłych" metod klas w tym kursie?

**Dalej:** [Misja 19: silnik pod ochroną](./04_silnik_pod_ochrona.md).
