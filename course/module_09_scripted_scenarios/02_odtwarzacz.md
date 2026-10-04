🇵🇱 Polski | [🇬🇧 English](02_odtwarzacz.en.md)

# 9.2 Uruchamianie scenariusza

## Problem

Poprawny `Scenario` jest tylko opisem danych wejściowych. Potrzebna jest jeszcze funkcja, która
zastosuje je w odpowiednich tickach, wykona kolejne kroki `Engine` i zbierze wyniki `TickResult`.

## Nowy element C++

```cpp
std::optional<std::vector<TickResult>> runScenario(const Scenario& scenario);
```

Funkcja tworzy wewnątrz nowy `Engine`, zamiast przyjmować `Engine&`. Każde uruchomienie zaczyna się
dzięki temu od takiego samego, pustego stanu.

Pomocnicze funkcje `activeSensorFault()` i `activeDiverterFault()` sprawdzają, czy dana usterka jest
aktywna w wybranym ticku. Umieść je w anonimowej przestrzeni nazw w `src/scenario.cpp`. Są szczegółem
implementacji `runScenario()` i nie powinny znaleźć się w `scenario.hpp`. Ich upublicznienie
wymagałoby utrzymywania dodatkowego kontraktu, choć żadna inna część programu ich nie potrzebuje.

## Kolejność operacji w ticku

W każdym ticku zachowaj następującą kolejność:

**przybycia paczek → polecenia operatora → ustawienie stanu usterek → `Engine::step()` → zapisanie
wyniku**

Stan usterek wyznaczaj od początku w każdym ticku. Następnie bezwarunkowo wywołuj odpowiednie metody
`injectSensorFault()`, `clearSensorFault()`, `injectDiverterFault()` i `clearDiverterFault()`. Nie
musisz pamiętać, czy usterka była aktywna poprzednio. Ponowne ustawienie albo wyczyszczenie tego
samego stanu nie zmienia wyniku.

## Zgłaszanie błędów przez typ zwracany

Nie używaj do tego `assert()`. Asercje mogą zostać wyłączone w konfiguracji Release, natomiast
nieprawidłowy scenariusz jest zwykłym przypadkiem, który wywołujący powinien móc obsłużyć.

`runScenario()` zwraca `std::nullopt` w dwóch sytuacjach:

- `isValidScenario()` odrzuci dane przed wykonaniem pierwszego ticku,
- nie uda się dodać zaplanowanej paczki, ponieważ `Plant::infeed` nadal jest zajęte przez poprzednią.

Drugiego przypadku nie zawsze można wykryć przed uruchomieniem. Zależy on między innymi od pracy
taśmy i trybu wynikającego z poleceń operatora. Nie ponawiaj nieudanego przybycia w następnym ticku.
Traktuj je jako błąd scenariusza i zwróć `std::nullopt`.

## Powtarzalność wyników

Dwa osobne wywołania `runScenario(scenario)` dla tego samego poprawnego i wykonalnego scenariusza
mają zwrócić sekwencje `TickResult` o takich samych wartościach wszystkich pól. Nie chodzi o
identyczną reprezentację obiektów w pamięci, lecz o ten sam wynik symulacji.

Jest to możliwe, ponieważ każde wywołanie tworzy nowy `Engine`, a symulator jest deterministyczny.
Nie korzysta z zegara systemowego, losowości ani ukrytego stanu globalnego.

## Dlaczego `{EmergencyStopReleased, Reset}` w jednym ticku jest bezpieczne

Wyjaśnia to kolejność warunków w `nextEStopLatchState()` z modułu 5. Jeśli poprzednim stanem jest
`Engaged`, jednoczesne `released` i `resetRequested` uruchamia najpierw gałąź obsługującą
`previous == Engaged`. W tym wywołaniu `resetRequested` nie jest jeszcze brane pod uwagę, a stan
zmienia się na `Armed`, nie na `Released`.

Para jest więc jednoznacznie zdefiniowana i bezpieczna. Nie skraca jednak dwuetapowego powrotu po
zatrzymaniu awaryjnym.

## Co już masz gotowe

W pliku
[`include/psm/scenario.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/include/psm/scenario.hpp)
znajdziesz deklarację `runScenario()`. Masz też własną implementację `isValidScenario()` z misji 33.

## Co masz napisać

Uzupełnij `runScenario()` w
[`src/scenario.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/src/scenario.cpp).
Dodaj również prywatne funkcje `activeSensorFault()` i `activeDiverterFault()`. Zachowaj kolejność
operacji opisaną powyżej.

## Sprawdź się

```bash
ctest --preset test -L misja-34
```

Oczekiwany wynik: `100% tests passed`. Test wykonuje tę samą krótką sekwencję bezpośrednio oraz za
pomocą `Scenario` i `runScenario()`, a następnie porównuje oba wyniki pole po polu. Sprawdza również:

- parę `{EmergencyStopReleased, Reset}` w jednym ticku,
- `std::nullopt` po próbie dodania paczki do zajętego `infeed`,
- odrzucenie nieprawidłowego scenariusza przed jego uruchomieniem,
- pusty, ale obecny wynik dla `duration == 0`,
- brak wpływu kolejności elementów w wektorach na wynik.

## Częste błędy

- **Pamiętanie stanu usterki z poprzedniego ticku**: nie jest potrzebne. W każdym ticku oblicz stan
  od początku i ustaw go przez `inject*()` albo `clear*()`.
- **Ponawianie nieudanego przybycia paczki**: funkcja ma od razu zwrócić `std::nullopt`.
- **Dodanie `activeSensorFault()` i `activeDiverterFault()` do publicznego nagłówka**: pozostaw je
  jako prywatne funkcje w `scenario.cpp`.

## Pytanie do zastanowienia

`runScenario()` wywołuje `isValidScenario()` przed rozpoczęciem symulacji. Co mogłoby się stać, gdyby
poszczególne reguły były sprawdzane dopiero w trakcie wykonywania scenariusza?

**Dalej:** [Misja 35: scenariusze w programie terminalowym](./03_integracja_w_cli.md).
