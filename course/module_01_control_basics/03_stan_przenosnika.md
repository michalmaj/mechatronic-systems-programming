# 1.3 Stan przenośnika

## Problem

Do tej pory pracowałeś z jedną paczką, o której zawsze zakładałeś, że istnieje. Ale prawdziwy
przenośnik bywa pusty — zanim pierwsza paczka wjedzie, albo po tym, jak ostatnia go opuści. `Item`
sam w sobie nie potrafi wyrazić "nic tu nie ma" — to zawsze *jakaś* konkretna paczka, z konkretnym
`id`, `zone` i `mass`.

Potrzebujesz też czegoś, co reprezentuje **cały przenośnik**, a nie tylko pojedynczą paczkę — miejsca,
gdzie ta ewentualna paczka mieszka.

## Nowe elementy C++

**`std::optional<Item>`** — typ, który albo zawiera wartość `Item`, albo jest pusty (`std::nullopt`).
To dokładnie odpowiada pytaniu "czy coś tu jest": `std::optional<Item>` z wartością znaczy "tak, oto
ona"; pusty — "nie, przenośnik jest wolny". Bez `std::optional` musiałbyś wymyślać sztuczne
rozwiązania w rodzaju "specjalne id oznaczające brak paczki", co łatwo pomylić z prawdziwą paczką.

**`struct Plant`** — grupuje stan przenośnika. Na razie to dokładnie jedno pole:

```cpp
struct Plant {
    std::optional<Item> item;
};
```

To celowo zwykły `struct`, nie `class` z prywatnymi polami i metodami. Na tym etapie nic nie musimy
chronić przed niepoprawnym użyciem — enkapsulacja ma sens wtedy, gdy istnieje jakiś niezmiennik do
pilnowania, a na razie takiego nie mamy. Gdy taki niezmiennik się pojawi (w dalszych modułach), to
będzie konkretny powód do przejścia na `class` — nie wcześniej.

**Funkcje wolne działające na `Plant&`** — tak jak `advanceZone` z Misji 2 działała na `Item&`, teraz
piszesz funkcje działające na `Plant&`, i **ponownie wykorzystujesz `advanceZone`** zamiast pisać
logikę przesuwania od nowa.

## Co już masz gotowe

[`include/psm/plant.hpp`](../../include/psm/plant.hpp):

```cpp
struct Plant {
    std::optional<Item> item;
};

void spawnItem(Plant& plant, Item item);
void advance(Plant& plant, DiverterPosition diverterPosition);
```

Zauważ parametr `diverterPosition` w `advance` — na razie go **zignoruj**. Wróci na scenę w Misji 4;
tutaj wystarczy wiedzieć, że tam jest, żeby sygnatura funkcji nie musiała się później zmieniać.

[`src/plant.cpp`](../../src/plant.cpp) zawiera pusty szkielet obu funkcji z komentarzami `// TODO`.

## Co masz napisać

**`spawnItem(Plant&, Item)`** — jeśli `plant.item` jest teraz puste, umieść w nim przekazaną paczkę
(na strefie `Infeed`). Jeśli przenośnik jest już zajęty, nic nie rób — nowa paczka po prostu czeka
(w tym module nie modelujemy kolejki wejściowej, tylko odrzucamy próbę).

**`advance(Plant&, DiverterPosition)`** — jeśli `plant.item` ma wartość i ta paczka **nie** jest
jeszcze w `Diverting`, przesuń ją o jedną strefę (użyj `advanceZone` z Misji 2, wywołanej na
`*plant.item`). Jeśli `plant.item` jest puste — nic nie rób. Zachowanie w `Diverting` zostaw na razie
bez zmian (paczka czeka) — to znowu praca dla Misji 4.

## Self-check

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
