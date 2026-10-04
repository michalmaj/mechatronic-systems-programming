🇵🇱 Polski | [🇬🇧 English](04_decyzja_sortowania.en.md)

# 1.4 Decyzja sortowania

## Problem

Od misji 2 paczka zatrzymuje się w `Diverting`. Teraz trzeba skierować ją do właściwego wyjścia:
lekkie paczki mają jechać na jedno wyjście, ciężkie na drugie. Innymi słowy: potrzebujesz **decyzji**,
a nie tylko ruchu.

Od tej chwili rozdzielamy dwie odpowiedzialności. `Plant` opisuje stan fizyczny, czyli położenie
paczki. Funkcje sterownika wybierają jej trasę. Sam sterownik nie jest jeszcze klasą.

## Nowe elementy C++

**`enum class WeightClass`** zapisuje wynik decyzji jako `Light` albo `Heavy` zamiast surowego `bool`.
Wartość `true` lub `false` nie wyjaśnia znaczenia. Trzeba byłoby pamiętać, czy `true` znaczy
„lekka”, czy „ciężka”. Nazwa `WeightClass::Light` nie pozostawia tej wątpliwości.

```cpp
enum class WeightClass { Light, Heavy };
```

**Funkcja zwracająca wartość na podstawie `if` z progiem:**

```cpp
WeightClass classify(Grams mass);
```

**`enum class DiverterPosition`** opisuje fizyczne położenie rozjazdu odpowiadające wynikowi
klasyfikacji:

```cpp
enum class DiverterPosition { Straight, Diverted };
```

## Co już masz gotowe

[`include/psm/weight_class.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/include/psm/weight_class.hpp) i
[`include/psm/diverter_position.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/include/psm/diverter_position.hpp). Oba `enum class` są już
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

**`classify(Grams mass)`:** porównaj `mass` z progiem **500 gramów**. Wartość poniżej progu oznacza
`WeightClass::Light`, a od `500` włącznie `WeightClass::Heavy`.

**`toDiverterPosition(WeightClass weightClass)`:** przypisz `Light` do `DiverterPosition::Straight`,
`Heavy` na `DiverterPosition::Diverted`.

**Rozszerz `advance` w `src/plant.cpp`** o brakującą część: gdy paczka **jest** w `Diverting`, użyj
przekazanego parametru `diverterPosition`, żeby zdecydować, czy przenieść ją do `Zone::OutputLight`
(dla `Straight`), czy `Zone::OutputHeavy` (dla `Diverted`). Dodatkowo: gdy paczka jest już w
`OutputLight` albo `OutputHeavy`, wyczyść `plant.item` (`std::nullopt`). Paczka opuszcza system.

## Sprawdź się

```bash
ctest --preset test -L misja-4
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza kilka wartości `mass`
wokół progu 500 g oraz oba przypisania wartości `WeightClass` do `DiverterPosition`.

Uruchom też ponownie test misji 3 (`ctest --preset test -L misja-3`). Powinien nadal
przechodzić, mimo że dopisałeś kod do tej samej funkcji `advance`.

## Dlaczego sterownik nie jest klasą

Nie ma jeszcze powodu, aby `classify` i `toDiverterPosition` umieszczać w klasie `Controller`. Żadna
z tych funkcji nie przechowuje stanu między wywołaniami. Obie przyjmują dane i zwracają wynik. Na
razie sterownik jest więc grupą funkcji odpowiedzialnych za podejmowanie decyzji.

## Częste błędy

- **Próg jako `<=` zamiast `<`:** paczka o masie 500 g należy do `Heavy`. Test obejmuje tę wartość
  graniczną.
- **Zapomniane wyczyszczenie `plant.item` po dotarciu do strefy wyjściowej:** bez tego paczka
  "utknie" tym razem już na dobre, w `OutputLight`/`OutputHeavy`.
- **Zmiana sygnatury `advance`:** parametr `diverterPosition` jest tam od misji 3. Nie musisz
  (i nie powinieneś) dodawać nowych parametrów ani zmieniać istniejących.

## Pytanie do zastanowienia

Co by się stało, gdyby `Plant::advance` samo, wewnątrz siebie, wywoływało `classify` i
`toDiverterPosition` zamiast dostawać gotowy `DiverterPosition` jako parametr? Czy to nadal byłby
podział na stan i decyzję?

**Dalej:** [Misja 5: pętla sterowania](./05_petla_sterowania.md).
