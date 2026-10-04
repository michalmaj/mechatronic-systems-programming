🇵🇱 Polski | [🇬🇧 English](05_petla_sterowania.en.md)

# 1.5 Pętla sterowania

## Problem

Masz już wszystkie potrzebne elementy: `Plant` reprezentuje stan, sterownik (`classify` i
`toDiverterPosition`) podejmuje decyzję, `advance` przesuwa paczkę o jeden krok, uwzględniając tę
decyzję. Do tej pory wywoływałeś jednak każdą z tych funkcji osobno w testach. Działający system
powtarza cały cykl: sprawdza stan, podejmuje decyzję i wykonuje działanie.

## Nowe elementy C++

**`for`** to pętla powtarzająca blok kodu określoną liczbę razy. Tutaj służy do wykonania
`tickCount` kolejnych kroków symulacji.

```cpp
for (int i = 0; i < tickCount; ++i) {
    // jeden krok symulacji
}
```

W tej misji nie pojawią się inne nowe elementy języka. Zadanie polega na poprawnym **złożeniu**
poznanych funkcji w jednym miejscu.

## Co już masz gotowe

[`include/psm/loop.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/include/psm/loop.hpp):

```cpp
void runTicks(Plant& plant, int tickCount);
```

[`src/loop.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/src/loop.cpp) ma pusty szkielet z komentarzem `// TODO`.

## Co masz napisać

Wypełnij `runTicks` tak, żeby dla każdego z `tickCount` ticków wykonać:

1. jeśli `plant.item` ma wartość, policz `WeightClass` przez `classify(plant.item->mass)`, a
   następnie `DiverterPosition` przez `toDiverterPosition(...)` na tym wyniku,
2. wywołaj `advance(plant, diverterPosition)`, gdzie `diverterPosition` to ta właśnie policzona
   wartość (jeśli `plant.item` jest puste, `advance` i tak nic nie zrobi, więc możesz podać
   dowolną wartość, np. `DiverterPosition::Straight`, gdy paczki nie ma).

Funkcja `runTicks` łączy sterownik z `Plant::advance` i powtarza ten krok `tickCount` razy.

## Sprawdź się

```bash
ctest --preset test -L misja-5
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test tworzy `Plant` z jedną
750-gramową paczką, wywołuje `runTicks(plant, 4)` i sprawdza, że paczka dotarła do `OutputHeavy`, a
kolejny tick czyści ją z systemu.

## Dlaczego sterownik nadal nie jest klasą

Sterownik obejmuje dwie współpracujące funkcje: `classify` i `toDiverterPosition`. Żadna z
nich nie przechowuje stanu, więc klasa z prywatnymi polami niczego by tu nie chroniła.

## Częste błędy

- **Wywołanie `classify`/`toDiverterPosition`, gdy `plant.item` jest puste:** `plant.item->mass`
  na pustym `std::optional` to niezdefiniowane zachowanie. Sprawdź `has_value()` najpierw.
- **Pętla `for` z błędnym warunkiem stopu** (`<=` zamiast `<`): wykona się o jeden raz za dużo.
- **Wywołanie `advance` tylko raz, poza pętlą:** zadanie wymaga *powtarzania* kroku, a nie
  wykonania go jednorazowo.

## Pytanie do zastanowienia

Test tej misji sprawdza tylko stan `Plant` po serii ticków. Nie widzi, co dzieje się „w środku” po
drodze. Czy to problem, czy zamierzona cecha takiego testu?

**Dalej:** [Misja 6: pierwszy przebieg](./06_pierwszy_przebieg.md).
