# 2.1 Polecenie a rzeczywistość

## Problem

W Module 1 Controller zwracał `DiverterPosition` bezpośrednio — funkcja `toDiverterPosition`
odpowiadała jednym słowem: "rozjazd MA BYĆ w pozycji X". To działało, bo rozjazd i tak zawsze
natychmiast tam trafiał. Ale skoro w tym module rozjazd zaczyna **potrzebować czasu**, żeby się
przestawić, ta sama nazwa przestaje mieć sens: `DiverterPosition` powinno opisywać, gdzie rozjazd
**faktycznie jest w tej chwili**, a nie gdzie sterownik chciałby, żeby był.

Innymi słowy: "czego chcemy" i "co fizycznie istnieje" to dwie różne rzeczy, i mylenie ich było
błędem, który ten moduł naprawia.

## Nowy element C++

**`enum class DiverterCommand`** — polecenie, żądanie, a nie stan fizyczny:

```cpp
enum class DiverterCommand { HoldStraight, Divert };
```

To jest jedyny nowy element składniowy w tej misji — sam enum niczym nie różni się od tych, które już
znasz z Modułu 1. Nowość leży w **pojęciu**, nie w składni: od teraz w kodzie będą istnieć obok siebie
dwa osobne typy, `DiverterCommand` (żądanie) i `DiverterPosition` (rzeczywistość), i nigdy nie wolno
ich mylić.

## Co już masz gotowe

[`include/psm/diverter_command.hpp`](../../include/psm/diverter_command.hpp) — `enum class
DiverterCommand` już zdefiniowany, kompletny.

[`include/psm/controller.hpp`](../../include/psm/controller.hpp) deklaruje:

```cpp
WeightClass classify(Grams mass);
DiverterCommand toDiverterCommand(WeightClass weightClass);
```

Zwróć uwagę: `classify` **nie zmienia się** — próg 500g wciąż działa dokładnie tak, jak w Module 1
(test `misja-4` to potwierdza, i wciąż przechodzi). Zmienia się wyłącznie druga funkcja — teraz
zwraca `DiverterCommand`, nie `DiverterPosition`.

[`src/controller.cpp`](../../src/controller.cpp) ma pusty szkielet `toDiverterCommand`:

```cpp
DiverterCommand toDiverterCommand(WeightClass weightClass) {
    // TODO (Misja 7: polecenie_a_rzeczywistosc): zmapuj Light -> HoldStraight, Heavy -> Divert.
    (void)weightClass;
    return DiverterCommand::HoldStraight;
}
```

## Co masz napisać

Uzupełnij `toDiverterCommand` tak, żeby:
- `WeightClass::Light` dawało `DiverterCommand::HoldStraight` (lekka paczka: rozjazd zostaje prosto),
- `WeightClass::Heavy` dawało `DiverterCommand::Divert` (ciężka paczka: rozjazd ma skręcić).

## Self-check

```bash
ctest --preset test -L misja-7
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`.

## Częste błędy

- **Zamiana kierunków** — sprawdź dokładnie, które słowo mapuje się na które (Light→HoldStraight,
  Heavy→Divert), test sprawdza obie strony.
- **Próba użycia starego `toDiverterPosition`** — ta funkcja już nie istnieje w tym module; jeśli Twój
  edytor podpowiada ją ze starej pamięci albo z innego pliku, to znak, że coś jest pomieszane.

## Pytanie do zastanowienia

`classify` zostaje bez zmian, ale funkcja, która przekształca jej wynik na akcję rozjazdu, zmienia i
nazwę, i typ zwracany. Dlaczego to jest właściwe miejsce na tę zmianę, a nie np. w `classify` samym?

**Dalej:** [Misja 8: dywerter jako klasa](./02_dywerter_jako_klasa.md).
