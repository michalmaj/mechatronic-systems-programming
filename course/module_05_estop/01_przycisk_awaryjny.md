# 5.1 Przycisk awaryjny

## Problem

Fizyczny przycisk awaryjny to nie jest zwykły przełącznik on/off. Ma subtelność, którą łatwo
przeoczyć: **puszczenie przycisku nie powinno cicho wznawiać normalnej pracy**. Operator, który
puszcza przycisk, mógł to zrobić przez przypadek albo dlatego, że problem wciąż nie jest rozwiązany —
system powinien czekać na jawne potwierdzenie, zanim uzna, że można wznowić.

## Nowy element C++

**`enum class EStopLatchState { Released, Engaged, Armed }`** + wolna funkcja
**`nextEStopLatchState(previous, pressed, released, resetRequested)`** — trzeci przykład wzorca
enum+funkcja w tym kursie (po `Mode` w Module 4).

## Reguła

- **`pressed` zawsze wygrywa** — z dowolnego stanu, natychmiast przechodzi do `Engaged`.
- Z `Engaged`: dopiero `released` (przycisk fizycznie puszczony) przechodzi do `Armed`.
- Z `Armed`: dopiero jawne `resetRequested` wraca do `Released`. `Armed` to stan "puszczony, ale
  wciąż czekam na potwierdzenie" — właśnie dlatego `Mode` (Misja 17) nigdy nie wznowi pracy
  automatycznie.

**Przypadek brzegowy: `released` i `resetRequested` prawdziwe w tym samym wywołaniu.** Liczy się
wyłącznie `released`. Wynikiem jest `Armed`, nie `Released` — reset nie może "przeskoczyć" stanu,
który dopiero co został zaobserwowany. Innymi słowy: z `Engaged`, `released=true` razem z
`resetRequested=true` daje `Armed`; dopiero **osobne, kolejne** wywołanie z `resetRequested=true`
(gdy `previous` jest już `Armed`) prowadzi do `Released`. To odzwierciedla fizyczną rzeczywistość: nie
można potwierdzić resetu przejścia, które jeszcze się nie zakończyło.

## Co już masz gotowe

[`include/psm/estop_latch.hpp`](../../include/psm/estop_latch.hpp) — `enum class EStopLatchState`
i deklaracja `nextEStopLatchState` już gotowe.

[`src/estop_latch.cpp`](../../src/estop_latch.cpp) — pusty szkielet z komentarzem `// TODO`.

## Co masz napisać

Uzupełnij ciało `nextEStopLatchState` zgodnie z regułą powyżej, uwzględniając przypadek brzegowy
`released`+`resetRequested`.

## Sprawdź się

```bash
ctest --preset test -L misja-16
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test przechodzi przez pełny cykl
`Released→Engaged→Armed→Released`, sprawdza ignorowanie resetu w `Engaged`, przypadek brzegowy
`released`+`resetRequested` naraz, oraz priorytet `pressed` nad wszystkim innym.

## Częste błędy

- **Sprawdzenie `resetRequested` przed `pressed`** — `pressed` musi wygrywać zawsze, niezależnie od
  tego, co jeszcze jest prawdziwe.
- **Reagowanie na `resetRequested` w stanie `Engaged`** — reset ma sens wyłącznie w `Armed`; w
  `Engaged` (przycisk wciąż wciśnięty) nie ma czego resetować.
- **Zwrócenie `Released` zamiast `Armed`** dla przypadku `released`+`resetRequested` naraz — to
  dokładnie przypadek opisany wyżej.

## Pytanie do zastanowienia

Dlaczego stan `Armed` w ogóle istnieje? Co konkretnie poszłoby nie tak, gdyby `Engaged` przechodziło
od razu z powrotem do `Released` w momencie puszczenia przycisku, bez pośredniego etapu?

**Dalej:** [Misja 17: tryb zatrzymania awaryjnego](./02_tryb_zatrzymania_awaryjnego.md).
