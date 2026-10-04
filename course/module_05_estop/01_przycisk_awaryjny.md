🇵🇱 Polski | [🇬🇧 English](01_przycisk_awaryjny.en.md)

# 5.1 Przycisk awaryjny

## Problem

Przycisk awaryjny nie działa jak zwykły przełącznik włącz/wyłącz. **Zwolnienie przycisku nie może
automatycznie wznowić normalnej pracy.** Operator mógł zwolnić go przypadkowo albo zanim usunięto
przyczynę zatrzymania. System powinien więc czekać na osobne potwierdzenie możliwości wznowienia.

## Nowy element C++

**`enum class EStopLatchState { Released, Engaged, Armed }`** wraz z wolną funkcją
**`nextEStopLatchState(previous, pressed, released, resetRequested)`** tworzą niewielki automat
stanów. Podobne rozwiązanie zastosowaliśmy dla `Mode` w module 4.

## Reguła

- **`pressed` ma najwyższy priorytet.** Z dowolnego stanu powoduje przejście do `Engaged`.
- Z `Engaged`: dopiero `released` (przycisk fizycznie puszczony) przechodzi do `Armed`.
- Z `Armed`: dopiero jawne `resetRequested` powoduje powrót do `Released`. `Armed` oznacza, że
  przycisk został zwolniony, ale system nadal czeka na potwierdzenie. Dzięki temu `Mode` w misji 17
  nie wznowi pracy automatycznie.

**Przypadek brzegowy: `released` i `resetRequested` prawdziwe w tym samym wywołaniu.** Liczy się
wyłącznie `released`. Wynikiem jest `Armed`, a nie `Released`. Dla stanu `Engaged` jednoczesne
`released=true` i `resetRequested=true` daje więc `Armed`. Dopiero **osobne, kolejne** wywołanie z
`resetRequested=true`, gdy `previous` ma już wartość `Armed`, prowadzi do `Released`. System musi
najpierw zarejestrować zwolnienie przycisku, a dopiero później przyjąć potwierdzenie.

## Co już masz gotowe

[`include/psm/estop_latch.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-05-start/include/psm/estop_latch.hpp)
zawiera gotowy `enum class EStopLatchState` oraz deklarację `nextEStopLatchState`.

W pliku
[`src/estop_latch.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-05-start/src/estop_latch.cpp)
znajdziesz pusty szkielet funkcji z komentarzem `// TODO`.

## Co masz napisać

Uzupełnij ciało `nextEStopLatchState` zgodnie z regułą powyżej, uwzględniając przypadek brzegowy
jednoczesnych `released` i `resetRequested`.

## Sprawdź się

```bash
ctest --preset test -L misja-16
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test przechodzi przez pełny cykl
`Released` → `Engaged` → `Armed` → `Released`, sprawdza ignorowanie resetu w `Engaged`, jednoczesne
`released` i `resetRequested` oraz najwyższy priorytet `pressed`.

## Częste błędy

- **Sprawdzenie `resetRequested` przed `pressed`:** `pressed` musi mieć najwyższy priorytet,
  niezależnie od pozostałych sygnałów.
- **Reagowanie na `resetRequested` w stanie `Engaged`:** reset ma sens wyłącznie w `Armed`. W
  `Engaged` (przycisk wciąż wciśnięty) nie ma czego resetować.
- **Zwrócenie `Released` zamiast `Armed`** dla jednoczesnych `released` i `resetRequested`: reguła
  opisana wyżej wymaga `Armed`.

## Pytanie do zastanowienia

Dlaczego stan `Armed` w ogóle istnieje? Co konkretnie poszłoby nie tak, gdyby `Engaged` przechodziło
od razu z powrotem do `Released` w momencie puszczenia przycisku, bez pośredniego etapu?

**Dalej:** [Misja 17: tryb zatrzymania awaryjnego](./02_tryb_zatrzymania_awaryjnego.md).
