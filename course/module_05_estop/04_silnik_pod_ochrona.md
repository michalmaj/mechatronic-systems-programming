# 5.4 Silnik pod ochroną

## Problem

`EStopLatchState`, rozszerzony `Mode` i obie funkcje bezpieczeństwa istnieją — ale nic w działającej
symulacji jeszcze ich nie wywołuje.

## Nowe elementy C++

**`Engine::requestEStop()`, `releaseEStop()`, `requestReset()`** — trzy kolejne operacje wejściowe,
ta sama kategoria co `spawnItem`/`requestStart`/`requestStop`: to wejście, nie posuwanie symulacji
naprzód, więc wolno je wywoływać między tickami.

Nowe pole prywatne `latch_`.

## Dwie niezależne ścieżki, naprawdę niezależne

To jest miejsce, gdzie łatwo popełnić subtelny błąd architektoniczny — i warto go nazwać wprost, żeby
go uniknąć. **Nie wystarczy** obliczyć `Mode::EStopped` i potem bramkować dywerter/pas *wyłącznie
przez* `Mode`. Gdyby tak zrobić, ścieżka awaryjna zależałaby od poprawności `modeStep` — dokładnie tej
zależności ta reguła zabrania (przyszły błąd w kolejności sprawdzeń `modeStep` mógłby cicho
unieważnić działanie przycisku awaryjnego). Dlatego `Engine::step()` sprawdza
`checkEmergencyOverride(latch_)` **bezpośrednio**, i rozgałęzia się na tej podstawie jawnie: gdy
override jest aktywny, rutynowa logika pasa/dywertera **w ogóle się nie wykonuje** w tym ticku — nie
"wykonuje się i przypadkiem się zgadza", tylko jest pominięta, wzajemnie wykluczająca się z rutynową
ścieżką w tym samym `if`/`else`.

## Rozszerzona kolejność `step()`

1. Skonsumuj i wyzeruj oczekujące żądania (eStop/release/reset/start/stop), policz
   `latch_ = nextEStopLatchState(latch_, ...)`.
2. Policz `const SafetyDecision decision = checkEmergencyOverride(latch_);` — czytane bezpośrednio z
   `latch_`, nie wyprowadzane z `Mode`.
3. Policz `mode_ = modeStep(mode_, startRequested, stopRequested, latch_)` — `Mode` wciąż jest
   potrzebny (to on jest obserwowalny i steruje ścieżką *rutynową*), po prostu ścieżka awaryjna już
   mu nie ufa.
4. **Pas, jako `if`/`else`, nigdy oba naraz:** gdy `decision.overrideActive` jest prawdziwe, wywołaj
   wyłącznie `beltMotor_.forceStop()` i nic więcej. W przeciwnym razie zachowaj się dokładnie tak, jak
   w Module 4: `beltMotor_.setCommand(mode_ == Mode::Running ? BeltMotorCommand::Run :
   BeltMotorCommand::Stop)`, a potem `beltMotor_.resolve()`.
5. **Dywerter, bramkowany oboma sygnałami bezpośrednio, nie wyłącznie przez `Mode`:**
   `if (!decision.overrideActive && diverterMayMove(mode_))` — dopiero wtedy wykonuje się decyzja
   Controllera → `diverter_.setCommand` → `diverter_.resolve()`. W przeciwnym razie dywerter
   pozostaje **całkowicie nietknięty** w tym ticku, zamrożony tam, gdzie jest.
6. Brama na `psm::advance(plant_, diverter_)` według `beltMotor_.actualState() == Running`, bez
   zmian względem Modułu 4.
7. Złóż `TickResult` (teraz z polem `latch`), zwiększ `tick_`.

## Przykładowy przebieg

System pracuje (paczka się porusza, pas `Running`) → `requestEStop()` → **w tym samym ticku**:
`mode = EStopped`, `latch = Engaged`, `beltActual = Stopped` (natychmiast, przez `forceStop()`, bez
rampy, bez konkurującego wywołania rutynowego w tym ticku), paczka zamrożona w miejscu →
`releaseEStop()` → `latch = Armed`, **`mode` zostaje `EStopped`** (latch wciąż nie jest `Released`) →
`requestReset()` → `latch = Released`, `mode = Idle` (**nie** `Running`, zgodnie z regułą z
Misji 17) → wznowienie wymaga świeżego `requestStart()`.

## Co już masz gotowe

`include/psm/engine.hpp` ma już wszystkie potrzebne pola i deklaracje. `src/engine.cpp` ma puste
szkielety `requestEStop()`/`releaseEStop()`/`requestReset()`; ciało `step()` wciąż wygląda dokładnie
tak, jak zostawił je Moduł 4 — to Twoje zadanie, żeby je rozszerzyć.

## Co masz napisać

- `Engine::requestEStop()` — ustaw `eStopPressed_` na `true`.
- `Engine::releaseEStop()` — ustaw `eStopReleased_` na `true`.
- `Engine::requestReset()` — ustaw `resetRequested_` na `true`.
- `Engine::step()` — rozszerz o siedem kroków opisanych wyżej.
- `apps/simulator_cli/main.cpp` — zademonstruj przebieg (naciśnij, puść, zresetuj, wznów) i wypisuj
  `mode`/`latch` obok istniejącego wyjścia.

## Sprawdź się

```bash
ctest --preset test -L misja-19
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

Oczekiwany wynik: wszystkie testy zielone — `misja-1` do `misja-4`, `misja-6` do `misja-19`.

## Zapisz swoją pracę

```bash
git status
git add <pliki które zmieniłeś>
git commit -m "..."
```

## Częste błędy

- **Wywołanie rutynowej logiki pasa/dywertera nawet gdy `decision.overrideActive`** — to dokładnie
  luka w niezależności, którą ta misja naprawia. Sprawdź, czy Twój `if`/`else` naprawdę się
  wyklucza.
- **Bramkowanie dywertera wyłącznie przez `diverterMayMove(mode_)`**, bez `!decision.overrideActive`
  — to ponownie ta sama luka, tym razem po stronie dywertera.
- **Zapomniany `requestStart()` po odzyskaniu** — po `EStopped` system wraca do `Idle`, nie
  `Running`; bez świeżego żądania startu nic więcej się nie wydarzy.

## Pytanie do zastanowienia

Ta misja sprawdza `decision.overrideActive` osobno, zamiast ufać wyłącznie `Mode::EStopped`. Wymyśl
konkretny (hipotetyczny) błąd w `modeStep`, który sprawiłby, że poleganie wyłącznie na `Mode`
faktycznie zawiodłoby — a bezpośrednie sprawdzenie `checkEmergencyOverride` nadal by zadziałało.

## Koniec modułu 5

System ma teraz prawdziwie niezależną ścieżkę awaryjną, obok istniejącej ścieżki rutynowej sterowanej
przez `Mode`. W kolejnych modułach ta architektura będzie się rozwijać dalej — pojawią się czujniki,
usterki sprzętowe i zdarzenia systemowe, które dadzą `Mode::Fault` wreszcie konkretny powód do
istnienia.
