🇵🇱 Polski | [🇬🇧 English](03_integracja_w_cli.en.md)

# 9.3 Scenariusze w programie terminalowym

## Problem

`Scenario` i `runScenario()` działają już w testach, ale program nie pokazuje jeszcze praktycznego
zastosowania tego rozwiązania. Przygotujesz dwa przykłady, które zastąpią ręcznie zapisane sekwencje
wywołań.

## Co już masz gotowe

Plik
[`apps/simulator_cli/main.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/apps/simulator_cli/main.cpp)
jest kompletny i nie należy go edytować. Program buduje dwa scenariusze, uruchamia je za pomocą
`runScenario()` i wypisuje każdy tick przez `describe()`. Sprawdza też podstawowe wyniki. Jeśli
scenariusz awarii nie osiągnie `Mode::Fault` albo scenariusz wielu paczek zawiera mniej niż trzy
odjazdy, program kończy się kodem innym niż `0`.

W pliku
[`apps/simulator_cli/scenario_demos.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/apps/simulator_cli/scenario_demos.hpp)
znajdziesz gotowe deklaracje `recoveryDemoScenario()` i `multiParcelDemoScenario()`.

## Co masz napisać

Uzupełnij obie funkcje w
[`apps/simulator_cli/scenario_demos.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/apps/simulator_cli/scenario_demos.cpp).

### `recoveryDemoScenario()`

Odtwórz mechanizm i kolejność zdarzeń ze scenariusza zablokowanego dywertera z modułu 7. Jego opis
znajdziesz w
[`course/module_07_fault_mode/04_silnik_z_wykrywaniem_awarii.md`](../module_07_fault_mode/04_silnik_z_wykrywaniem_awarii.md).

Nie próbuj odtwarzać dokładnego tekstu wypisywanego w module 7. W module 8 zmieniły się `TickResult`
i format `describe()`. Zamiast pojedynczych pól `item` i `zone` wynik zawiera teraz osobne pola dla
każdej strefy. Bez zmian pozostały jednak `Diverter`, `Mode`, `BeltMotor` i `EStopLatch`. Zdarzenia
`DiverterNotReady` i `RoutingDeadlineMissed`, przejście do `Mode::Fault` oraz powrót do pracy mogą
więc wystąpić w tych samych tickach.

Utwórz scenariusz o następujących danych:

- paczka o masie 750 g przybywa w ticku 0,
- `StartRequested` występuje w ticku 0,
- usterka `ScriptedDiverterFault{Blocked}` jest aktywna od ticku 0 do ticku 8, bez ticku 8,
- `Reset` występuje w ticku 9,
- kolejne `StartRequested` występuje w ticku 10,
- `duration = 12`.

Koniec przedziału usterki odpowiada wywołaniu `clearDiverterFault()` w pierwotnym przykładzie.

### `multiParcelDemoScenario()`

Zaplanuj przybycie trzech paczek należących do różnych klas, na przykład o masach 100 g, 800 g i
150 g. Powinny trafić kolejno do wyjść Light, Heavy i Light.

Między tickami przybycia zachowaj odstęp co najmniej dwóch ticków. Paczka opuszcza `infeed` dopiero
po rozpędzeniu taśmy, które trwa jeden tick. Mniejszy odstęp mógłby spowodować próbę dodania kolejnej
paczki do zajętej strefy.

Ten przykład pokazuje kilka przybyć zapisanych z góry w `Scenario`. Nie wymaga ręcznej pętli, która
ponawia `spawnItem()` w każdym ticku, jak w `tests/multiple_items_engine_test.cpp` z modułu 8.

## Sprawdź się

```bash
ctest --preset test -L misja-35
```

Test `simulator_cli_scenario_smoke` uruchamia to samo polecenie co wcześniejszy
`simulator_cli_smoke` z misji 6, ale sprawdza nowe wymagania. Pierwszy test historycznie potwierdzał
tylko, że program uruchamia się i zwraca kod `0`. Teraz kod zakończenia zależy także od wewnętrznych
sprawdzeń obu demonstracji wykonywanych w `main.cpp`. Do ukończenia tej misji testy `misja-6` i
`misja-35` mogą więc zgłaszać ten sam błąd.

Zbuduj i uruchom program, a następnie porównaj przebieg awarii z opisem w module 7:

```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

## Koniec modułu: pełny zestaw testów

```bash
ctest --preset test
```

Oczekiwany wynik: przechodzą testy `misja-1`, `misja-3`–`misja-4` oraz `misja-6`–`misja-35`.

## Zapisz swoją pracę

```bash
git status
git add <pliki które zmieniłeś>
git commit -m "..."
```

## Częste błędy

- **Próba odtworzenia dokładnego tekstu z programu w module 7**: format wyniku zmienił się w module
  8. Ważne są mechanizm i kolejność zdarzeń, a nie identyczny napis.
- **Zbyt małe odstępy między przybyciami w `multiParcelDemoScenario()`**: jeśli poprzednia paczka nie
  opuściła jeszcze `infeed`, `runScenario()` zwróci `std::nullopt`.
- **Edycja `main.cpp`**: ten plik jest gotowy. Zmiany należy wprowadzić wyłącznie w
  `scenario_demos.cpp`.

## Pytanie do zastanowienia

`main.cpp` sprawdza tylko, czy wystąpił `Mode::Fault` i czy odjechały co najmniej trzy paczki. Podaj
przykład błędnego `recoveryDemoScenario()`, który spełniłby te minimalne warunki i zakończył program
kodem `0`, choć nie odtwarzałby mechanizmu z modułu 7.

## Koniec modułu 9

Symulacją można teraz sterować na dwa sposoby. Pierwszy polega na bezpośrednim wywoływaniu kolejnych
metod `Engine`. Drugi zapisuje cały eksperyment w `Scenario`, które można przechować, przekazać i
ponownie uruchomić. Oba sposoby korzystają z tego samego publicznego interfejsu `Engine`, który nie
wymagał żadnych zmian.
