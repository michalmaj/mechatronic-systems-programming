🇵🇱 Polski | [🇬🇧 English](01_partie_i_paczki.en.md)

# 8.1 Model wielu paczek

## Problem

Do tej pory `Plant` przechowywał tylko jedną paczkę, a próba dodania kolejnej kończyła się
niepowodzeniem. Aby kilka paczek mogło znajdować się w układzie jednocześnie, każda strefa potrzebuje
własnego miejsca. Paczki muszą też mieć niepowtarzalne identyfikatory.

## Nowe elementy C++

```cpp
struct Item {
    ItemId id;
    Grams mass;
    bool presenceConfirmed = false;
    std::optional<WeightClass> classification;
    int divertingWaitTicks = 0;
};
```

`Item` nie ma już pola `zone`. Położenie paczki wynika z pola `Plant`, w którym jest przechowywana.
Pozostawienie `zone` oznaczałoby zapisanie tej samej informacji w dwóch miejscach, które mogłyby się
ze sobą nie zgadzać.

```cpp
struct Plant {
    std::optional<Item> infeed;
    std::optional<Item> presenceCheck;
    std::optional<Item> weighing;
    std::optional<Item> diverting;
};

bool spawnItem(Plant& plant, ItemId id, Grams mass);
```

Każde pole mieści najwyżej jedną paczkę. `spawnItem()` przyjmuje jej `ItemId` i `Grams`, a następnie
sam tworzy nowy `Item`. Wywołujący nie może więc przekazać paczki z wynikiem wcześniejszego
przetwarzania. Pola stanu zawsze otrzymują wartości domyślne.

## Wymagania wobec `spawnItem()`

`ItemId` musi być unikalny wśród paczek, które obecnie znajdują się w `Plant`. Nie musi być unikalny
w całej historii programu. Gdy paczka opuści układ przez `OutputLight` albo `OutputHeavy`, jej
identyfikator można wykorzystać ponownie. Nie jest do tego potrzebny żaden globalny rejestr.

Funkcja ma zwrócić `false` i nie zmienić `plant` w dwóch przypadkach:

- `infeed` jest zajęty i nie można dodać następnej paczki,
- podany identyfikator należy już do paczki w `presenceCheck`, `weighing` albo `diverting`.

Nie musisz osobno sprawdzać identyfikatora paczki w `infeed`, ponieważ zajętość tej strefy już
powoduje odrzucenie operacji.

W pozostałych przypadkach utwórz w `infeed` nowy `Item` z podanymi `id` i `mass`, a następnie zwróć
`true`. Po nieudanej próbie to wywołujący, docelowo `Engine` lub program terminalowy, odpowiada za
ponowienie operacji w kolejnym ticku i wybór wolnego identyfikatora. `Plant` nie ma własnej kolejki
ani generatora identyfikatorów.

## Co już masz gotowe

[`include/psm/item.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/include/psm/item.hpp)
zawiera kompletną, nową definicję `Item`.

W pliku
[`include/psm/plant.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/include/psm/plant.hpp)
znajdziesz nową definicję `Plant`, typy `ItemDeparture` i `AdvanceResult` oraz deklaracje
`spawnItem()` i `advance()`.

W pliku
[`src/plant.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/plant.cpp)
metoda `advance()` pozostaje na razie pusta. Uzupełnisz ją w misji 30. W tej misji zajmij się
oznaczonym przez `// TODO` ciałem `spawnItem()`.

## Co masz napisać

Uzupełnij `spawnItem()` w `src/plant.cpp` zgodnie z wymaganiami opisanymi powyżej.

## Sprawdź się

```bash
ctest --preset test -L misja-29
```

Oczekiwany wynik: `100% tests passed`. Test sprawdza dodanie paczki do pustego `infeed`, odrzucenie
operacji przy zajętej strefie wejściowej oraz wykrywanie powtórzonego identyfikatora w pozostałych
strefach. Potwierdza też, że przy wolnym `infeed` można dodać paczkę z nowym identyfikatorem, choć w
układzie znajdują się już inne paczki.

## Częste błędy

- **Sprawdzenie identyfikatora tylko w `infeed`**: taki sam identyfikator w `presenceCheck`,
  `weighing` albo `diverting` również musi spowodować odrzucenie operacji.
- **Ręczne zerowanie `presenceConfirmed`, `classification` i `divertingWaitTicks` po utworzeniu
  paczki**: konstrukcja `Item{id, mass}` korzysta już z wartości domyślnych pozostałych pól.
- **Zmiana funkcji tak, aby przyjmowała cały `Item`**: przekazanie `ItemId` i `Grams` gwarantuje, że
  nowa paczka nie zawiera stanu pozostałego po wcześniejszym przetwarzaniu.

## Pytanie do zastanowienia

`spawnItem()` nie przechowuje nieudanych prób we własnej kolejce. Dlaczego za ponowienie operacji
powinien odpowiadać wywołujący, na przykład `Engine` albo program terminalowy, a nie sam `Plant`?

**Dalej:** [Misja 30: przesuwanie paczek](./02_przesuwanie_partii.md).
