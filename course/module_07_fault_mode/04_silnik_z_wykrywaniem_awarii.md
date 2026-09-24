🇵🇱 Polski | [🇬🇧 English](04_silnik_z_wykrywaniem_awarii.en.md)

# 7.4 Silnik z wykrywaniem awarii

## Problem

Wszystkie kawałki istnieją osobno — blokowalny dywerter, licznik terminu w `Plant`, dwuetapowe
liczenie `Mode` — ale nic w działającym silniku jeszcze ich nie łączy.

## Nowe elementy C++

**Rozdzielone API usterek.** `SensorFaultKind`/`SensorTarget` to Moduł 6 pod nowymi, jaśniejszymi
nazwami (`injectFault`/`clearFault` też się przemianowały na `injectSensorFault`/`clearSensorFault`)
— to już gotowe, bez Twojej pracy. Nowość to:

```cpp
void injectDiverterFault(DiverterFaultKind kind);
void clearDiverterFault();
```

Bez parametru celu — jest tylko jeden dywerter, więc nie ma czego wybierać.

## Rozszerzenie `step()` — sedno tej misji

`step()` z Modułu 6 pozostaje w niemal niezmienionym kształcie (flagi wejściowe, `latch_`, `decision`,
czujniki i `ControllerState` — bez zmian) — zmieniają się cztery rzeczy, w tej kolejności:

1. **Liczenie `Mode` przenosi się na sam początek `step()`**, przed jakąkolwiek próbą rutowania w tym
   ticku. Policz `modeForTick` dokładnie tym samym wywołaniem `modeStep(...)` co dotąd i zapamiętaj
   wynik w zmiennej lokalnej — **nie** przypisuj go jeszcze do `mode_`.
2. **Bramkowania aktuatorów (dywerter, pas) używają `modeForTick`, nie `mode_`** — pole `mode_` w tym
   ticku wciąż ma starą wartość, aż do punktu 4 poniżej. Bramka dywertera dodatkowo przekazuje
   `diverterFault_` do `diverter_.resolve(...)`, tak jak czujniki dostają swoje usterki w
   Module 6.
3. **`psm::advance(...)` woła się pod tą samą bramką co w Module 6** (pas faktycznie `Running`), ale
   jego wynik trzeba teraz zapamiętać — `advance()` zwraca informację, czy w tym ticku wystąpił
   `SystemEventKind` (np. przekroczenie terminu rutowania).
4. **Drugie liczenie `Mode` dzieje się dopiero po `advance()`**, nie przed nim: to jedyne miejsce,
   gdzie `mode_` dostaje nową wartość w tym ticku, przez `reactToSystemEvent(modeForTick, event)` —
   `modeForTick` z punktu 1, `event` z punktu 3.

`TickResult` tego ticku dostaje dodatkowo pole `event`.

## Jednotickowe opóźnienie zatrzymania pasa — zaakceptowane, nie naprawiane

Tick, który wykrywa `RoutingDeadlineMissed`, bramkuje swoje aktuatory wartością `modeForTick ==
Running` (to była wartość znana na początku tego ticku — próba, która ujawniła przekroczenie
terminu, sama musiała zostać wykonana pod bramkowaniem `Running`) i dopiero na samym końcu zgłasza
`mode = Fault`. Efekt: `TickResult` tego ticku może pokazywać `mode = Fault`, a `beltActual` wciąż
`Running`. Pas zaczyna faktycznie zwalniać (`RampingDown`) dopiero w **następnym** ticku, gdy
`modeForTick` dla tego kolejnego ticku jest już `Fault`.

To celowo inaczej niż e-stop, który wymusza `beltMotor_.forceStop()` natychmiast, przez
`decision.overrideActive` — liczone niezależnie od `Mode`, właśnie po to, żeby zadziałać w tym samym
ticku, w którym wykryto naciśnięcie. `Fault` nie ma i nie dostaje żadnej takiej wymuszonej ścieżki w
tym module: to stan kontrolowany, związany z rutowaniem, nie stan bezpieczeństwa krytycznego. Jednotickowe
opóźnienie jest poprawnym, zaakceptowanym zachowaniem, nie luką.

## Przykładowy scenariusz odzyskiwania

```text
krok 1: item wchodzi do Infeed, mode=Running, belt=RampingUp — paczka jeszcze czeka, pas dopiero się rozpędza.
krok 2: item PresenceCheck, belt=Running — dopiero teraz pas realnie jedzie, paczka rusza.
krok 3: item Weighing.
krok 4: item Diverting, klasyfikacja już w ControllerState.
krok 5: event=DiverterNotReady — pierwsza aktywna próba, dywerter Blocked.
krok 6: event=RoutingDeadlineMissed, mode=Fault, belt wciąż Running — ten sam tick.
krok 7: mode=Fault, belt=RampingDown — dopiero teraz pas zaczyna zwalniać.
krok 8: mode=Fault, belt=Stopped.
— clearDiverterFault() —
krok 9: mode=Fault — samo wyczyszczenie usterki nie wystarcza.
— requestReset() —
krok 10: mode=Idle — zatrzask ustępuje dopiero teraz. Paczka wciąż w Diverting.
— requestStart() —
krok 11: mode=Running, belt=RampingUp — paczka wciąż czeka, pas znów musi się rozpędzić od zera.
krok 12: belt=Running, event=brak — dywerter (usterka już wyczyszczona, w ruchu od poprzedniego ticku) zdążył się ustawić, zanim advance() w ogóle zdążył sprawdzić — prosto do rutowania, bez żadnego DiverterNotReady po drodze.
```

