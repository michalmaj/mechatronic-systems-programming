🇵🇱 Polski | [🇬🇧 English](02_silnik_formalizuje_kolejnosc.en.md)

# 3.2 Silnik formalizuje kolejność

W tej misji powstaje główna klasa modułu, dlatego opis jest nieco dłuższy.

## Problem

Kolejność ticka z misji 9 jest zapisana osobno w `runTicks` i w `main()`. Otwórz
[`src/loop.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/src/loop.cpp)
i porównaj ją z kodem CLI. Za chwilę przeniesiesz tę sekwencję do jednego miejsca, aby następne
zmiany nie wymagały utrzymywania dwóch kopii.

## Nowy element C++: druga klasa, tym razem **kompozycja**

`Diverter` (Moduł 2) chronił jedno pole przed niepoprawną zmianą. `Engine` to inny rodzaj klasy —
demonstruje **kompozycję**: `Engine` *posiada* `Plant` i `Diverter` jako zwykłe pola składowe, nie
referencje ani wskaźniki:

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

`plant_` i `diverter_` żyją i umierają razem z `Engine` — nie istnieją nigdzie indziej, nikt inny nie
trzyma do nich dostępu. To inny sposób użycia `class` niż w module 2, ale ten sam mechanizm
(enkapsulacja) i ta sama konwencja nazewnicza (§1 Moduł 2: "typ z ukrytym stanem i publicznym
interfejsem nazywamy `class`").

## Chroniony niezmiennik

**Fizyczne posuwanie się symulacji naprzód** (licznik `tick_`, ruch `Plant`, ustawianie się
`Diverter`) odbywa się **wyłącznie** wewnątrz `step()`, w ustalonej kolejności poniżej — nigdy
niezależnie, z zewnątrz `Engine`.

`spawnItem()` jest wyjątkiem od tej reguły, ponieważ dodaje paczkę między tickami, ale nie wykonuje
kroku symulacji. Tak samo działała wcześniej funkcja `spawnItem(Plant&, Item)`.

## Kolejność `step()` — krok po kroku

1. Jeśli `plant_` zawiera paczkę, wyznacz dla niej `DiverterCommand` za pomocą wolnych funkcji
   `classify` i `toDiverterCommand` (`Engine` **nie posiada** Controllera — nie ma żadnego obiektu
   Controller, są tylko funkcje bez stanu, które `step()` wywołuje jako pierwszy krok,
   przed `setCommand`).
2. `diverter_.setCommand(...)`.
3. `diverter_.resolve()`.
4. `psm::advance(plant_, diverter_)` — **wolna funkcja** z modułów 1 i 2, nie metoda. `Plant` to wciąż
   zwykły `struct`, bez metod — z tego samego powodu `Engine::spawnItem` musi wywołać wolną funkcję
   `psm::spawnItem(plant_, item)`, a nie `plant_.spawnItem(item)` (taka metoda nie
   istnieje).
5. Złóż i zwróć `TickResult` dla ticku, który właśnie przetworzyłeś — numer ticku to `tick_`
   **sprzed** inkrementacji — a dopiero potem zwiększ `tick_`.

## Reguła numeracji

Pierwsze wywołanie `step()` zwraca `tick == 0`. `TickResult` opisuje stan **po** przetworzeniu tego
ticku — pierwsze wywołanie już odzwierciedla pełny cykl decyzja→`setCommand`→`resolve`→`advance`, nie
stan sprzed niego.

## Co już masz gotowe

[`include/psm/engine.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/include/psm/engine.hpp) — deklaracje kompletne, jak wyżej.

[`src/engine.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/src/engine.cpp) — puste szkielety obu metod z komentarzami `// TODO`.

## Co masz napisać

- `Engine::spawnItem(Item)` — jedna linijka: wywołanie wolnej funkcji `psm::spawnItem(plant_, item)`.
- `Engine::step()` — pięć kroków opisanych wyżej, w podanej kolejności.

## Sprawdź się

```bash
ctest --preset test -L misja-11
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test wykonuje sześć kolejnych
`step()` na 750-gramowej paczce i sprawdza numer ticku, strefę paczki oraz `diverterCommand`/
`diverterActual` na kilku z nich — to te same zależności, które już widziałeś w testach modułu 2,
teraz zweryfikowane przez jedno wywołanie zamiast czterech ręcznie poukładanych.

## Częste błędy

- **Zamieniona kolejność kroku 3 i 4** — `resolve()` musi się wykonać przed `advance()`, tak jak w
  misji 9. Odwrócenie kolejności opóźnia każdą decyzję o trasie o jeden tick.
- **Wywołanie `plant_.spawnItem(item)` albo `plant_.advance(diverter_)`** — `Plant` nie ma takich
  metod. Kompilator to złapie, ale komunikat błędu bywa nieoczywisty przy pierwszym spotkaniu.
- **Numer ticku w `TickResult` po inkrementacji zamiast przed** — pierwszy `step()` zwróciłby wtedy
  `tick == 1`, nie `tick == 0`, łamiąc regułę numeracji opisaną wyżej.

## Pytanie do zastanowienia

`Engine` "posiada" `Plant` i `Diverter` jako zwykłe pola, nie referencje. Co by się zmieniło, gdyby
zamiast tego `Engine` przechowywał referencje do `Plant`/`Diverter` utworzonych gdzieś indziej? Czy
`Engine::spawnItem`/`step()` nadal miałyby sens?

**Dalej:** [Misja 12: przepięcie na silnik](./03_przepiecie_na_silnik.md).
