🇵🇱 Polski | [🇬🇧 English](01_tick_i_wynik.en.md)

# 3.1 Tick i wynik

## Problem

W kolejnej misji zbudujesz `Engine`, który wykonuje jeden pełny cykl symulacji. Najpierw potrzebny
jest typ opisujący wynik takiego cyklu oraz funkcja zamieniająca go na czytelny tekst.

## Nowe elementy C++

**`using Tick = std::uint64_t;`** tworzy alias typu. `Tick` to nadal zwykła 64-bitowa liczba
całkowita bez znaku. Taki zakres wystarczy, aby licznik nie przepełnił się podczas symulacji, a
nazwa `Tick` wyjaśnia znaczenie wartości lepiej niż zwykły `int`.

**`struct TickResult`** przechowuje wynik jednego ticka:

```cpp
struct TickResult {
    Tick tick;
    std::optional<Item> item;
    DiverterCommand diverterCommand;
    DiverterPosition diverterActual;
};
```

Podobnie jak `Item` i `Plant` w module 1, jest to zwykła struktura przechowująca dane. Zawiera
zarówno `diverterCommand` (wydane polecenie), jak i `diverterActual` (rzeczywiste położenie). Bez
jednej z tych wartości opis ticku byłby niepełny.

## Co już masz gotowe

[`include/psm/tick.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/include/psm/tick.hpp) i
[`include/psm/tick_result.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/include/psm/tick_result.hpp)
zawierają kompletne definicje `Tick` i `TickResult`. Jest tam również deklaracja `describe`:

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

Będziesz potrzebować `#include <psm/zone.hpp>`, aby skorzystać z `psm::toString`. Do zamiany liczb
na tekst wystarczy `std::to_string` z nagłówka `<string>`, który jest już dołączony przez
`tick_result.hpp`.

Funkcja `describe` zostanie podłączona do programu dopiero w misji 12.

## Sprawdź się

```bash
ctest --preset test -L misja-10
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza obie sytuacje: paczkę
obecną i pusty przenośnik, porównując dokładny, oczekiwany napis.

## Częste błędy

- **Zły format napisu:** test porównuje znak w znak. Sprawdź spacje i dwukropki.
- **Użycie `result.item->id` bez wcześniejszego sprawdzenia `has_value()`:** tak jak zawsze przy
  `std::optional`, najpierw sprawdź, potem odczytaj.
- **Zapomniany `#include <psm/zone.hpp>`:** `psm::toString(Zone)` nie jest automatycznie widoczny
  przez sam `tick_result.hpp`.

## Pytanie do zastanowienia

`TickResult` przechowuje zarówno `diverterCommand`, jak i `diverterActual`, mimo że moduł 2
wprowadził już oba te pojęcia w klasie `Diverter`. Po co zapisywać te informacje również w wyniku
ticku, zamiast w razie potrzeby odczytywać bieżący stan obiektu `Diverter`?

**Dalej:** [Misja 11: silnik porządkuje kolejność](./02_silnik_formalizuje_kolejnosc.md).
