🇵🇱 Polski | [🇬🇧 English](01_model_i_walidacja.en.md)

# 9.1 Model i sprawdzanie poprawności

## Problem

Dotychczas scenariusze były zapisane jako ciąg pojedynczych wywołań. Nie można było opisać całego
przebiegu jedną wartością i sprawdzić go przed uruchomieniem symulacji.

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

`ScriptedSensorFault` i `ScriptedDiverterFault` są osobnymi typami. Dzięki temu nie można połączyć
rodzaju usterki z niewłaściwym celem, na przykład usterki dywertera z czujnikiem. Taka pomyłka jest
wykrywana podczas kompilacji, a nie dopiero w trakcie działania programu.

`duration` jest polem `Scenario`, ponieważ czas trwania należy do opisu eksperymentu. Funkcja nie
przyjmuje go jako osobnego parametru.

Kolejność elementów w wektorach nie ma znaczenia. O ich wykonaniu decydują wartości `at`, `from` i
`until`, a nie pozycja w kontenerze.

## Dokładne reguły

1. Każdy przedział usterki spełnia `from < until` oraz `until <= duration`.
2. Przedziały dwóch usterek czujnika o tym samym `target` nie mogą się nakładać. Przedział
   `[from, until)` obejmuje `from`, ale nie obejmuje `until`. Usterki różnych czujników mogą być
   aktywne jednocześnie.
3. Przedziały usterek dywertera nie mogą się nakładać, ponieważ układ ma tylko jeden dywerter.
4. W jednym ticku dany `ScenarioInputKind` może wystąpić najwyżej raz. Dopuszczalne są najwyżej dwa
   różne polecenia i tylko w parze `{EmergencyStopReleased, Reset}`.

   Jest to bardziej rygorystyczne niż wymagania samego `Engine`. Funkcje `nextEStopLatchState()` i
   `modeStep()` potrafią bezpiecznie obsłużyć jednoczesne `Reset` i `StartRequested`, ale poprawny
   scenariusz powinien zapisać te polecenia w osobnych tickach.
5. Każde `operatorInputs.at` jest mniejsze od `duration`.
6. Każde `arrivals.at` jest mniejsze od `duration`.
7. W jednym ticku może być zaplanowane najwyżej jedno przybycie. `infeed` mieści tylko jedną paczkę,
   więc dwóch takich operacji nie da się wykonać.
8. Każdy `ItemId` w `arrivals` jest unikalny w całym `Scenario`. Jest to silniejszy warunek niż w
   `Plant`, gdzie identyfikator musi być unikalny tylko wśród paczek obecnych w danej chwili. Dzięki
   temu autor scenariusza nie musi ustalać, czy poprzednia paczka o tym samym identyfikatorze zdążyła
   już opuścić układ.

`duration == 0` jest poprawnym przypadkiem granicznym. Taki scenariusz musi mieć puste wektory,
ponieważ żadna nieujemna wartość `Tick` nie jest mniejsza od zera.

## Co już masz gotowe

Pliki
[`scenario_input.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/include/psm/scenario_input.hpp),
[`scripted_item_arrival.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/include/psm/scripted_item_arrival.hpp),
[`scripted_sensor_fault.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/include/psm/scripted_sensor_fault.hpp),
[`scripted_diverter_fault.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/include/psm/scripted_diverter_fault.hpp)
i
[`scenario.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/include/psm/scenario.hpp)
zawierają gotowe definicje typów oraz deklarację `isValidScenario()`.

## Co masz napisać

Uzupełnij `isValidScenario()` w
[`src/scenario.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/src/scenario.cpp)
zgodnie z ośmioma regułami opisanymi powyżej.

## Sprawdź się

```bash
ctest --preset test -L misja-33
```

Oczekiwany wynik: `100% tests passed`. Test sprawdza osobno każdą z ośmiu reguł oraz przypadek
`duration == 0`.

## Częste błędy

- **Dopuszczenie pary `{Reset, StartRequested}`**: fakt, że `Engine` potrafi ją obsłużyć, nie oznacza,
  że spełnia ona bardziej rygorystyczne wymagania `Scenario`.
- **Sprawdzenie nakładania się usterek czujników bez uwzględnienia `target`**: jednoczesne usterki
  różnych czujników są dozwolone.
- **Odrzucenie `duration == 0`**: pusty scenariusz o zerowym czasie trwania jest poprawny.

## Pytanie do zastanowienia

Reguła 8 wymaga unikalnych identyfikatorów w całym `Scenario`, choć `Plant` sprawdza tylko paczki
obecne w danej chwili. Podaj przykład scenariusza, który spełnia wymagania `Plant`, ale zostanie
odrzucony przez regułę 8.

**Dalej:** [Misja 34: uruchamianie scenariusza](./02_odtwarzacz.md).
