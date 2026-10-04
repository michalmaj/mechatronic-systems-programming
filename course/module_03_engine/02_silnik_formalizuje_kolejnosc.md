🇵🇱 Polski | [🇬🇧 English](02_silnik_formalizuje_kolejnosc.en.md)

# 3.2 Silnik porządkuje kolejność

W tej misji powstaje główna klasa modułu, dlatego opis jest nieco dłuższy.

## Problem

Kolejność ticka z misji 9 jest zapisana osobno w `runTicks` i w `main()`. Otwórz
[`src/loop.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/src/loop.cpp)
i porównaj ją z kodem programu konsolowego. Za chwilę przeniesiesz tę sekwencję do jednego miejsca,
aby kolejne zmiany nie wymagały poprawiania dwóch kopii kodu.

## Nowy element C++: kompozycja obiektów

`Diverter` z modułu 2 chronił swój stan przed niepoprawną zmianą. `Engine` pokazuje inne
zastosowanie klasy, czyli **kompozycję**. Zawiera obiekty `Plant` i `Diverter` jako zwykłe pola
składowe, a nie referencje lub wskaźniki:

```cpp
class Engine {
public:
    void spawnItem(Item item);
    TickResult step();

private:
    Tick tick_ = 0;
    Plant plant_;
    Diverter diverter_;
};
```

Pola `plant_` i `diverter_` są tworzone i niszczone razem z obiektem `Engine`. Kod spoza tej klasy
nie ma do nich bezpośredniego dostępu. Jest to inne zastosowanie `class` niż w module 2, ale nadal
korzysta z tego samego mechanizmu enkapsulacji: stan pozostaje ukryty za publicznymi metodami.

## Chroniony niezmiennik

**Zmiana stanu symulacji** obejmuje zwiększenie licznika `tick_`, ruch paczki w `Plant` oraz zmianę
położenia `Diverter`. Wszystkie te operacje odbywają się **wyłącznie** wewnątrz `step()` i w
ustalonej kolejności. Nie można wywołać ich niezależnie z zewnątrz `Engine`.

`spawnItem()` jest wyjątkiem od tej reguły, ponieważ dodaje paczkę między tickami, ale nie wykonuje
kroku symulacji. Tak samo działała wcześniej funkcja `spawnItem(Plant&, Item)`.

## Kolejność operacji w `step()`

1. Jeśli `plant_` zawiera paczkę, wyznacz dla niej `DiverterCommand` za pomocą wolnych funkcji
   `classify` i `toDiverterCommand`. `Engine` nie zawiera osobnego obiektu sterownika. Te dwie
   funkcje nie przechowują stanu, a `step()` wywołuje je przed `setCommand`.
2. `diverter_.setCommand(...)`.
3. `diverter_.resolve()`.
4. `psm::advance(plant_, diverter_)`. Jest to **wolna funkcja** z modułów 1 i 2, a nie metoda.
   `Plant` pozostaje zwykłym `struct` bez metod. Z tego samego powodu `Engine::spawnItem` musi
   wywołać `psm::spawnItem(plant_, item)`, a nie nieistniejące `plant_.spawnItem(item)`.
5. Utwórz `TickResult` dla właśnie przetworzonego ticku i zapisz w nim bieżącą wartość `tick_`.
   Następnie zwiększ `tick_` i zwróć przygotowany wynik.

## Reguła numeracji

Pierwsze wywołanie `step()` zwraca `tick == 0`. `TickResult` opisuje stan **po** przetworzeniu tego
ticku. Odzwierciedla więc pełny cykl decyzja → `setCommand` → `resolve` → `advance`, a nie stan
sprzed rozpoczęcia cyklu.

## Co już masz gotowe

[`include/psm/engine.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/include/psm/engine.hpp)
zawiera kompletne deklaracje pokazane wyżej.

W pliku
[`src/engine.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/src/engine.cpp)
znajdziesz puste szkielety obu metod z komentarzami `// TODO`.

## Co masz napisać

- `Engine::spawnItem(Item)`: wywołaj wolną funkcję `psm::spawnItem(plant_, item)`.
- `Engine::step()`: wykonaj pięć opisanych wyżej kroków w podanej kolejności.

## Sprawdź się

```bash
ctest --preset test -L misja-11
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test wykonuje sześć kolejnych
`step()` na 750-gramowej paczce i sprawdza numer ticku, strefę paczki oraz `diverterCommand`/
`diverterActual` po kilku krokach. Sprawdza więc te same zależności co testy modułu 2, ale tym razem
do wykonania całego cyklu wystarcza jedno wywołanie.

## Częste błędy

- **Zamieniona kolejność kroku 3 i 4:** `resolve()` musi się wykonać przed `advance()`, tak jak w
  misji 9. Po odwróceniu kolejności paczka trafiałaby do strefy wyjściowej o jeden tick później.
- **Wywołanie `plant_.spawnItem(item)` albo `plant_.advance(diverter_)`:** `Plant` nie ma takich
  metod. Kompilator zgłosi błąd, ale jego komunikat może być początkowo niejasny.
- **Numer ticku w `TickResult` pobrany po zwiększeniu licznika zamiast przed:** pierwszy `step()`
  zwróciłby wtedy `tick == 1`, a nie `tick == 0`, co złamałoby opisaną wyżej regułę numeracji.

## Pytanie do zastanowienia

`Engine` zawiera `Plant` i `Diverter` jako zwykłe pola, a nie referencje. Co zmieniłoby się, gdyby
przechowywał referencje do obiektów utworzonych gdzieś indziej? Czy metody `Engine::spawnItem` i
`Engine::step()` nadal miałyby sens?

**Dalej:** [Misja 12: przejście na silnik](./03_przepiecie_na_silnik.md).
