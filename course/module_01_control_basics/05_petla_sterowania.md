# 1.5 Pętla sterowania

## Problem

Masz już wszystkie kawałki: `Plant` reprezentuje stan, Controller (`classify` +
`toDiverterPosition`) podejmuje decyzję, `advance` przesuwa paczkę o jeden krok, uwzględniając tę
decyzję. Ale do tej pory wywoływałeś to wszystko ręcznie, pojedynczo, w testach. Prawdziwy system
nie robi jednego kroku — powtarza ten sam cykl (obejrzyj stan → zdecyduj → działaj) raz za razem, aż
coś się skończy.

## Nowe elementy C++

**`for`** — pętla powtarzająca blok kodu określoną liczbę razy. Tu: "wykonaj jeden krok symulacji,
`tickCount` razy z rzędu".

```cpp
for (int i = 0; i < tickCount; ++i) {
    // jeden krok symulacji
}
```

Nic więcej nowego nie wprowadzamy w tej misji — cała trudność polega na poprawnym **złożeniu** już
znanych elementów w jednym miejscu.

## Co już masz gotowe

[`include/psm/loop.hpp`](../../include/psm/loop.hpp):

```cpp
void runTicks(Plant& plant, int tickCount);
```

[`src/loop.cpp`](../../src/loop.cpp) ma pusty szkielet z komentarzem `// TODO`.

## Co masz napisać

Wypełnij `runTicks` tak, żeby dla każdego z `tickCount` "ticków" wykonać:

1. jeśli `plant.item` ma wartość — policz `WeightClass` przez `classify(plant.item->mass)`, a
   następnie `DiverterPosition` przez `toDiverterPosition(...)` na tym wyniku,
2. wywołaj `advance(plant, tenDiverterPosition)` (użyj tej właśnie policzonej pozycji — jeśli
   `plant.item` jest puste, `advance` i tak nic nie zrobi, więc możesz podać dowolną wartość, np.
   `DiverterPosition::Straight`, gdy paczki nie ma).

Innymi słowy: `runTicks` **spina w jedną całość** to, co wcześniej testowałeś osobno — Controller i
`Plant::advance` — i robi to `tickCount` razy z rzędu.

## Self-check

```bash
ctest --preset test -L misja-5
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test tworzy `Plant` z jedną
750-gramową paczką, wywołuje `runTicks(plant, 4)` i sprawdza, że paczka dotarła do `OutputHeavy`, a
kolejny tick czyści ją z systemu.

## Wciąż "Controller", nie klasa

Nazywamy `classify` + `toDiverterPosition` razem "Controllerem" — to nazwa dla pary współpracujących
funkcji, nie zapowiedź klasy. Ten sam brak potrzeby enkapsulacji, o którym mówiliśmy przy `Plant` w
Misji 3, dotyczy też tutaj: żadna z tych funkcji nie ma własnego stanu do ochrony.

## Częste błędy

- **Wywołanie `classify`/`toDiverterPosition`, gdy `plant.item` jest puste** — `plant.item->mass`
  na pustym `std::optional` to niezdefiniowane zachowanie. Sprawdź `has_value()` najpierw.
- **Pętla `for` z błędnym warunkiem stopu** (`<=` zamiast `<`) — wykona się o jeden raz za dużo.
- **Wywołanie `advance` tylko raz, poza pętlą** — cały sens tej misji to *powtórzenie* kroku, a nie
  wykonanie go jednorazowo.

## Pytanie do zastanowienia

Test tej misji sprawdza tylko stan `Plant` po serii ticków — nie widzi, co dzieje się "w środku" po
drodze. Czy to problem, czy zamierzona cecha takiego testu?

**Dalej:** [Misja 6: pierwszy przebieg](./06_pierwszy_przebieg.md).
