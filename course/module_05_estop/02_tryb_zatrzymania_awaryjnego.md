# 5.2 Tryb zatrzymania awaryjnego

To najważniejsza teoretycznie misja tego modułu — poświęcimy jej trochę więcej miejsca.

## Problem

`Mode` z Modułu 4 nic nie wie o istnieniu przycisku awaryjnego. Musi się dowiedzieć — i musi
przejmować się nim **bardziej** niż czymkolwiek innym, co dziś sprawdza.

## Nowe elementy C++

`modeStep` zyskuje nowy parametr i `Mode` zyskuje nową wartość:

```cpp
enum class Mode { Idle, Running, EStopped };

Mode modeStep(Mode current, bool startRequested, bool stopRequested,
              EStopLatchState latch = EStopLatchState::Released);
```

Zwróć uwagę na dwie rzeczy:

**Parametr `latch` jest ostatni, nie drugi.** Mógłby logicznie stać zaraz po `current`, ale
umieszczenie go na końcu, **z wartością domyślną**, ma konkretny cel: każde istniejące wywołanie
`modeStep` sprzed tego modułu (włącznie z testem Misji 14, `mode_test.cpp`, i z ciałem
`Engine::step()`) nadal się kompiluje, bez żadnej zmiany, korzystając z domyślnego `Released`. To
prawdziwy przykład na to, po co w ogóle istnieją wartości domyślne parametrów: pozwalają rozszerzyć
interfejs funkcji, nie psując nikogo, kto już go używa.

**`Mode::EStopped`** — trzecia wartość, obok `Idle` i `Running`.

## Bezwzględny priorytet

**E-stop sprawdzany jest jako pierwszy, przed czymkolwiek innym.** Jeśli
`latch != EStopLatchState::Released`, wynikiem jest `Mode::EStopped` — koniec, żadna inna reguła się
nie liczy w tym wywołaniu. Nieważne, czy `startRequested` czy `stopRequested` są prawdziwe — latch
wygrywa zawsze.

## Powrót zawsze przez `Idle`

**Odzyskanie po `EStopped` zawsze ląduje w `Idle`, nigdy automatycznie w `Running`.** W chwili, gdy
`latch` wreszcie znowu czyta `Released`, `Mode` staje się `Idle` — wznowienie pracy wymaga od
operatora jawnego, osobnego `requestStart()`, dokładnie tak samo jak przy starcie od zera.

**Przypadek brzegowy: `resetRequested` i `startRequested` prawdziwe w tym samym ticku.** Wynikiem
wciąż jest `Idle`, nigdy `Running`. Sprawdzenie "właśnie wróciłem z `EStopped`" ma pierwszeństwo przed
sprawdzeniem "`Idle` plus `startRequested`" — `Start` zażądany w tym samym ticku co reset, który
wyczyścił latch, **nie** zadziała w tym ticku. Operator zobaczy `Idle`, a dopiero **osobne**, kolejne
`requestStart()` faktycznie wznowi pracę.

## Co już masz gotowe

[`include/psm/mode.hpp`](../../include/psm/mode.hpp) — zaktualizowana deklaracja, jak wyżej.

[`src/mode.cpp`](../../src/mode.cpp) — logika `Idle`/`Running` z Modułu 4 zostaje **nietknięta i
działająca**; dwa nowe komentarze `// TODO` opisują, co dopisać, i gdzie dokładnie (przed resztą
reguł).

## Co masz napisać

Rozszerz ciało `modeStep` o dwa sprawdzenia, w tej kolejności, **przed** istniejącą logiką:
1. jeśli `latch != EStopLatchState::Released`, zwróć `Mode::EStopped`,
2. jeśli `current == Mode::EStopped` (a powyższy warunek nie zadziałał, czyli `latch` jest już
   `Released`), zwróć `Mode::Idle`.

## Sprawdź się

```bash
ctest --preset test -L misja-17
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test to nowy, dedykowany plik —
sprawdza priorytet e-stopu z różnych stanów, powrót do `Idle`, oraz przypadek brzegowy
`resetRequested`+`startRequested` naraz.

Warto też ponownie odpalić `ctest --preset test -L misja-14` — powinien nadal przechodzić, mimo że
nic w nim nie zmieniłeś.

## Częste błędy

- **Sprawdzenie `stopRequested`/`startRequested` przed sprawdzeniem `latch`** — e-stop musi wygrywać
  zawsze, bez wyjątków.
- **Zwrócenie `Mode::Running` zamiast `Mode::Idle`** przy powrocie z `EStopped`, gdy `startRequested`
  jest akurat prawdziwe w tym samym wywołaniu — to dokładnie ten przypadek konfliktu.
- **Umieszczenie nowych sprawdzeń na końcu funkcji** zamiast na początku — kolejność ma znaczenie,
  e-stop musi być sprawdzony pierwszy.

## Pytanie do zastanowienia

Gdyby `latch` był drugim parametrem `modeStep`, bez wartości domyślnej, co dokładnie musiałoby się
zmienić w tym module — i w ilu miejscach?

**Dalej:** [Misja 18: dwie niezależne ścieżki](./03_dwie_niezalezne_sciezki.md).
