# 3.2 Silnik formalizuje kolejność

To druga i najważniejsza teoretycznie misja tego modułu — poświęcimy jej trochę więcej miejsca.

## Problem

Kolejność jednego ticka z Misji 9 Modułu 2 jest poprawna, ale żyje wyłącznie z konwencji — jest ręcznie
powtórzona i w `runTicks`, i w `main()`. Otwórz [`src/loop.cpp`](../../src/loop.cpp) i zobacz tę
kolejność na własne oczy — to dokładnie ta sama sekwencja, którą za chwilę zamkniesz w jednym miejscu.
Nic dziś nie gwarantuje, że dwie osobne kopie tej sekwencji się nie rozjadą, i nic nie udowadnia, że
są ze sobą zgodne.

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
trzyma do nich dostępu. To inny sposób użycia `class` niż w Module 2, ale ten sam mechanizm
(enkapsulacja) i ta sama konwencja nazewnicza (§1 Module 2: "typ z ukrytym stanem i publicznym
interfejsem nazywamy `class`").

## Niezmiennik — dokładna wersja

**Fizyczne posuwanie się symulacji naprzód** (licznik `tick_`, ruch `Plant`, ustawianie się
`Diverter`) odbywa się **wyłącznie** wewnątrz `step()`, w ustalonej kolejności poniżej — nigdy
niezależnie, z zewnątrz `Engine`.

**`spawnItem()` jest jawnym wyjątkiem** — to nie jest "posuwanie symulacji naprzód", to sposób, w
jaki paczka w ogóle trafia do systemu. Wywołanie go między tickami jest w pełni dozwolone i
oczekiwane — dokładnie tak, jak robiłeś to w Module 1 i 2 z `spawnItem(Plant&, Item)`.

## Kolejność `step()` — dokładnie, krok po kroku

1. Jeśli `plant_`'s aktualna paczka ma wartość: zdecyduj jej `DiverterCommand` przez wolne funkcje
   `classify` i `toDiverterCommand` (`Engine` **nie posiada** Controllera — nie ma żadnego obiektu
   Controller, są tylko bezstanowe funkcje, które `step()` po prostu wywołuje, jako pierwszy krok,
   przed `setCommand`).
2. `diverter_.setCommand(...)`.
3. `diverter_.resolve()`.
4. `psm::advance(plant_, diverter_)` — **wolna funkcja** z Modułu 1/2, nie metoda. `Plant` to wciąż
   zwykły `struct`, bez metod — z tego samego powodu `Engine::spawnItem` musi wywołać wolną funkcję
   `psm::spawnItem(plant_, item)`, **nigdy** `plant_.spawnItem(item)` (taka metoda po prostu nie
   istnieje).
5. Złóż i zwróć `TickResult` dla ticku, który właśnie przetworzyłeś — numer ticku to `tick_`
   **sprzed** inkrementacji — a dopiero potem zwiększ `tick_`.

## Zamrożona semantyka numeracji

Pierwsze wywołanie `step()` zwraca `tick == 0`. `TickResult` opisuje stan **po** przetworzeniu tego
ticku — pierwsze wywołanie już odzwierciedla pełny cykl decyzja→`setCommand`→`resolve`→`advance`, nie
stan sprzed niego.

## Co już masz gotowe

[`include/psm/engine.hpp`](../../include/psm/engine.hpp) — deklaracje kompletne, dokładnie jak wyżej.

[`src/engine.cpp`](../../src/engine.cpp) — puste szkielety obu metod z komentarzami `// TODO`.

## Co masz napisać

- `Engine::spawnItem(Item)` — jedna linijka: wywołanie wolnej funkcji `psm::spawnItem(plant_, item)`.
- `Engine::step()` — pięć kroków opisanych wyżej, w tej dokładnie kolejności.

## Self-check

```bash
ctest --preset test -L misja-11
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test wykonuje sześć kolejnych
`step()` na 750-gramowej paczce i sprawdza numer ticku, strefę paczki oraz `diverterCommand`/
`diverterActual` na kilku z nich — to te same zależności, które już widziałeś w testach Modułu 2,
teraz zweryfikowane przez jedno wywołanie zamiast czterech ręcznie poukładanych.

## Częste błędy

- **Zamieniona kolejność kroku 3 i 4** — `resolve()` musi się wykonać przed `advance()`, dokładnie
  jak w Misji 9 Modułu 2. Zamiana o jeden krok przesuwa każdą decyzję o routingu.
- **Wywołanie `plant_.spawnItem(item)` albo `plant_.advance(diverter_)`** — `Plant` nie ma takich
  metod. Kompilator to złapie, ale komunikat błędu bywa nieoczywisty przy pierwszym spotkaniu.
- **Numer ticku w `TickResult` po inkrementacji zamiast przed** — pierwszy `step()` zwróciłby wtedy
  `tick == 1`, nie `tick == 0`, łamiąc zamrożoną semantykę numeracji.

## Pytanie do zastanowienia

`Engine` "posiada" `Plant` i `Diverter` jako zwykłe pola, nie referencje. Co by się zmieniło, gdyby
zamiast tego `Engine` przechowywał referencje do `Plant`/`Diverter` utworzonych gdzieś indziej? Czy
`Engine::spawnItem`/`step()` nadal miałyby sens?

**Dalej:** [Misja 12: przepięcie na silnik](./03_przepiecie_na_silnik.md).
