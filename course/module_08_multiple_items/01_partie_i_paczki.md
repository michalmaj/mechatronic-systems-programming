# 8.1 Partie i paczki

## Problem

`Plant` do tej pory mieścił dokładnie jedną paczkę. Druga próba `spawnItem` była po cichu ignorowana.
Żeby modelować kilka paczek naraz, `Plant` potrzebuje osobnego miejsca na każdą strefę — i sposobu,
żeby jednoznacznie odróżnić od siebie paczki, które są w nim jednocześnie.

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

`Item` nie ma już pola `zone`. Skoro `Plant` będzie miał osobne pole na każdą strefę, przynależność do
strefy to fakt wynikający z tego, w którym polu leży dana paczka — trzymanie dodatkowego pola `zone`
obok tego byłoby dwoma źródłami tej samej prawdy, które mogłyby się rozjechać.

```cpp
struct Plant {
    std::optional<Item> infeed;
    std::optional<Item> presenceCheck;
    std::optional<Item> weighing;
    std::optional<Item> diverting;
};

bool spawnItem(Plant& plant, ItemId id, Grams mass);
```

Każde pole mieści najwyżej jedną paczkę. `spawnItem` przyjmuje teraz tylko `ItemId` i `Grams` — nie
cały `Item` — i sam konstruuje świeży `Item` wewnątrz. Skoro wywołujący nie ma żadnej możliwości
przekazać "używanego" `Item` z niezerowym stanem przetwarzania, nie ma też czego zerować.

## Kontrakt: co `spawnItem` musi gwarantować

`ItemId` musi być unikalne wśród paczek **aktualnie obecnych** w `Plant` — nie wśród wszystkich, które
kiedykolwiek istniały. Gdy paczka opuszcza system (odjeżdża do `OutputLight`/`OutputHeavy`), jej id
jest wolne do ponownego użycia. Nie ma globalnego rejestru id i nie jest potrzebny — `spawnItem` sam
odrzuca kolizję, sprawdzając tylko to, co jest obecne *teraz*.

Funkcja musi zwrócić `false` i zostawić `plant` bez żadnej zmiany w dwóch niezależnych przypadkach:

- `infeed` jest już zajęty — nie ma gdzie umieścić nowej paczki,
- podane `id` koliduje z id dowolnej *innej* paczki obecnej gdziekolwiek w `Plant`, czyli w
  `presenceCheck`, `weighing` albo `diverting` (kolizja z samym `infeed` jest już pokryta pierwszym
  warunkiem, więc nie trzeba jej sprawdzać osobno).

W każdym innym przypadku funkcja umieszcza w `infeed` świeżo skonstruowany `Item` o podanych `id` i
`mass` i zwraca `true`. Wywołujący (docelowo `Engine`, a nad nim CLI/testy) odpowiada za ponowną próbę w
kolejnym ticku i za wybór id, które się nie koliduje — nie ma tu wewnętrznej kolejki ani generatora id.

## Co już masz gotowe

[`include/psm/item.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/include/psm/item.hpp) — nowy kształt `Item`, kompletny.

[`include/psm/plant.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/include/psm/plant.hpp) — nowy kształt `Plant`, `ItemDeparture`,
`AdvanceResult`, sygnatury `spawnItem`/`advance` — wszystko gotowe.

[`src/plant.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/plant.cpp) — `advance()` na razie celowo nic nie robi (to praca Misji 30,
nie tej) — nie polegaj na tym, że cokolwiek przesuwa, dopóki nie napiszesz Misji 30. Ciało `spawnItem`
jest `// TODO`.

## Co masz napisać

Uzupełnij ciało `spawnItem` w [`src/plant.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/plant.cpp) tak, by spełniało kontrakt opisany
wyżej.

## Sprawdź się

```bash
ctest --preset test -L misja-29
```

Oczekiwany wynik: `100% tests passed`. Test sprawdza: udany spawn do pustego `infeed`; odrzucenie, gdy
`infeed` jest zajęty; odrzucenie przy kolizji id z paczką obecną gdziekolwiek indziej w `Plant`; że
kolejne wywołanie z id, które nie koliduje, udaje się mimo obecności innej paczki.

## Częste błędy

- **Sprawdzanie kolizji id tylko względem `infeed`** — kolizja z paczką w `presenceCheck`/`weighing`/
  `diverting` musi być odrzucona równie stanowczo.
- **Ręczne zerowanie `presenceConfirmed`/`classification`/`divertingWaitTicks` po konstrukcji** —
  niepotrzebne: `Item{id, mass}` już bierze resztę pól z ich domyślnych wartości w klasie.
- **Próba przyjęcia całego `Item` zamiast `ItemId`/`Grams`** — to dokładnie ta niejednoznaczność
  ("co znaczy przekazać 'używany' `Item`?"), której zawężona sygnatura ma unikać.

## Pytanie do zastanowienia

`spawnItem` nie ma wewnętrznej kolejki — druga nieudana próba po prostu przepada, chyba że wywołujący
spróbuje ponownie. Dlaczego to jest właściwa odpowiedzialność dla wywołującego (np. `Engine` albo CLI),
a nie dla samego `Plant`?

**Dalej:** [Misja 30: przesuwanie partii](./02_przesuwanie_partii.md).
