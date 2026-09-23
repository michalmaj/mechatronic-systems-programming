# 4.3 Przenośnik pod kontrolą trybu

## Problem

`BeltMotor` i `Mode` istnieją, ale nic ich jeszcze nie łączy z działającą symulacją — i nic jeszcze
nie przeszkadza paczce ruszać się na pasie, którego nikt nie poprosił o pracę.

## Nowe elementy C++

**`Engine::requestStart()` / `Engine::requestStop()`** — jawne operacje wejściowe, dokładnie tej samej
kategorii co `spawnItem` (niezmiennik z Modułu 3: to wejście, nie posuwanie symulacji naprzód, więc
wolno je wywoływać między tickami).

Nowe pola prywatne `Engine`: `mode_`, `beltMotor_`, oraz dwie flagi (`startRequested_`,
`stopRequested_`) zapisujące żądanie do skonsumowania w najbliższym `step()`.

## `Mode` a `BeltMotorState` — to dwa różne pytania

**`Mode`** to **intencja** — czego chce cały system, zmienia się natychmiast w tym samym ticku, w
którym `modeStep` obliczy nową wartość. **`BeltMotorState`** to **rzeczywisty stan fizycznego
urządzenia** — i celowo zostaje w tyle za intencją, bo na tym właśnie polega rampowanie.

Konsekwencja: `Mode::Idle` razem z `BeltMotorState::RampingDown`, oraz `Mode::Running` razem z
`BeltMotorState::RampingUp`, to **poprawne, oczekiwane, przejściowe** kombinacje — nie błędy, nie coś
do specjalnego traktowania. Zobaczysz je w weryfikacji tej misji.

## Rozszerzona kolejność `step()`

1. Skonsumuj i wyzeruj oczekujące żądania start/stop, policz `mode_ = modeStep(mode_, ...)`
   (reguła konfliktu z Misji 14 obowiązuje też tutaj).
2. `beltMotor_.setCommand(mode_ == Running ? Run : Stop)`, `beltMotor_.resolve()`.
3. Decyzja Controllera → `diverter_.setCommand` → `diverter_.resolve()` — bez zmian względem
   Modułu 3.
4. Wywołaj `psm::advance(plant_, diverter_)` **tylko jeśli** `beltMotor_.actualState() == Running` —
   w przeciwnym razie pomiń to wywołanie całkowicie; paczka po prostu nie rusza się w tym ticku.
5. Złóż większy teraz `TickResult`, zwiększ `tick_`.

## Przykładowy przebieg

Świeży `Engine`, paczka już dodana przez `spawnItem`. Wywołujesz `requestStart()`, potem dwa razy
`step()`:

- **Pierwszy `step()`**: `mode` staje się `Running` w tym samym wywołaniu, `beltActual` staje się
  `RampingUp` (pas był `Stopped`, teraz chce być `Running`) — **paczka się nie rusza** (brama nie
  przechodzi: jeszcze nie `Running`).
- **Drugi `step()`**: `mode` zostaje `Running`, `beltActual` staje się `Running` (jeszcze jeden krok
  `resolve()` od `RampingUp`) — brama teraz przechodzi, i paczka przesuwa się **dokładnie o jeden
  krok**.

Dokładnie ten przebieg sprawdza dostarczony test — Twoja implementacja musi go odtworzyć co do joty.

## Co już masz gotowe

`include/psm/engine.hpp` i `include/psm/tick_result.hpp` mają już wszystkie potrzebne pola i
deklaracje. `src/engine.cpp` ma puste szkielety `requestStart()`/`requestStop()`; ciało `step()` wciąż
wygląda dokładnie tak, jak zostawił je Moduł 3 — to Twoje zadanie, żeby je rozszerzyć.

## Co masz napisać

- `Engine::requestStart()` — ustaw `startRequested_` na `true`.
- `Engine::requestStop()` — ustaw `stopRequested_` na `true`.
- `Engine::step()` — rozszerz o pięć kroków opisanych wyżej.
- `apps/simulator_cli/main.cpp` — wywołaj `engine.requestStart()` przed pętlą (inaczej pas nigdy nie
  ruszy) i wypisuj obok istniejącej linii `describe()` również `mode`/`beltActual`.

**`describe()` pozostaje nietknięte** — jego kontrakt (tekst opisujący `tick`/`item`) się nie
zmienia; informacje o trybie/pasie wypisujesz osobno w `main()`.

## Sprawdź się

```bash
ctest --preset test -L misja-15
```

To prawdziwy, dedykowany test tej misji — nie fragment czegoś innego. Oczekiwany wynik:
`100% tests passed, 0 tests failed out of 1`.

Uruchom też program naprawdę:
```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

## Koniec modułu — pełny zestaw testów

```bash
ctest --preset test
```

Oczekiwany wynik: wszystkie testy zielone — `misja-1` do `misja-4`, `misja-6` do `misja-15`.

## Zapisz swoją pracę

```bash
git status
git add <pliki które zmieniłeś>
git commit -m "..."
```

## Częste błędy

- **Wywołanie `advance()` przed sprawdzeniem bramy** — brama musi sprawdzać stan pasa **po**
  `beltMotor_.resolve()` w tym samym ticku, nie sprzed niego.
- **Zapomniane `requestStart()` w `main()`** — bez tego `Mode` zostaje `Idle` na zawsze, pas nigdy nie
  rusza, a program wypisze same puste/nieruchome tiki.
- **Modyfikacja `describe()`** — nie ma takiej potrzeby; informacje o trybie/pasie idą do osobnej
  linii w `main()`.

## Pytanie do zastanowienia

Przykładowy przebieg pokazuje, że pierwszy `step()` po `requestStart()` **nie** rusza paczki, a dopiero
drugi tak. Gdyby ktoś zamienił kolejność kroków 2 i 4 (najpierw sprawdzić bramę, potem dopiero
wywołać `beltMotor_.resolve()`), czy przebieg wyglądałby inaczej? Spróbuj prześledzić to na kartce
przed uruchomieniem testu.

## Koniec modułu 4

Przenośnik ma teraz dwa aktuatory z realnym opóźnieniem fizycznym, a system jako całość ma pojęcie
trybu pracy. W kolejnych modułach `Mode` urośnie o stany związane z bezpieczeństwem — przycisk
awaryjny, którego celowo unikaliśmy w tym module, wreszcie znajdzie swoje miejsce.
