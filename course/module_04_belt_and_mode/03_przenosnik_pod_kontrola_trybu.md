🇵🇱 Polski | [🇬🇧 English](03_przenosnik_pod_kontrola_trybu.en.md)

# 4.3 Przenośnik pod kontrolą trybu

## Problem

`BeltMotor` i `Mode` są już gotowe, ale nie zostały jeszcze połączone z działającą symulacją. Paczka
może się więc poruszać nawet wtedy, gdy silnik przenośnika nie otrzymał polecenia uruchomienia.

## Nowe elementy C++

**`Engine::requestStart()` i `Engine::requestStop()`** zapisują żądania uruchomienia lub zatrzymania
systemu. Podobnie jak `spawnItem`, można je wywoływać między tickami. Same nie wykonują kroku
symulacji.

Do `Engine` dochodzą prywatne pola `mode_` i `beltMotor_` oraz dwie flagi, `startRequested_` i
`stopRequested_`. Flagi zapamiętują żądania, które zostaną obsłużone w następnym `step()`.

## `Mode` i `BeltMotorState` opisują różne rzeczy

**`Mode`** określa, czy system ma pracować. Zmienia się od razu w ticku, w którym `modeStep` oblicza
nową wartość. **`BeltMotorState`** opisuje rzeczywisty stan silnika i zmienia się z opóźnieniem,
ponieważ rozruch i zatrzymanie wymagają czasu.

Konsekwencja: `Mode::Idle` razem z `BeltMotorState::RampingDown` oraz `Mode::Running` razem z
`BeltMotorState::RampingUp` to **poprawne stany przejściowe**, a nie błędy. Zobaczysz je podczas
sprawdzania tej misji.

## Rozszerzona kolejność `step()`

1. Odczytaj oczekujące żądania uruchomienia i zatrzymania, oblicz
   `mode_ = modeStep(mode_, ...)`, a następnie wyzeruj obie flagi. Reguła konfliktu z misji 14 nadal
   obowiązuje.
2. `beltMotor_.setCommand(mode_ == Running ? Run : Stop)`, `beltMotor_.resolve()`.
3. Decyzja sterownika → `diverter_.setCommand` → `diverter_.resolve()`. Ta część nie zmienia się od
   modułu 3.
4. Wywołaj `psm::advance(plant_, diverter_)` **tylko wtedy**, gdy
   `beltMotor_.actualState() == Running`. W przeciwnym razie paczka pozostaje w miejscu.
5. Utwórz rozszerzony `TickResult` i zwiększ `tick_`.

## Przykładowy przebieg

Utwórz `Engine`, dodaj paczkę przez `spawnItem()`, wywołaj `requestStart()`, a następnie dwa razy
`step()`:

- **Pierwszy `step()`**: `mode` staje się `Running` w tym samym wywołaniu, `beltActual` staje się
  `RampingUp`, ponieważ silnik rozpoczyna rozruch ze stanu `Stopped`. **Paczka się nie rusza**, bo
  warunek `beltActual == Running` nie jest jeszcze spełniony.
- **Drugi `step()`**: `mode` zostaje `Running`, `beltActual` staje się `Running` (jeszcze jeden krok
  `resolve()` od `RampingUp`). Warunek ruchu jest już spełniony i paczka przesuwa się **o jeden
  krok**.

Test tej misji sprawdza właśnie taki przebieg.

## Co już masz gotowe

`include/psm/engine.hpp` i `include/psm/tick_result.hpp` mają już wszystkie potrzebne pola i
deklaracje. W `src/engine.cpp` znajdziesz puste szkielety `requestStart()` i `requestStop()`. Ciało
`step()` nadal odpowiada wersji z modułu 3, którą teraz rozszerzysz.

## Co masz napisać

- `Engine::requestStart()`: ustaw `startRequested_` na `true`.
- `Engine::requestStop()`: ustaw `stopRequested_` na `true`.
- `Engine::step()`: dodaj pięć opisanych wyżej kroków.
- `apps/simulator_cli/main.cpp`: wywołaj `engine.requestStart()` przed pętlą (inaczej taśma nigdy
  nie ruszy) i oprócz istniejącej linii `describe()` wypisuj również `mode` oraz `beltActual`.

Nie zmieniaj **`describe()`**. Ta funkcja nadal zwraca tekst opisujący `tick` i `item`. Informacje o
trybie oraz silniku wypisz osobno w `main()`.

## Sprawdź się

```bash
ctest --preset test -L misja-15
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`.

Uruchom też program:

```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

## Koniec modułu: pełny zestaw testów

```bash
ctest --preset test
```

Oczekiwany wynik: przechodzą wszystkie testy od `misja-1` do `misja-4` oraz od `misja-6` do
`misja-15`.

## Zapisz swoją pracę

```bash
git status
git add <pliki które zmieniłeś>
git commit -m "..."
```

## Częste błędy

- **Wywołanie `advance()` bez sprawdzenia stanu silnika:** warunek ruchu musi używać stanu **po**
  `beltMotor_.resolve()` w tym samym ticku, nie sprzed niego.
- **Brak `requestStart()` w `main()`:** bez tego `Mode` pozostaje w `Idle`, silnik nie rusza, a paczka
  cały czas znajduje się w tej samej strefie.
- **Modyfikacja `describe()`:** informacje o trybie i silniku należy wypisać w osobnej linii w
  `main()`.

## Pytanie do zastanowienia

Przykładowy przebieg pokazuje, że pierwszy `step()` po `requestStart()` **nie** rusza paczki, a
dopiero drugi tak. Czy wynik byłby inny, gdyby program najpierw sprawdzał stan silnika, a dopiero
potem wywoływał `beltMotor_.resolve()`? Prześledź taki przebieg na kartce przed uruchomieniem testu.

## Koniec modułu 4

Przenośnik ma teraz dwa elementy wykonawcze z modelowanym opóźnieniem reakcji, a system jako całość
ma określony tryb pracy. W kolejnych modułach do `Mode` dojdą stany związane z bezpieczeństwem, w
tym zatrzymanie wywołane przyciskiem awaryjnym.
