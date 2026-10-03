🇵🇱 Polski | [🇬🇧 English](03_integracja_w_cli.en.md)

# 9.3 Integracja w CLI

## Problem

Nic jeszcze nie pokazuje `Scenario`/`runScenario` naprawdę zastępujących ręcznie pisane sekwencje
bezpośrednie w działającym programie. Poza testem jednostkowym nic nie potwierdza, że ta
demonstracja jest prawdziwa i kompletna.

## Co już masz gotowe

[`apps/simulator_cli/main.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/apps/simulator_cli/main.cpp) jest w tym module **w całości
gotowy i niezmienny — to nie jest Twoja misja**. Buduje oba demonstracyjne scenariusze, uruchamia je
przez `runScenario`, wypisuje każdy tick przez `describe()`, i sam sprawdza wynik: jeśli scenariusz
odzyskiwania nigdy nie pokaże `Mode::Fault`, albo scenariusz wielu paczek da mniej niż trzy odjazdy,
program kończy się kodem innym niż `0`.

[`apps/simulator_cli/scenario_demos.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/apps/simulator_cli/scenario_demos.hpp) — deklaracje
`recoveryDemoScenario()`/`multiParcelDemoScenario()`, gotowe.

## Co masz napisać

Oba ciała w [`apps/simulator_cli/scenario_demos.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/apps/simulator_cli/scenario_demos.cpp):

### `recoveryDemoScenario()`

Odtwórz **ten sam mechanizm i oś czasu**, co ślad odzyskiwania po zablokowanym dywerterze z modułu 7
([`course/module_07_fault_mode/04_silnik_z_wykrywaniem_awarii.md`](../module_07_fault_mode/04_silnik_z_wykrywaniem_awarii.md))
— **nie** dosłownie identyczny, historyczny tekst CLI, bo to już niemożliwe: kształt `TickResult` i
format `describe()` zmieniły się w module 8 (osobne pole dla każdej strefy zamiast jednego
`item`/`zone`). To, co się **nie** zmieniło od modułu 7, to sam mechanizm — `Diverter`, `Mode`,
`BeltMotor`, `EStopLatch` —
więc te same numery ticków, na których pojawiają się `DiverterNotReady`/`RoutingDeadlineMissed`/
`Mode::Fault`/odzyskiwanie, są nadal odtwarzalne, choć zapisane w obecnym formacie
`describe()`.

Konkretnie: jedno przybycie (750g) na ticku 0, `StartRequested` na ticku 0, `ScriptedDiverterFault{Blocked}`
aktywny od ticku 0 do ticku 8 (odpowiednik `clearDiverterFault()` w oryginale), `Reset` na ticku 9,
`StartRequested` na ticku 10, `duration = 12`.

### `multiParcelDemoScenario()`

Trzy przybycia o różnych klasyfikacjach (np. 100g/800g/150g — Light/Heavy/Light), z odstępami między
tickami przybyć wynoszącymi co najmniej dwa, żeby żadne przybycie nie trafiło na wciąż zajęty `Infeed`
(paczka opuszcza `Infeed` dopiero, gdy pas realnie jedzie — rozpęd trwa jeden tick). To demonstruje
zdolność, której sama Misja 34 nie mogła jeszcze pokazać: wiele paczek zadeklarowanych z góry,
deklaratywnie, zamiast przez ręczną pętlę ponawiającą spawn co tick (por.
`tests/multiple_items_engine_test.cpp` z modułu 8).

## Sprawdź się

```bash
ctest --preset test -L misja-35
```

To prawdziwy test (`simulator_cli_scenario_smoke`), nie tylko stary `simulator_cli_smoke` (misja-6).
Obydwa uruchamiają to samo polecenie, ale sprawdzają inne wymagania: `simulator_cli_smoke`
zawsze sprawdzał tylko "program się uruchomił i zwrócił 0"; własna weryfikacja w `main.cpp` sprawia, że
ten sam kod wyjścia jest teraz naprawdę powiązany z tym, czy obie demonstracje faktycznie się udały.
Dlatego `misja-6`, mimo że historycznie osobny, w tym module dzieli wynik z `misja-35`, dopóki ta misja
nie jest gotowa.

Uruchom też program i porównaj wypisany ślad odzyskiwania z modułu 7:
```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

## Koniec modułu — pełny zestaw testów

```bash
ctest --preset test
```

Oczekiwany wynik: wszystkie testy zielone (`misja-1`, `misja-3`–`misja-4`, `misja-6`–`misja-35`).

## Zapisz swoją pracę

```bash
git status
git add <pliki które zmieniłeś>
git commit -m "..."
```

## Częste błędy

- **Próba dosłownego odtworzenia starego tekstu CLI z modułu 7** — niemożliwa i niepotrzebna; liczy
  się mechanizm i oś czasu, nie literalny string.
- **Zbyt małe odstępy między tickami przybyć w `multiParcelDemoScenario`** — przybycie zaplanowane
  zanim poprzednia paczka zdążyła opuścić `Infeed` sprawi, że `runScenario` zwróci `std::nullopt`.
- **Edytowanie `main.cpp`** — to gotowy szkielet programu dla całego modułu; Twoja praca jest
  wyłącznie w `scenario_demos.cpp`.

## Pytanie do zastanowienia

`main.cpp` sprawdza tylko dwie rzeczy: czy pojawił się `Mode::Fault` i czy nastąpiły co najmniej trzy
odjazdy. To minimalny warunek. Jaki błędny, ale wciąż „działający” (kod wyjścia `0`)
`recoveryDemoScenario()` przeszedłby tę weryfikację, mimo że w ogóle nie odtwarza mechanizmu z modułu 7?

## Koniec modułu 9

Symulator pozwala teraz prowadzić eksperyment na dwa sposoby: bezpośrednio, krok po kroku (od
modułu 1), i deklaratywny, jako jedna wartość `Scenario` do zapisania, przekazania i odtworzenia w
niezmienionej postaci. Żaden nie zastąpił drugiego; oba korzystają z tego samego publicznego
API `Engine`, które nigdy nie musiało się dowiedzieć, że taki wybór w ogóle istnieje.
