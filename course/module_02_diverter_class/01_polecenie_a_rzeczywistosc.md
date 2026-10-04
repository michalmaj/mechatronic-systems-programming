🇵🇱 Polski | [🇬🇧 English](01_polecenie_a_rzeczywistosc.en.md)

# 2.1 Polecenie a rzeczywistość

## Problem

W module 1 sterownik zwracał bezpośrednio `DiverterPosition`. Mogło to działać, dopóki rozjazd
natychmiast osiągał zadaną pozycję. Teraz `DiverterPosition` będzie opisywać jego rzeczywiste
położenie, a polecenie sterownika otrzyma osobny typ.

Rozdzielamy więc wartość zadaną od stanu fizycznego urządzenia.

## Nowy element C++

**`enum class DiverterCommand`** opisuje polecenie, a nie stan fizyczny:

```cpp
enum class DiverterCommand { HoldStraight, Divert };
```

Składnię `enum class` znasz już z modułu 1. Nowe jest rozróżnienie pojęć: `DiverterCommand` opisuje
żądanie, a `DiverterPosition` rzeczywiste położenie.

## Co już masz gotowe

W pliku
[`include/psm/diverter_command.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-02-start/include/psm/diverter_command.hpp)
znajdziesz kompletną definicję `enum class DiverterCommand`.

[`include/psm/controller.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-02-start/include/psm/controller.hpp) deklaruje:

```cpp
WeightClass classify(Grams mass);
DiverterCommand toDiverterCommand(WeightClass weightClass);
```

Funkcja `classify` **nie zmienia się**. Próg 500 g działa tak samo jak w module 1, co nadal
potwierdza test `misja-4`. Zmienia się wyłącznie druga funkcja. Teraz zwraca `DiverterCommand`, a
nie `DiverterPosition`.

[`src/controller.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-02-start/src/controller.cpp) ma pusty szkielet `toDiverterCommand`:

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

## Sprawdź się

```bash
ctest --preset test -L misja-7
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`.

## Częste błędy

- **Zamiana kierunków:** sprawdź przyporządkowanie wartości (Light→HoldStraight,
  Heavy→Divert), test sprawdza obie strony.
- **Próba użycia starego `toDiverterPosition`:** ta funkcja już nie istnieje w tym module. Jeśli Twój
  edytor podpowiada ją ze starej pamięci albo z innego pliku, to znak, że coś jest pomieszane.

## Pytanie do zastanowienia

`classify` zostaje bez zmian, ale funkcja, która przekształca jej wynik na polecenie dla rozjazdu,
zmienia zarówno nazwę, jak i typ zwracany. Dlaczego właśnie w niej należy wprowadzić tę zmianę, a
nie w `classify`?

**Dalej:** [Misja 8: dywerter jako klasa](./02_dywerter_jako_klasa.md).
