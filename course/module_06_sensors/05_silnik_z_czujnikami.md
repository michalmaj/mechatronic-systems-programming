# 6.5 Silnik z czujnikami

## Problem

Oba czujniki, `decideClassification` i `ControllerState` istnieją — ale nic w działającej symulacji
jeszcze z nich nie korzysta. `Engine` wciąż ufa `Item::mass` bezpośrednio.

## Nowe elementy C++

**`Engine::injectFault(FaultTarget, FaultKind)` / `clearFault(FaultTarget)`** — różnią się od
wszystkich dotychczasowych metod wejściowych (`requestStart`, `requestEStop`, ...): usterka jest
**trwała**. Nie znika po jednym ticku, tak jak flagi żądań z Modułów 4–5 — trwa, dopóki nie zostanie
jawnie wyczyszczona przez `clearFault`. To odzwierciedla rzeczywistość: usterka czujnika sama się nie
naprawia.

Nowe pola prywatne `presenceSensor_`, `weightSensor_`, `controllerState_`, `presenceFault_`,
`weightFault_` (te dwa ostatnie: `std::optional<FaultKind>`).

## Błąd, który ta misja naprawia: `nullopt` nie może cicho zamienić się w `HoldStraight`

To ważne, żeby zrozumieć wprost, zanim napiszesz kod. Naiwne podejście wyglądałoby tak: "jeśli
`controllerState_.classification` ma wartość, użyj jej; w przeciwnym razie zostaw domyślną komendę
dywertera (`HoldStraight`)." **To jest błąd.** `HoldStraight` to prawidłowa komenda — dywerter by się
ustawił, i paczka **pojechałaby dalej**, mimo że nigdy nie została naprawdę sklasyfikowana. Brak
wiarygodnej decyzji musi oznaczać **brak rutowania**, nie "rutuj w stronę domyślnej pozycji."

Dlatego `Plant::advance` (od tej misji) przyjmuje trzeci parametr:

```cpp
void advance(Plant& plant, const Diverter& diverter, bool routingReady = true);
```

Używany **wyłącznie** w gałęzi `Diverting` — reszta stref rusza się bez zmian. Gdy `routingReady`
jest fałszywe, paczka zostaje w `Diverting`, dokładnie tak samo, jakby dywerter nie był jeszcze
ustawiony.

## Rozszerzona kolejność `step()`

Kroki 1–4 (żądania, `latch_`, ścieżka awaryjna, `Mode`) zostają bez zmian względem Modułu 5. Nowe:

5. **Odczytaj oba czujniki, bezwarunkowo, co tick** — niezależnie od `Mode` czy override:
   `presenceSensor_.read(plant_.item, presenceFault_)`, `weightSensor_.read(plant_.item,
   weightFault_)`. Odczyt to obserwacja, nie posuwanie symulacji naprzód.
6. **Wywołaj `updateControllerState`** (Misja 23) z bieżącą paczką i świeżymi odczytami.
7. **Policz `routingReady`:**
   `!decision.overrideActive && diverterMayMove(mode_) && controllerState_.classification.has_value()`.
8. **Decyzja dywertera** (wciąż bramkowana przez `!decision.overrideActive && diverterMayMove(mode_)`,
   bez zmian) — ale teraz na podstawie `controllerState_.classification`, nie bezpośredniego
   `classify(item->mass)`.
9. **Wywołaj `psm::advance(plant_, diverter_, routingReady)`** — pierwsze w tym module wywołanie z
   trzema argumentami.
10. Złóż `TickResult` (teraz z polem `sensors`), zwiększ `tick_`.

## Zamrożona demonstracja w CLI

**`Stale` nie powoduje "innego rutowania" — pokazuje, że wiarygodnie wyglądająca liczba NIE jest
używana do nowej klasyfikacji.** Najbardziej przekonujący sposób, żeby to pokazać: przepuść **jedną**
paczkę normalnie (bez usterki), żeby czujnik wagi naprawdę zapamiętał prawdziwą masę. Potem wstrzyknij
`Stale` na czujniku wagi i wyślij **drugą** paczkę. Czujnik wagi będzie teraz powtarzał masę
**pierwszej** paczki (prawdziwą, wiarygodnie wyglądającą liczbę) — ale `decideClassification`
odrzuci ją mimo to, bo status to `Stale`, nie `Ok`. Druga paczka dotrze do `Diverting` i zostanie
tam zamrożona.

## Co już masz gotowe

`include/psm/engine.hpp` ma już wszystkie potrzebne pola i deklaracje. `src/engine.cpp` ma puste
szkielety `injectFault`/`clearFault`; ciało `step()` wciąż wygląda dokładnie tak, jak zostawił je
Moduł 5 — to Twoje zadanie, żeby je rozszerzyć.

## Co masz napisać

- `Engine::injectFault(FaultTarget, FaultKind)` — zapisz `kind` do `presenceFault_` albo
  `weightFault_`, zależnie od `target`.
- `Engine::clearFault(FaultTarget)` — wyczyść (`std::nullopt`) odpowiednie pole.
- `Engine::step()` — rozszerz o kroki 5–10 opisane wyżej.
- `apps/simulator_cli/main.cpp` — zaimplementuj zamrożoną demonstrację (dwie paczki, usterka między
  nimi) i wypisuj `sensors` obok istniejącego wyjścia.

## Self-check

```bash
ctest --preset test -L misja-24
```

To prawdziwy, dedykowany test tej misji. Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`.

Uruchom też program naprawdę:
```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

## Koniec modułu — pełny zestaw testów

```bash
ctest --preset test
```

Oczekiwany wynik: wszystkie testy zielone — `misja-1` do `misja-4`, `misja-6` do `misja-24`.

## Zapisz swoją pracę

```bash
git status
git add <pliki które zmieniłeś>
git commit -m "..."
```

## Częste błędy

- **Cicha zamiana `nullopt` na `HoldStraight`** — dokładnie błąd opisany wyżej. Sprawdź, czy
  `routingReady` naprawdę dociera do `psm::advance`.
- **Bramkowanie tylko przez `Mode`, bez `controllerState_.classification.has_value()`** —
  `routingReady` musi łączyć wszystkie trzy warunki naraz.
- **Aktualizowanie `ControllerState` tylko czasami** (np. tylko gdy `diverterMayMove` jest
  prawdziwe) — odczyt czujników i aktualizacja stanu muszą się dziać co tick, bezwarunkowo; to
  bramkowanie decyzji o ruchu jest osobne.

## Pytanie do zastanowienia

Ta misja wprowadza `routingReady` jako trzeci, generyczny parametr `Plant::advance` — nie
`WeightClass` ani `DiverterCommand`. Dlaczego to jest właściwy poziom szczegółowości dla tej
konkretnej granicy między `Plant` a `Controller`, skoro `Plant` od Modułu 1 nigdy nic nie wiedział o
klasyfikacji?

## Koniec modułu 6

System teraz naprawdę **nie ufa** swoim czujnikom bezwarunkowo — a Ty zbudowałeś mechanizm, który
łączy dwa niezależne, sekwencyjne potwierdzenia w jedną wiarygodną decyzję. W kolejnych modułach
pojawią się zdarzenia systemowe i usterki aktuatorów, które wreszcie dadzą `Mode::Fault` konkretny
powód, żeby istnieć.
