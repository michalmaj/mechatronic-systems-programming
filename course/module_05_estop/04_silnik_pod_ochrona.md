🇵🇱 Polski | [🇬🇧 English](04_silnik_pod_ochrona.en.md)

# 5.4 Obsługa zatrzymania awaryjnego w `Engine`

## Problem

`EStopLatchState`, rozszerzony `Mode` i obie funkcje bezpieczeństwa są już gotowe, ale działająca
symulacja jeszcze z nich nie korzysta.

## Nowe elementy C++

**`Engine::requestEStop()`, `releaseEStop()` i `requestReset()`** zapisują kolejne żądania wejściowe.
Podobnie jak `spawnItem`, `requestStart` i `requestStop`, można je wywoływać między tickami. Same nie
wykonują kroku symulacji.

Nowe pole prywatne `latch_`.

## Dwie niezależne ścieżki

**Nie wystarczy** obliczyć `Mode::EStopped` i uzależnić ruch dywertera oraz taśmy wyłącznie od `Mode`.
W takim rozwiązaniu ścieżka awaryjna zależałaby od poprawności `modeStep`, a błąd w kolejności
warunków mógłby ją wyłączyć. Dlatego `Engine::step()` sprawdza
`checkEmergencyOverride(latch_)` **bezpośrednio**. Gdy `overrideActive` ma wartość `true`, zwykła
logika silnika i dywertera nie wykonuje się w tym ticku. Obie ścieżki są rozdzielone za pomocą
`if`/`else`, więc nie mogą wykonać się jednocześnie.

## Rozszerzona kolejność `step()`

1. Odczytaj oczekujące żądania zatrzymania awaryjnego, zwolnienia, resetu, uruchomienia i
   zatrzymania. Oblicz `latch_ = nextEStopLatchState(latch_, ...)`, a następnie wyzeruj odpowiednie
   flagi.
2. Oblicz `const SafetyDecision decision = checkEmergencyOverride(latch_);` bezpośrednio na
   podstawie `latch_`, a nie `Mode`.
3. Oblicz `mode_ = modeStep(mode_, startRequested, stopRequested, latch_)`. `Mode` nadal opisuje
   stan całego systemu i steruje zwykłą ścieżką pracy, ale ścieżka awaryjna nie zależy wyłącznie od
   jego wartości.
4. Gdy `decision.overrideActive` ma wartość `true`, wywołaj tylko `beltMotor_.forceStop()`. W
   przeciwnym razie wykonaj logikę z modułu 4:
   `beltMotor_.setCommand(mode_ == Mode::Running ? BeltMotorCommand::Run :
   BeltMotorCommand::Stop)`, a potem `beltMotor_.resolve()`.
5. Sprawdź `if (!decision.overrideActive && diverterMayMove(mode_))`. Tylko wtedy wykonaj decyzję
   sterownika, `diverter_.setCommand` i `diverter_.resolve()`. W przeciwnym razie stan dywertera nie
   zmienia się w tym ticku.
6. Wywołaj `psm::advance(plant_, diverter_)` tylko wtedy, gdy
   `beltMotor_.actualState() == Running`, tak jak w module 4.
7. Utwórz `TickResult`, tym razem również z polem `latch`, a następnie zwiększ `tick_`.

## Przykładowy przebieg

System pracuje, silnik ma stan `Running`, a paczka się porusza. Po wywołaniu `requestEStop()` w tym
samym ticku `mode` zmienia się na `EStopped`, `latch` na `Engaged`, a `beltActual` na `Stopped`.
`forceStop()` zmienia stan modelu bez przejścia przez `RampingDown`, a paczka pozostaje w miejscu.
Po `releaseEStop()` wartość `latch` zmienia się na `Armed`, natomiast `mode` pozostaje w `EStopped`.
Po `requestReset()` otrzymujemy `latch = Released` i `mode = Idle`, a nie `Running`. Wznowienie pracy
wymaga osobnego `requestStart()`.

## Co już masz gotowe

`include/psm/engine.hpp` ma już wszystkie potrzebne pola i deklaracje. W `src/engine.cpp` znajdziesz
puste szkielety `requestEStop()`, `releaseEStop()` i `requestReset()`. Ciało `step()` nadal odpowiada
wersji z modułu 4, którą teraz rozszerzysz.

## Co masz napisać

- `Engine::requestEStop()`: ustaw `eStopPressed_` na `true`.
- `Engine::releaseEStop()`: ustaw `eStopReleased_` na `true`.
- `Engine::requestReset()`: ustaw `resetRequested_` na `true`.
- `Engine::step()`: dodaj siedem opisanych wyżej kroków.
- `apps/simulator_cli/main.cpp`: pokaż zatrzymanie awaryjne, zwolnienie przycisku, reset i ponowne
  uruchomienie. Oprócz dotychczasowego wyniku wypisuj `mode` oraz `latch`.

## Sprawdź się

```bash
ctest --preset test -L misja-19
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
`misja-19`.

## Zapisz swoją pracę

```bash
git status
git add <pliki które zmieniłeś>
git commit -m "..."
```

## Częste błędy

- **Wywołanie zwykłej logiki silnika lub dywertera, gdy `decision.overrideActive` ma wartość
  `true`:** narusza niezależność ścieżki awaryjnej. Sprawdź, czy gałęzie `if` i `else` wzajemnie się
  wykluczają.
- **Sprawdzenie dywertera wyłącznie przez `diverterMayMove(mode_)`**, bez
  `!decision.overrideActive`: pomija bezpośrednią informację ze ścieżki awaryjnej.
- **Brak `requestStart()` po resecie:** po `EStopped` system wraca do `Idle`, a nie `Running`.
  Ponowne uruchomienie wymaga nowego żądania.

## Pytanie do zastanowienia

Ta misja sprawdza `decision.overrideActive` osobno, zamiast ufać wyłącznie `Mode::EStopped`. Wymyśl
konkretny błąd w `modeStep`, który sprawiłby, że poleganie wyłącznie na `Mode`
nie zatrzymałoby urządzeń, ale bezpośrednie sprawdzenie `checkEmergencyOverride` nadal by zadziałało.

## Koniec modułu 5

Model ma teraz niezależną ścieżkę awaryjną obok zwykłej ścieżki sterowanej przez `Mode`. W kolejnych
modułach dojdą czujniki, usterki sprzętowe i zdarzenia systemowe. Na ich podstawie system będzie
mógł przechodzić do `Mode::Fault`.
