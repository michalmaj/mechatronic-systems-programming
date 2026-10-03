🇵🇱 Polski | [🇬🇧 English](03_stan_przenosnika.en.md)

# 1.3 Stan przenośnika

## Problem

Do tej pory pracowałeś z jedną paczką, o której zawsze zakładałeś, że istnieje. Ale prawdziwy
przenośnik bywa pusty — zanim pierwsza paczka wjedzie, albo po tym, jak ostatnia go opuści. `Item`
sam w sobie nie potrafi wyrazić "nic tu nie ma" — to zawsze *jakaś* konkretna paczka, z konkretnym
`id`, `zone` i `mass`.

Potrzebny jest również obiekt reprezentujący **cały przenośnik** i przechowujący paczkę, jeśli ta
znajduje się na linii.

## Nowe elementy C++

**`std::optional<Item>`** — typ, który albo zawiera wartość `Item`, albo jest pusty (`std::nullopt`).
`std::optional<Item>` z wartością oznacza obecność paczki, a pusty — wolny przenośnik. Bez
`std::optional` trzeba byłoby umówić się na specjalne `id` oznaczające brak paczki, które łatwo
pomylić z prawidłową wartością.

**`struct Plant`** — grupuje stan przenośnika. Na razie zawiera jedno pole:

```cpp
struct Plant {
    std::optional<Item> item;
};
```

Zwykły `struct` wystarcza, ponieważ nie ma jeszcze niezmiennika wymagającego ochrony przez prywatne
pola. Klasa pojawi się później, gdy taki niezmiennik rzeczywiście będzie potrzebny.

**Funkcje wolne działające na `Plant&`** — tak jak `advanceZone` z Misji 2 działała na `Item&`, teraz
piszesz funkcje działające na `Plant&`, i **ponownie wykorzystujesz `advanceZone`** zamiast pisać
logikę przesuwania od nowa.

## Co już masz gotowe

[`include/psm/plant.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/include/psm/plant.hpp):

```cpp
struct Plant {
    std::optional<Item> item;
};

void spawnItem(Plant& plant, Item item);
void advance(Plant& plant, DiverterPosition diverterPosition);
```

Parametr `diverterPosition` w `advance` nie będzie jeszcze używany. Przyda się w misji 4; jest już
w sygnaturze, aby nie trzeba było jej później zmieniać.

[`src/plant.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/src/plant.cpp) zawiera pusty szkielet obu funkcji z komentarzami `// TODO`.

## Co masz napisać

**`spawnItem(Plant&, Item)`** — jeśli `plant.item` jest teraz puste, umieść w nim przekazaną paczkę
(w strefie `Infeed`). Jeśli przenośnik jest już zajęty, odrzuć próbę dodania paczki. W tym module nie
modelujemy jeszcze kolejki wejściowej.

**`advance(Plant&, DiverterPosition)`** — jeśli `plant.item` ma wartość i ta paczka **nie** jest
jeszcze w `Diverting`, przesuń ją o jedną strefę (użyj `advanceZone` z Misji 2, wywołanej na
`*plant.item`). Jeśli `plant.item` jest puste — nic nie rób. Zachowanie w `Diverting` zostaw na razie
bez zmian (paczka czeka) — to znowu praca dla Misji 4.

## Sprawdź się

```bash
ctest --preset test -L misja-3
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza pusty `Plant`,
poprawne dodanie paczki, odrzucenie drugiej próby `spawnItem` przy zajętym przenośniku, oraz że trzy
wywołania `advance` z rzędu doprowadzają paczkę do `Diverting`.

## Częste błędy

- **Zapomniane sprawdzenie `has_value()`** — wywołanie `*plant.item` na pustym `std::optional` to
  niezdefiniowane zachowanie (program może się wywalić albo, gorzej, pozornie "działać" i dawać złe
  wyniki). Zawsze sprawdź `has_value()` (albo warunek `if (plant.item)`, co znaczy to samo) przed
  użyciem `*plant.item`.
- **Nadpisywanie zajętego przenośnika w `spawnItem`** — pamiętaj o warunku "tylko jeśli pusty".
- **Ponowne pisanie logiki przesuwania stref od zera** zamiast wywołania `advanceZone` — w tej
  misji nie musisz (i nie powinieneś) duplikować tego, co już masz z Misji 2.

## Pytanie do zastanowienia

Dlaczego `std::optional<Item>` to lepszy wybór niż np. dodanie do `Item` pola `bool exists`?

**Dalej:** [Misja 4: decyzja sortowania](./04_decyzja_sortowania.md).
