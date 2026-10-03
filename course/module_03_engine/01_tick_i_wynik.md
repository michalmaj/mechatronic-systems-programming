🇵🇱 Polski | [🇬🇧 English](01_tick_i_wynik.en.md)

# 3.1 Tick i wynik

## Problem

W kolejnej misji zbudujesz `Engine`, który wykonuje jeden pełny cykl symulacji. Najpierw potrzebny
jest typ opisujący wynik takiego cyklu oraz funkcja zamieniająca go na czytelny tekst.

## Nowe elementy C++

**`using Tick = std::uint64_t;`** — alias typu. `Tick` to wciąż zwykła liczba całkowita (bez znaku,
64-bitowa — licznik ticków nie powinien przepełnić się podczas symulacji), ale nazwa `Tick` wyjaśnia
znaczenie liczby lepiej niż zwykły `int`.

**`struct TickResult`** — migawka jednego ticka:

```cpp
struct TickResult {
    Tick tick;
    std::optional<Item> item;
    DiverterCommand diverterCommand;
    DiverterPosition diverterActual;
};
```

To zwykła struktura przechowująca dane, podobnie jak `Item` i `Plant` w module 1. Zawiera zarówno
`diverterCommand` (wydane polecenie), jak i `diverterActual` (rzeczywiste położenie). Bez jednej z
tych wartości nie dałoby się odtworzyć pełnego wyniku ticku.

## Co już masz gotowe

[`include/psm/tick.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/include/psm/tick.hpp) i
[`include/psm/tick_result.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/include/psm/tick_result.hpp) — `Tick` i `TickResult` już w
pełni zdefiniowane. Deklaracja `describe` też już tam jest:

```cpp
std::string describe(const TickResult& result);
```

[`src/tick_result.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/src/tick_result.cpp) ma pusty szkielet:

```cpp
std::string describe(const TickResult& result) {
    // TODO (Misja 10: tick_i_wynik): ...
    (void)result;
    return "TODO";
}
```

## Co masz napisać

Uzupełnij `describe`, żeby zwracał:
- `"tick T: item ID in zone Z"`, gdy `result.item` ma wartość (`T` = `result.tick`, `ID` =
  `result.item->id`, `Z` = `psm::toString(result.item->zone)` z modułu 1),
- `"tick T: empty"`, gdy `result.item` nie ma wartości.

Będziesz potrzebować `#include <psm/zone.hpp>` (dla `psm::toString`) oraz zamiany liczb na tekst —
`std::to_string` z `<string>` (już dołączonego przez `tick_result.hpp`) załatwia to bez dodatkowego
wysiłku.

Funkcja `describe` zostanie podłączona do programu dopiero w misji 12.

## Sprawdź się

```bash
ctest --preset test -L misja-10
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza obie sytuacje: paczkę
obecną i pusty przenośnik, porównując dokładny, oczekiwany napis.

## Częste błędy

- **Zły format napisu** — test porównuje znak w znak. Sprawdź spacje i dwukropki.
- **Użycie `result.item->id` bez wcześniejszego sprawdzenia `has_value()`** — tak jak zawsze przy
  `std::optional`, najpierw sprawdź, potem odczytaj.
- **Zapomniany `#include <psm/zone.hpp>`** — `psm::toString(Zone)` nie jest automatycznie widoczny
  przez sam `tick_result.hpp`.

## Pytanie do zastanowienia

`TickResult` przechowuje zarówno `diverterCommand`, jak i `diverterActual`, mimo że to Moduł 2 już
wprowadził oba te pojęcia osobno w `Diverter`. Po co powtarzać tę informację w migawce, skoro
teoretycznie dałoby się ją zawsze dociągnąć bezpośrednio z obiektu `Diverter`?

**Dalej:** [Misja 11: silnik formalizuje kolejność](./02_silnik_formalizuje_kolejnosc.md).
