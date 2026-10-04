🇵🇱 Polski | [🇬🇧 English](03_klasyfikacja_odporna_na_awarie.en.md)

# 6.3 Klasyfikacja odporna na awarie

## Problem

`classify(mass)` z modułu 1 korzysta z każdej przekazanej wartości. Odczyt czujnika może jednak mieć
status `Missing` albo `Stale`. Potrzebujemy więc funkcji, która odrzuci taki odczyt, zanim jego
wartość trafi do `classify`.

## Nowy element C++

```cpp
std::optional<WeightClass> decideClassification(WeightReading weight);
```

Funkcja przyjmuje na razie odczyt tylko **jednego** czujnika. W misji 23 połączysz pomiar wagi z
potwierdzeniem obecności. Tutaj obowiązuje jedna zasada: nie klasyfikuj paczki na podstawie
niewiarygodnego odczytu.

## Reguła

- Jeśli `weight.status != ReadingStatus::Ok`, zwróć `std::nullopt`, niezależnie od wartości
  `weight.grams`.
- W przeciwnym razie: zwróć `classify(weight.grams)`.

## Co już masz gotowe

[`include/psm/controller.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/include/psm/controller.hpp)
zawiera deklarację `decideClassification` obok niezmienionych `classify` i `toDiverterCommand`.

W pliku
[`src/controller.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/src/controller.cpp)
znajdziesz pusty szkielet `decideClassification` z komentarzem `// TODO`. Funkcje `classify` i
`toDiverterCommand` są już gotowe.

## Co masz napisać

Uzupełnij ciało `decideClassification` zgodnie z regułą powyżej.

## Sprawdź się

```bash
ctest --preset test -L misja-22
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test zawiera przypadek
`{Stale, 750}`. Sama wartość wygląda wiarygodnie, ale status nie pozwala użyć jej do klasyfikacji.

## Częste błędy

- **Sprawdzanie `weight.grams` zamiast `weight.status`:** o wiarygodności odczytu decyduje status, a
  nie wartość pomiaru.

## Pytanie do zastanowienia

Dlaczego ta misja nie przyjmuje jeszcze `PresenceReading` jako drugiego argumentu, skoro misja 23
będzie potrzebowała obu czujników? Co konkretnie poszłoby nie tak, gdybyśmy
spróbowali połączyć oba czujniki w tej samej funkcji, wywoływanej w jednym ticku?

**Dalej:** [Misja 23: pamięć decyzji sterownika](./04_pamiec_decyzji_sterownika.md).
