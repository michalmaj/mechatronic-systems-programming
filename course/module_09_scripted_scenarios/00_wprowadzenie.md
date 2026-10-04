🇵🇱 Polski | [🇬🇧 English](00_wprowadzenie.en.md)

# Moduł 9: zaplanowane scenariusze

## Gdzie jesteśmy

Dotychczas każdy przebieg symulacji powstawał z wywołań `request*()`, `inject*()`, `clear*()` i
`spawnItem()` umieszczonych pomiędzy kolejnymi wywołaniami `step()`. W tym module zapiszesz cały
eksperyment jako jedną wartość `Scenario`. Będzie ona zawierać polecenia operatora, terminy przybycia
paczek, przedziały aktywności usterek oraz czas trwania symulacji.

Funkcja `runScenario()` uruchomi taki opis na nowym obiekcie `Engine` i zwróci pełną sekwencję
wyników `TickResult`.

## Co się zmienia, a co pozostaje bez zmian

`Engine` nie otrzymuje nowego konstruktora ani dodatkowych pól. Obsługa scenariuszy korzysta z jego
publicznych metod, czyli tych samych, które wywołują dotychczasowe testy i program terminalowy.
`Scenario` jest dodatkowym sposobem sterowania symulacją, a nie zamiennikiem istniejącego interfejsu.

W symulatorze referencyjnym przyjęto inne rozwiązanie: `Engine` nie udostępnia metod `request*()`,
`inject*()` ani `clear*()`, a scenariusz całkowicie zastępuje bezpośrednie sterowanie. W tym kursie
oba sposoby pozostają dostępne.

`Scenario` przechowuje `operatorInputs`, `arrivals`, `sensorFaults`, `diverterFaults` i `duration`.
Funkcja `isValidScenario()` sprawdza te dane przed rozpoczęciem symulacji. `runScenario()` wykonuje
je tick po ticku i zwraca `std::optional<std::vector<TickResult>>`. Wynikiem jest `std::nullopt`,
jeśli scenariusz jest nieprawidłowy albo nie można go wykonać do końca.

## Trzy misje

- **Misja 33: model i sprawdzanie poprawności.** Poznasz definicję `Scenario` i pięciu typów, z
  których korzysta, a następnie zaimplementujesz `isValidScenario()`.
- **Misja 34: uruchamianie scenariusza.** Zaimplementujesz `runScenario()`, ustalisz kolejność
  operacji w ticku i zadbasz o powtarzalność wyników.
- **Misja 35: scenariusze w programie terminalowym.** Przygotujesz przebieg awarii dywertera znany z
  modułu 7 oraz nowy przykład z zaplanowanym przybyciem kilku paczek.

## Zanim zaczniesz

Typy `Item`, `Plant`, `Engine` i `TickResult` pozostają bez zmian. Plik
`apps/simulator_cli/main.cpp` jest natomiast nowy i nie stanowi części zadania. Gotowy program buduje
dwa scenariusze, uruchamia je i sprawdza podstawowe wyniki. Twoim zadaniem będzie implementacja
`isValidScenario()`, `runScenario()` oraz dwóch funkcji tworzących przykładowe scenariusze.

**Dalej:** [Misja 33: model i sprawdzanie poprawności](./01_model_i_walidacja.md).
