# 9.1 Model i walidacja

## Problem

Każdy scenariusz do tej pory był napisany ręcznie, jedno wywołanie na raz. Nie ma sposobu, żeby zapisać
"co ma się wydarzyć" jako jedną, sprawdzalną wartość, zanim cokolwiek zostanie uruchomione.

## Nowe elementy C++

```cpp
enum class ScenarioInputKind {
    EmergencyStopPressed, EmergencyStopReleased, Reset, StartRequested, StopRequested
};
struct ScenarioInput { Tick at; ScenarioInputKind kind; };

struct ScriptedItemArrival { Tick at; ItemId id; Grams mass; };

struct ScriptedSensorFault { Tick from; Tick until; SensorTarget target; SensorFaultKind kind; };
struct ScriptedDiverterFault { Tick from; Tick until; DiverterFaultKind kind; };

struct Scenario {
    std::vector<ScenarioInput> operatorInputs;
    std::vector<ScriptedItemArrival> arrivals;
    std::vector<ScriptedSensorFault> sensorFaults;
    std::vector<ScriptedDiverterFault> diverterFaults;
    Tick duration;
};

bool isValidScenario(const Scenario& scenario);
```

`ScriptedSensorFault`/`ScriptedDiverterFault` są dwoma **osobnymi** typami, nie jednym wspólnym
"usterka + cel" — dokładnie ta sama decyzja co `SensorFaultKind`/`DiverterFaultKind` w Module 7:
nielegalna kombinacja ma być niewyrażalna w typach, nie odrzucana dopiero w trakcie działania programu.

`duration` żyje na `Scenario`, nie jako osobny parametr — scenariusz ma być kompletnym opisem
powtarzalnego eksperymentu, a to, jak długo trwa, jest częścią tego opisu.

**Kolejność elementów w każdym wektorze nie ma znaczenia.** Każda reguła niżej jest zdefiniowana przez
wartości `at`/`from`/`until` zapisane w danych, nie przez pozycję w wektorze.

## Dokładne reguły

1. Każdy przedział usterki: `from < until` i `until <= duration`.
2. Żadne dwie `ScriptedSensorFault` o tym samym `target` nie mogą się nakładać `[from, until)`.
   Nakładanie się dla *różnych* targetów jest w porządku.
3. Żadne dwie `ScriptedDiverterFault` nie mogą się nakładać (jest jeden dywerter).
4. Wejścia operatora, per tick: żaden powtórzony `ScenarioInputKind` na tym samym ticku; co najwyżej
   dwa różne rodzaje na tym samym ticku, i tylko jeśli to dokładnie `{EmergencyStopReleased, Reset}`.
   **To węższy kontrakt niż sam `Engine`** — `nextEStopLatchState`/`modeStep` obsługują `Reset` i
   `StartRequested` ustawione jednocześnie bezpiecznie (niezmienione od Modułu 5), ale odporność `Engine`
   na kombinację flag to inny kontrakt niż to, co dobrze napisany `Scenario` powinien *mówić*. Scenariusz,
   który chce obu efektów, zapisuje je na osobnych tickach.
5. Każde `operatorInputs.at` musi być `< duration`.
6. Każde `arrivals.at` musi być `< duration`.
7. Żadne dwa `arrivals` nie mogą dzielić tego samego `at` — to zawsze niewykonalne (Infeed mieści
   jedną paczkę), więc wykrywalne statycznie.
8. Każdy `ItemId` w `arrivals` musi być unikalny globalnie w całym `Scenario` — nie tylko "unikalny
   wśród aktualnie obecnych" (słabszy invariant `Plant` z Modułu 8). To celowe uproszczenie: autor
   scenariusza nie musi rozumować o dokładnym momencie odjazdu, żeby wiedzieć, czy ponowne użycie id
   jest bezpieczne.

`duration == 0` jest jawnie poprawnym, granicznym przypadkiem — pusty scenariusz o zerowym czasie
trwania. Powyższe reguły same to gwarantują: żaden `at`/`from` nie może spełnić `< 0` dla nieujemnego
`Tick`, więc poprawny scenariusz o `duration == 0` musi mieć wszystkie wektory puste.

## Co już masz gotowe

[`include/psm/scenario_input.hpp`](../../include/psm/scenario_input.hpp),
[`scripted_item_arrival.hpp`](../../include/psm/scripted_item_arrival.hpp),
[`scripted_sensor_fault.hpp`](../../include/psm/scripted_sensor_fault.hpp),
[`scripted_diverter_fault.hpp`](../../include/psm/scripted_diverter_fault.hpp),
[`scenario.hpp`](../../include/psm/scenario.hpp) — wszystkie kształty i sygnatura `isValidScenario`
gotowe.

## Co masz napisać

Uzupełnij ciało `isValidScenario` w [`src/scenario.cpp`](../../src/scenario.cpp) zgodnie z ośmioma
regułami powyżej.

## Sprawdź się

```bash
ctest --preset test -L misja-33
```

Oczekiwany wynik: `100% tests passed`. Test sprawdza każdą z ośmiu reguł osobno, plus graniczny
przypadek `duration == 0`.

## Częste błędy

- **Traktowanie `{Reset, StartRequested}` jako dozwolonej pary** — to *nie* jest to samo pytanie co
  "czy `Engine` sobie z tym poradzi". `Scenario` ma węższy kontrakt.
- **Sprawdzanie nakładania się usterek czujników bez uwzględnienia `target`** — nakładanie się dla
  różnych czujników jest w porządku, tylko dla tego samego jest błędem.
- **Odrzucanie `duration == 0`** — to jawnie poprawny przypadek, nie błąd do złapania.

## Pytanie do zastanowienia

Reguła 8 wymaga globalnej unikalności id w całym `Scenario`, mimo że `Plant` sam wymaga tylko
unikalności wśród aktualnie obecnych paczek. Jaki konkretny scenariusz byłby poprawny względem reguły
`Plant`, ale odrzucony przez regułę 8?

**Dalej:** [Misja 34: odtwarzacz](./02_odtwarzacz.md).
