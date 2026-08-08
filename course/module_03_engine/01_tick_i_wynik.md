# 3.1 Tick i wynik

## Problem

W kolejnej misji zbudujesz `Engine` — coś, co wykonuje jeden pełny cykl symulacji na raz. Zanim to
zrobisz, potrzebujesz sposobu, żeby opisać **wynik** takiego cyklu: jeden, niezmienny zestaw
informacji o tym, co się właśnie wydarzyło. I skoro taki opis będzie istniał, przyda się też sposób,
żeby zamienić go na czytelny dla człowieka tekst.

## Nowe elementy C++

**`using Tick = std::uint64_t;`** — alias typu. `Tick` to wciąż zwykła liczba całkowita (bez znaku,
64-bitowa — licznik ticków nigdy nie powinien "przekręcić się" z powrotem do zera), ale nazwa `Tick`
mówi czytelnikowi kodu dokładnie, co ta liczba reprezentuje, zamiast zostawiać go z gołym `int`.

**`struct TickResult`** — migawka jednego ticka:

```cpp
struct TickResult {
    Tick tick;
    std::optional<Item> item;
    DiverterCommand diverterCommand;
    DiverterPosition diverterActual;
};
```

To zwykły `struct`, bez żadnego niezmiennika do ochrony — czysto bierny zestaw danych, taki sam
wybór jak `Item` czy `Plant` w Module 1. Zwróć uwagę, że migawka zawiera **oba** elementy rozróżnienia
poznanego w Module 2: `diverterCommand` (czego chcieliśmy) i `diverterActual` (co faktycznie się
stało) — gdyby zabrakło jednego z nich, wynik nie pokazywałby całej prawdy o tym ticku.

## Co już masz gotowe

[`include/psm/tick.hpp`](../../include/psm/tick.hpp) i
[`include/psm/tick_result.hpp`](../../include/psm/tick_result.hpp) — `Tick` i `TickResult` już w
pełni zdefiniowane. Deklaracja `describe` też już tam jest:

```cpp
std::string describe(const TickResult& result);
```

[`src/tick_result.cpp`](../../src/tick_result.cpp) ma pusty szkielet:

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
  `result.item->id`, `Z` = `psm::toString(result.item->zone)` z Modułu 1),
- `"tick T: empty"`, gdy `result.item` nie ma wartości.

Będziesz potrzebować `#include <psm/zone.hpp>` (dla `psm::toString`) oraz zamiany liczb na tekst —
`std::to_string` z `<string>` (już dołączonego przez `tick_result.hpp`) załatwia to bez dodatkowego
wysiłku.

`describe` nie ma jeszcze żadnego wywołania w programie — to celowe. Dostanie swojego pierwszego
użytkownika dopiero w Misji 12.

## Self-check

```bash
ctest --preset test -L misja-10
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza obie sytuacje: paczkę
obecną i pusty przenośnik, porównując dokładny, oczekiwany napis.

## Częste błędy

- **Zły format napisu** — test porównuje dokładnie, znak w znak. Sprawdź spacje i dwukropki.
- **Użycie `result.item->id` bez wcześniejszego sprawdzenia `has_value()`** — tak jak zawsze przy
  `std::optional`, najpierw sprawdź, potem odczytaj.
- **Zapomniany `#include <psm/zone.hpp>`** — `psm::toString(Zone)` nie jest automatycznie widoczny
  przez sam `tick_result.hpp`.

## Pytanie do zastanowienia

`TickResult` przechowuje zarówno `diverterCommand`, jak i `diverterActual`, mimo że to Module 2 już
wprowadził oba te pojęcia osobno w `Diverter`. Po co powtarzać tę informację w migawce, skoro
teoretycznie dałoby się ją zawsze dociągnąć bezpośrednio z obiektu `Diverter`?

**Dalej:** [Misja 11: silnik formalizuje kolejność](./02_silnik_formalizuje_kolejnosc.md).
