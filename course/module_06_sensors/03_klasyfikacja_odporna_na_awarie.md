# 6.3 Klasyfikacja odporna na awarie

## Problem

`classify(mass)` (Moduł 1) wciąż bezwarunkowo ufa liczbie, którą dostaje. Teraz, gdy ta liczba
pochodzi z czujnika, który może zgłosić `Missing` albo `Stale`, potrzebujemy warstwy pośredniej,
która odrzuci niewiarygodny odczyt, zanim w ogóle trafi do `classify`.

## Nowy element C++

```cpp
std::optional<WeightClass> decideClassification(WeightReading weight);
```

Celowo tylko **jeden** czujnik na wejściu — nie próbujemy tu jeszcze łączyć odczytu wagi z
potwierdzeniem obecności (to problem Misji 23, i to znacznie trudniejszy). Ta misja uczy jednej,
wyizolowanej rzeczy: nie klasyfikuj na podstawie odczytu, któremu nie możesz ufać.

## Reguła

- Jeśli `weight.status != ReadingStatus::Ok`: zwróć `std::nullopt` — niezależnie od tego, jak
  sensowna wygląda liczba w `weight.grams`.
- W przeciwnym razie: zwróć `classify(weight.grams)`.

## Co już masz gotowe

[`include/psm/controller.hpp`](../../include/psm/controller.hpp) — deklaracja `decideClassification`
już dodana, obok istniejących `classify`/`toDiverterCommand` (te dwie zostają bez zmian).

[`src/controller.cpp`](../../src/controller.cpp) — pusty szkielet `decideClassification` z
komentarzem `// TODO`; `classify`/`toDiverterCommand` już działają poprawnie.

## Co masz napisać

Uzupełnij ciało `decideClassification` zgodnie z regułą powyżej.

## Self-check

```bash
ctest --preset test -L misja-22
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test celowo zawiera przypadek
`{Stale, 750}` — liczba wygląda całkowicie prawdopodobnie, ale status mówi wyraźnie: nie ufaj temu.

## Częste błędy

- **Sprawdzanie `weight.grams` zamiast `weight.status`** jako sygnału do odrzucenia — to odwrotność
  całego sensu tej funkcji.

## Pytanie do zastanowienia

Dlaczego ta misja nie przyjmuje jeszcze `PresenceReading` jako drugiego argumentu, skoro realny
problem (Misja 23) będzie potrzebował obu czujników? Co konkretnie poszłoby nie tak, gdybyśmy
spróbowali połączyć oba czujniki w tej samej funkcji, wywoływanej w jednym ticku?

**Dalej:** [Misja 23: pamięć decyzji sterownika](./04_pamiec_decyzji_sterownika.md).
