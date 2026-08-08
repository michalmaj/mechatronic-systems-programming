# 1.2 Ruch paczki

## Problem

W Misji 1 nauczyłeś się nazywać strefy i opisywać jedną paczkę. Ale sama paczka się nie rusza —
potrzebujesz czegoś, co weźmie paczkę stojącą w jednej strefie i przesunie ją do następnej.

Kolejność stref jest ustalona: `Infeed` → `PresenceCheck` → `Weighing` → `Diverting`. Dalej,
w `Diverting`, paczka na razie utknie — i to jest **celowe**. Rozjazd musi jeszcze zdecydować,
w którą stronę skierować paczkę, a o decyzjach zajmiemy się dopiero w Misji 4. Na razie interesuje
Cię wyłącznie ruch "o jedną strefę do przodu".

## Nowe elementy C++

**Funkcja wolna (nie metoda)**, przyjmująca `Item&` — referencję do paczki, którą ma zmienić.
Referencja (`&`) oznacza, że funkcja nie dostaje kopii paczki, tylko dostęp do tej samej, oryginalnej
zmiennej — zmiany, które w niej zrobi, są widoczne także po wyjściu z funkcji.

```cpp
void advanceZone(Item& item);
```

Gdyby parametr był typu `Item` (bez `&`), funkcja dostałaby kopię — mogłaby ją dowolnie zmieniać,
ale oryginalna paczka u wywołującego pozostałaby nietknięta. Tu właśnie chodzi o odwrotność: chcemy
zmienić stan istniejącej paczki.

**`switch` nad `enum class`** — już go widziałeś w Misji 1 przy zamianie na tekst; tutaj używasz go
do podjęcia decyzji "co jest następną strefą", zamiast do zwrócenia napisu.

## Co już masz gotowe

[`include/psm/item_motion.hpp`](../../include/psm/item_motion.hpp) deklaruje funkcję, którą
uzupełnisz:

```cpp
void advanceZone(Item& item);
```

[`src/item_motion.cpp`](../../src/item_motion.cpp) zawiera jej pusty szkielet:

```cpp
void advanceZone(Item& item) {
    // TODO (Misja 2: ruch_paczki): przesuń item.zone o jedną strefę do przodu
    // (Infeed -> PresenceCheck -> Weighing -> Diverting). Na Diverting zatrzymaj się --
    // co dalej, to już Misja 4.
    (void)item;
}
```

## Co masz napisać

Wypełnij ciało `advanceZone` tak, żeby:
- `Infeed` zmieniało się na `PresenceCheck`,
- `PresenceCheck` zmieniało się na `Weighing`,
- `Weighing` zmieniało się na `Diverting`,
- `Diverting` **zostawało bez zmian** (paczka czeka na decyzję, której jeszcze nie umiemy podjąć).

`switch` nad `item.zone`, przypisujący nową wartość do `item.zone` w każdym przypadku, jest
naturalnym narzędziem do tego zadania.

## Self-check

```bash
ctest --preset test -L misja-2
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test wywołuje `advanceZone`
czterokrotnie na jednej paczce i sprawdza, że po trzecim wywołaniu paczka jest w `Diverting`, a po
czwartym — nadal w `Diverting`.

## Częste błędy

- **Brak przypadku dla `Diverting`** — jeśli go pominiesz, `switch` bez `default` po prostu nic nie
  zrobi dla tej wartości, co akurat tutaj daje poprawny efekt, ale lepiej dodać go jawnie
  (`case Zone::Diverting: break;`), żeby było widać, że to świadoma decyzja, a nie przeoczenie.
- **Zmiana `item` zamiast `item.zone`** — pamiętaj, że modyfikujesz tylko pole `zone`, nie całą
  paczkę (`id` i `mass` mają zostać bez zmian).
- **Próba "przeskoczenia" `Diverting` od razu do strefy wyjściowej** — to nie jest zadanie tej misji;
  taka pokusa to dobry sygnał, że dotknąłeś problemu, który rozwiąże dopiero Misja 4.

## Pytanie do zastanowienia

Paczka utyka w `Diverting`. Czy to błąd w Twoim kodzie, czy poprawne zachowanie na obecnym etapie
projektu? Skąd to wiadomo?

**Dalej:** [Misja 3: stan przenośnika](./03_stan_przenosnika.md).
