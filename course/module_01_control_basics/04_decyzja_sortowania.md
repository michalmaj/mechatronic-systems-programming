🇵🇱 Polski | [🇬🇧 English](04_decyzja_sortowania.en.md)

# 1.4 Decyzja sortowania

## Problem

Od Misji 2 masz paczkę utykającą w `Diverting`. Czas ją stamtąd uwolnić — ale nie w dowolną stronę:
lekkie paczki mają jechać na jedno wyjście, ciężkie na drugie. Innymi słowy: potrzebujesz **decyzji**,
a nie tylko ruchu.

Od tej chwili rozdzielamy dwie odpowiedzialności. `Plant` opisuje stan fizyczny, czyli położenie
paczki. Funkcje nazywane wspólnie Controllerem wybierają jej trasę. Controller nie jest jeszcze
klasą.

## Nowe elementy C++

**`enum class WeightClass`** — nazwany wynik decyzji (`Light`/`Heavy`) zamiast gołego `bool`.
`bool` typu `true`/`false` nie mówi nic o *znaczeniu* — trzeba by pamiętać, czy `true` znaczy
„lekka”, czy „ciężka”. Nazwa `WeightClass::Light` nie pozostawia tej wątpliwości.

```cpp
enum class WeightClass { Light, Heavy };
```

**Funkcja zwracająca wartość na podstawie `if` z progiem:**

```cpp
WeightClass classify(Grams mass);
```

**`enum class DiverterPosition`** — fizyczne położenie rozjazdu, do którego mapujemy wynik
klasyfikacji:

```cpp
enum class DiverterPosition { Straight, Diverted };
```

## Co już masz gotowe

[`include/psm/weight_class.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/include/psm/weight_class.hpp) i
[`include/psm/diverter_position.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/include/psm/diverter_position.hpp) — oba `enum class` już
kompletne.

[`include/psm/controller.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/include/psm/controller.hpp) deklaruje dwie funkcje:

```cpp
WeightClass classify(Grams mass);
DiverterPosition toDiverterPosition(WeightClass weightClass);
```

[`src/controller.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/src/controller.cpp) ma ich puste szkielety z komentarzami `// TODO`.

W funkcji `advance` z misji 3 znajdziesz drugi komentarz `// TODO (Misja 4: ...)`. Uzupełnisz
istniejącą funkcję, zamiast pisać nową.

## Co masz napisać

**`classify(Grams mass)`** — porównaj `mass` z progiem **500 gramów**: poniżej `WeightClass::Light`,
od `500` włącznie `WeightClass::Heavy`.

**`toDiverterPosition(WeightClass weightClass)`** — zmapuj `Light` na `DiverterPosition::Straight`,
`Heavy` na `DiverterPosition::Diverted`.

**Rozszerz `advance` w `src/plant.cpp`** o brakującą część: gdy paczka **jest** w `Diverting`, użyj
przekazanego parametru `diverterPosition`, żeby zdecydować, czy przenieść ją do `Zone::OutputLight`
(dla `Straight`), czy `Zone::OutputHeavy` (dla `Diverted`). Dodatkowo: gdy paczka jest już w
`OutputLight` albo `OutputHeavy`, wyczyść `plant.item` (`std::nullopt`) — paczka opuszcza system.

## Sprawdź się

```bash
ctest --preset test -L misja-4
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza kilka wartości `mass`
wokół progu 500g oraz oba kierunki mapowania `WeightClass` → `DiverterPosition`.

Warto też ponownie odpalić test Misji 3 (`ctest --preset test -L misja-3`) — powinien nadal
przechodzić, mimo że dopisałeś kod do tej samej funkcji `advance`.

## Dlaczego "Controller", a nie klasa

Nie ma jeszcze powodu, aby `classify` i `toDiverterPosition` umieszczać w klasie `Controller`. Żadna
z tych funkcji nie przechowuje stanu między wywołaniami: przyjmuje dane i zwraca wynik. Nazwa
„Controller” oznacza tu grupę funkcji odpowiedzialnych za decyzję.

## Częste błędy

- **Próg jako `<=` zamiast `<`** — paczka o masie 500 g należy do `Heavy`. Test obejmuje tę wartość
  graniczną.
- **Zapomniane wyczyszczenie `plant.item` po dotarciu do strefy wyjściowej** — bez tego paczka
  "utknie" tym razem już na dobre, w `OutputLight`/`OutputHeavy`.
- **Zmiana sygnatury `advance`** — parametr `diverterPosition` już tam jest od Misji 3; nie musisz
  (i nie powinieneś) dodawać nowych parametrów ani zmieniać istniejących.

## Pytanie do zastanowienia

Co by się stało, gdyby `Plant::advance` samo, wewnątrz siebie, wywoływało `classify` i
`toDiverterPosition` zamiast dostawać gotowy `DiverterPosition` jako parametr? Czy to nadal byłby
podział "stan vs. decyzja"?

**Dalej:** [Misja 5: pętla sterowania](./05_petla_sterowania.md).
