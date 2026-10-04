🇵🇱 Polski | [🇬🇧 English](02_tryb_zatrzymania_awaryjnego.en.md)

# 5.2 Tryb zatrzymania awaryjnego

Ta misja wprowadza najważniejszą zasadę tego modułu, dlatego jej opis jest nieco dłuższy.

## Problem

`Mode` z modułu 4 nie uwzględnia jeszcze przycisku awaryjnego. Jego stan musi mieć pierwszeństwo
przed wszystkimi dotychczasowymi żądaniami.

## Nowe elementy C++

`modeStep` zyskuje nowy parametr i `Mode` zyskuje nową wartość:

```cpp
enum class Mode { Idle, Running, EStopped };

Mode modeStep(Mode current, bool startRequested, bool stopRequested,
              EStopLatchState latch = EStopLatchState::Released);
```

Zwróć uwagę na dwie rzeczy:

**Parametr `latch` jest ostatni, nie drugi.** Mógłby logicznie stać zaraz po `current`, ale
umieszczenie go na końcu, **z wartością domyślną**, ma konkretny cel. Istniejące wywołania
`modeStep`, w tym test misji 14 i kod w `Engine::step()`, nadal się kompilują bez zmian, korzystając
z domyślnego `Released`. Wartość domyślna pozwala więc rozszerzyć interfejs funkcji bez poprawiania
każdego miejsca, które już jej używa.

**`Mode::EStopped`** jest trzecią wartością, obok `Idle` i `Running`.

## Bezwzględny priorytet

**Stan przycisku awaryjnego jest sprawdzany jako pierwszy.** Jeśli
`latch != EStopLatchState::Released`, wynikiem jest `Mode::EStopped` i żadna inna reguła nie jest
już sprawdzana w tym wywołaniu. Wartości `startRequested` i `stopRequested` nie mają wtedy znaczenia.

## Powrót zawsze przez `Idle`

**Po wyjściu z `EStopped` system zawsze przechodzi do `Idle`, nigdy automatycznie do `Running`.** Gdy
`latch` ponownie ma wartość `Released`, `Mode` zmienia się na `Idle`. Wznowienie pracy wymaga od
operatora osobnego `requestStart()`, tak samo jak przy pierwszym uruchomieniu.

**Przypadek brzegowy: `resetRequested` i `startRequested` prawdziwe w tym samym ticku.** Wynikiem
wciąż jest `Idle`, nigdy `Running`. Warunek powrotu z `EStopped` ma pierwszeństwo przed sprawdzeniem
`startRequested` dla trybu `Idle`. Żądanie uruchomienia zgłoszone w tym samym ticku co reset **nie**
zostanie wykonane. Operator zobaczy `Idle`, a pracę wznowi dopiero osobne, późniejsze
`requestStart()`.

## Co już masz gotowe

[`include/psm/mode.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-05-start/include/psm/mode.hpp)
zawiera zaktualizowaną deklarację pokazaną wyżej.

W pliku
[`src/mode.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-05-start/src/mode.cpp)
pozostaje działająca logika `Idle` i `Running` z modułu 4. Dwa nowe komentarze `// TODO` wskazują,
co należy dopisać przed dotychczasowymi regułami.

## Co masz napisać

Rozszerz ciało `modeStep` o dwa sprawdzenia, w tej kolejności, **przed** istniejącą logiką:
1. jeśli `latch != EStopLatchState::Released`, zwróć `Mode::EStopped`,
2. jeśli `current == Mode::EStopped` (a powyższy warunek nie zadziałał, czyli `latch` jest już
   `Released`), zwróć `Mode::Idle`.

## Sprawdź się

```bash
ctest --preset test -L misja-17
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza priorytet zatrzymania
awaryjnego w różnych stanach, powrót do `Idle` oraz jednoczesne `resetRequested` i `startRequested`.

Warto też ponownie uruchomić `ctest --preset test -L misja-14`. Powinien nadal przechodzić, mimo że
nic w nim nie zmieniłeś.

## Częste błędy

- **Sprawdzenie `stopRequested` lub `startRequested` przed sprawdzeniem `latch`:** zatrzymanie
  awaryjne ma zawsze pierwszeństwo.
- **Zwrócenie `Mode::Running` zamiast `Mode::Idle`** przy powrocie z `EStopped`, gdy `startRequested`
  jest prawdziwe w tym samym wywołaniu: reguła wymaga powrotu do `Idle`.
- **Umieszczenie nowych sprawdzeń na końcu funkcji** zamiast na początku: kolejność ma znaczenie.
  Stan przycisku awaryjnego trzeba sprawdzić jako pierwszy.

## Pytanie do zastanowienia

Gdyby `latch` był drugim parametrem `modeStep` i nie miał wartości domyślnej, które istniejące
wywołania tej funkcji należałoby zmienić?

**Dalej:** [Misja 18: dwie niezależne ścieżki](./03_dwie_niezalezne_sciezki.md).
