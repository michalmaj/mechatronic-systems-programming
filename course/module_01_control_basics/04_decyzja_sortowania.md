# 1.4 Decyzja sortowania

## Problem

Od Misji 2 masz paczkę utykającą w `Diverting`. Czas ją stamtąd uwolnić — ale nie w dowolną stronę:
lekkie paczki mają jechać na jedno wyjście, ciężkie na drugie. Innymi słowy: potrzebujesz **decyzji**,
a nie tylko ruchu.

To dobry moment, żeby nazwać podział odpowiedzialności, który będzie obowiązywał przez resztę
modułu: **`Plant` opisuje stan fizyczny** (gdzie jest paczka), a **decyzja o tym, dokąd ją skierować,
to osobna sprawa** — nazwiemy ją zbiorczo "Controllerem". To na razie tylko nazwa dla kilku funkcji,
nie żadna nowa klasa (więcej o tym niżej).

## Nowe elementy C++

**`enum class WeightClass`** — nazwany wynik decyzji (`Light`/`Heavy`) zamiast gołego `bool`.
`bool` typu `true`/`false` nie mówi nic o *znaczeniu* — trzeba by pamiętać, czy `true` znaczy
"lekka", czy "ciężka". `WeightClass::Light` mówi to wprost, w miejscu użycia.

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

[`include/psm/weight_class.hpp`](../../include/psm/weight_class.hpp) i
[`include/psm/diverter_position.hpp`](../../include/psm/diverter_position.hpp) — oba `enum class` już
kompletne.

[`include/psm/controller.hpp`](../../include/psm/controller.hpp) deklaruje dwie funkcje:

```cpp
WeightClass classify(Grams mass);
DiverterPosition toDiverterPosition(WeightClass weightClass);
```

[`src/controller.cpp`](../../src/controller.cpp) ma ich puste szkielety z komentarzami `// TODO`.

`src/plant.cpp`'s `advance` z Misji 3 ma **drugi** komentarz `// TODO (Misja 4: ...)` w tym samym
ciele funkcji, który dziś uzupełnisz — to nie nowa funkcja, tylko rozwinięcie tej, którą zacząłeś
w poprzedniej misji.

## Co masz napisać

**`classify(Grams mass)`** — porównaj `mass` z progiem **500 gramów**: poniżej `WeightClass::Light`,
od `500` włącznie `WeightClass::Heavy`.

**`toDiverterPosition(WeightClass weightClass)`** — zmapuj `Light` na `DiverterPosition::Straight`,
`Heavy` na `DiverterPosition::Diverted`.

**Rozszerz `advance` w `src/plant.cpp`** o brakującą część: gdy paczka **jest** w `Diverting`, użyj
przekazanego parametru `diverterPosition`, żeby zdecydować, czy przenieść ją do `Zone::OutputLight`
(dla `Straight`), czy `Zone::OutputHeavy` (dla `Diverted`). Dodatkowo: gdy paczka jest już w
`OutputLight` albo `OutputHeavy`, wyczyść `plant.item` (`std::nullopt`) — paczka opuszcza system.

## Self-check

```bash
ctest --preset test -L misja-4
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza kilka wartości `mass`
wokół progu 500g oraz oba kierunki mapowania `WeightClass` → `DiverterPosition`.

Warto też ponownie odpalić test Misji 3 (`ctest --preset test -L misja-3`) — powinien nadal
przechodzić, mimo że dopisałeś kod do tej samej funkcji `advance`.

## Dlaczego "Controller", a nie klasa

Moglibyśmy teraz zapakować `classify` i `toDiverterPosition` w klasę `Controller` z metodami. Na tym
etapie to niepotrzebna komplikacja: obie funkcje nie trzymają żadnego własnego stanu między
wywołaniami — dostają dane wejściowe, zwracają wynik, koniec. "Controller" to na razie wygodna nazwa
dla tej pary funkcji, nie sygnał, że trzeba je zamknąć w klasie. Ten sam wzorzec — decyzja jako
zestaw funkcji bez stanu — jest zresztą tym, jak wygląda moduł decyzyjny w prawdziwym, znacznie
większym projekcie tego symulatora.

## Częste błędy

- **Próg jako `<=` zamiast `<`** — dokładnie 500g ma być `Heavy`, nie `Light`. Test to jawnie
  sprawdza.
- **Zapomniane wyczyszczenie `plant.item` po dotarciu do strefy wyjściowej** — bez tego paczka
  "utknie" tym razem już na dobre, w `OutputLight`/`OutputHeavy`.
- **Zmiana sygnatury `advance`** — parametr `diverterPosition` już tam jest od Misji 3; nie musisz
  (i nie powinieneś) dodawać nowych parametrów ani zmieniać istniejących.

## Pytanie do zastanowienia

Co by się stało, gdyby `Plant::advance` samo, wewnątrz siebie, wywoływało `classify` i
`toDiverterPosition` zamiast dostawać gotowy `DiverterPosition` jako parametr? Czy to nadal byłby
podział "stan vs. decyzja"?

**Dalej:** [Misja 5: pętla sterowania](./05_petla_sterowania.md).