Zwróć uwagę na krok 12: w tej konkretnej sekwencji odzyskiwanie **nie** przechodzi przez
`DiverterNotReady` wcale — bo pas też musiał się rozpędzić od zera (był w pełni zatrzymany podczas
`Fault`), więc `advance()` nie sprawdza dywertera aż do kroku 12, a dywerter miał już dwa tiki, żeby
się ustawić (kroki 11 i 12). To nie przypadek: i pas, i dywerter potrzebują dokładnie dwóch wywołań
`resolve()`, żeby dojść do celu ze stanu spoczynkowego.

Gdyby `clearDiverterFault()` nie zostało wywołane, kroki 11–12 powtórzyłyby dokładnie sekwencję z
kroków 5–6: znowu `DiverterNotReady`, potem znowu `RoutingDeadlineMissed` i `Fault`. To pokazuje
wprost, że `requestReset()` sam w sobie czyści tylko **objaw** (zatrzask trybu), nigdy **przyczynę**.

## Co już masz gotowe

`include/psm/engine.hpp` ma już wszystkie potrzebne pola i deklaracje. `src/engine.cpp` ma gotowe,
przemianowane `injectSensorFault`/`clearSensorFault`; puste szkielety `injectDiverterFault`/
`clearDiverterFault`; ciało `step()` wciąż wygląda dokładnie tak, jak zostawił je Moduł 6 — to Twoje
zadanie, żeby je rozszerzyć.

## Co masz napisać

- `Engine::injectDiverterFault(DiverterFaultKind)` — zapisz `kind` do `diverterFault_`.
- `Engine::clearDiverterFault()` — wyczyść (`std::nullopt`) `diverterFault_`.
- `Engine::step()` — rozszerz zgodnie z opisem powyżej: `modeForTick` do bramkowania aktuatorów,
  `diverterFault_` przekazywane do `diverter_.resolve()`, `reactToSystemEvent` na końcu ustawiające
  `mode_`, `event` w `TickResult`.
- `apps/simulator_cli/main.cpp` — zaimplementuj scenariusz odzyskiwania opisany powyżej.

## Sprawdź się

```bash
ctest --preset test -L misja-28
```

To prawdziwy, dedykowany test tej misji, śledzący scenariusz odzyskiwania powyżej krok po
kroku. Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`.

Uruchom też program naprawdę:
```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

## Koniec modułu — pełny zestaw testów

```bash
ctest --preset test
```

Oczekiwany wynik: wszystkie testy zielone — `misja-1` do `misja-4`, `misja-6` do `misja-28`.

## Zapisz swoją pracę

```bash
git status
git add <pliki które zmieniłeś>
git commit -m "..."
```

## Częste błędy

- **Bramkowanie aktuatorów przez `mode_` zamiast `modeForTick`** — w tym ticku `mode_` jeszcze nie
  ma nowej wartości; ona pojawia się dopiero po `reactToSystemEvent`.
- **Wywołanie `reactToSystemEvent` przed `Plant::advance()`** — wtedy `event` nie byłoby jeszcze
  znane. Kolejność w kroku 7 i 8 jest tu istotna.
- **Dodanie wymuszonego zatrzymania pasa dla `Fault`** — to jest dokładnie ta rzecz, której ten
  moduł świadomie nie robi (patrz sekcja o jednotickowym opóźnieniu wyżej).

## Pytanie do zastanowienia

Scenariusz odzyskiwania powyżej pokazuje, że pas i dywerter "przypadkiem" kończą rozpędzanie się w
tym samym kroku. Zmodyfikuj w głowie (albo na boku, eksperymentalnie) czas rozpędzania pasa, żeby był
o jeden tick dłuższy niż settle-time dywertera. Co dokładnie zobaczyłby `TickResult` w tym
hipotetycznym scenariuszu na kroku, w którym dywerter jest już ustawiony, ale pas jeszcze nie jest
`Running`?

## Koniec modułu 7

`Mode::Fault` ma teraz prawdziwy, przetestowany wyzwalacz — a Ty zbudowałeś mechanizm, który
rozróżnia dwie naprawdę różne odpowiedzialności (wejścia operatora kontra reakcja na zdarzenie
systemowe) jako dwie osobne, nazwane funkcje, zamiast maskować je pod jedną. To ten sam duch, co
dwie niezależne ścieżki bezpieczeństwa z Modułu 5 — tym razem zastosowany do dwóch różnych momentów
w czasie jednego ticku, nie do dwóch różnych aktuatorów.
