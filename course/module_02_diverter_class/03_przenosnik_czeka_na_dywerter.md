# 2.3 Przenośnik czeka na dywerter

## Problem

Masz już `DiverterCommand` (Misja 7) i działający `Diverter` (Misja 8). Ale `Plant` wciąż nic o nich
nie wie — w Module 1 dostawał gotową pozycję i natychmiast kierował paczkę. Teraz, gdy rozjazd może
potrzebować kilku ticków, żeby się ustawić, `Plant` musi to uszanować: jeśli paczka dotarła do
`Diverting`, a dywerter jeszcze się nie ustawił, paczka **czeka**.

## Nowe elementy C++

**Przekazywanie obiektu klasy przez `const&`** — `Plant::advance` przyjmuje teraz
`const Diverter& diverter` zamiast surowej wartości `DiverterPosition`. `const&` oznacza: dostęp do
tego konkretnego obiektu, bez kopiowania, i bez prawa go zmieniać — `Plant` tylko **pyta** dywerter o
jego stan, nigdy go nie modyfikuje.

**Wywoływanie metod składowych** — zamiast porównywać surową wartość enuma, wywołujesz
`diverter.isSettled()` i `diverter.actualPosition()`.

## Minimalna kolejność jednego ticka

To jeszcze nie docelowy silnik symulacji (do tego dojdziemy w dalszych modułach) — ale coś musi
ustalić kolejność, inaczej testy staną się niejednoznaczne. W tym module obowiązuje dokładnie taka
kolejność, używana zarówno przez `runTicks`, jak i przez `main()`:

1. **Decyzja Controllera** — jeśli `plant.item` ma wartość: `classify(mass)`, potem
   `toDiverterCommand(...)`.
2. **`diverter.setCommand(...)`** — powiedz dywerterowi, czego teraz chcemy.
3. **`diverter.resolve()`** — dywerter robi swój jeden fizyczny krok w tym ticku.
4. **`advance(plant, diverter)`** — `Plant` reaguje na stan dywertera **po** tegorocznym `resolve()`,
   nie sprzed niego.
5. **Obserwacja** (tylko w `main()`) — wypisanie wyniku; `runTicks` pozostaje czystą funkcją bez
   wypisywania, tak jak w Module 1.

Kolejność kroków 3 i 4 nie jest przypadkowa: gdybyś je zamienił, `Plant::advance` widziałby pozycję
dywertera sprzed tego ticku zamiast aktualnej — każda decyzja o routingu przesunęłaby się cicho o
jeden tick.

## Co już masz gotowe

W [`src/plant.cpp`](../../src/plant.cpp) gałęzie `Infeed`/`PresenceCheck`/`Weighing` (przesuwanie
przez `advanceZone`) oraz `OutputLight`/`OutputHeavy` (czyszczenie `plant.item`) są już gotowe i
działają — to logika z Module 1, niezmieniona. Brakuje tylko gałęzi `Diverting`:

```cpp
case Zone::Diverting:
    // TODO (Misja 9: przenosnik_czeka_na_dywerter): jeśli !diverter.isSettled(), paczka
    // czeka (nic nie rób -- zostaje w Diverting). Jeśli diverter.isSettled(), skieruj
    // paczkę do OutputLight albo OutputHeavy na podstawie diverter.actualPosition().
    (void)diverter;
    return;
```

`src/loop.cpp` i `apps/simulator_cli/main.cpp` mają puste szkielety całych funkcji — obie wymagają
teraz własnego, lokalnego obiektu `Diverter`, prowadzonego zgodnie z kolejnością opisaną wyżej.

## Co masz napisać

1. **Gałąź `Diverting` w `src/plant.cpp`** — zaimplementuj dokładnie to, co mówi komentarz TODO.
2. **`runTicks` w `src/loop.cpp`** — dla każdego z `tickCount` ticków wykonaj pięć kroków z sekcji
   "Minimalna kolejność" (bez kroku 5 — `runTicks` nic nie wypisuje).
3. **`main()` w `apps/simulator_cli/main.cpp`** — stwórz `Plant` i `Diverter`, dodaj paczkę przez
   `spawnItem`, i w pętli (np. 8 ticków) wykonaj wszystkie pięć kroków, wypisując numer ticku, strefę
   paczki (`psm::toString`) i `diverter.actualPosition()`.

## Dlaczego dostarczony test wymaga spóźnionego polecenia

Jeśli Controller decyduje **od razu**, od ticku 0 (tak jak robi to `runTicks`), dywerter zawsze zdąży
się ustawić, zanim paczka w ogóle dotrze do `Diverting` — 3 tiki podróży zawsze starczą na 2 tiki
ustawiania się. W takim scenariuszu oczekiwanie nigdy nie jest widoczne z zewnątrz! Dlatego
dostarczony test, oprócz sprawdzenia zwykłej, "chętnej" ścieżki (dokładnie tej samej matematyki co w
Module 1: 4 tiki do `OutputHeavy`), zawiera też scenariusz, w którym polecenie `Divert` wydawane jest
**dopiero, gdy paczka już czeka w `Diverting`** — to jedyny sposób, żeby naprawdę zobaczyć, jak
przenośnik czeka na urządzenie.

## Self-check

```bash
ctest --preset test -L misja-9
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`.

Uruchom też program naprawdę:

```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

## Koniec modułu — pełny zestaw testów

```bash
ctest --preset test
```

Oczekiwany wynik: wszystkie testy zielone — `misja-1` do `misja-4`, `misja-6` do `misja-9`.

## Zapisz swoją pracę

```bash
git status
git add <pliki które zmieniłeś>
git commit -m "..."
```

## Częste błędy

- **Zamieniona kolejność `resolve()`/`advance()`** — patrz sekcja o kolejności ticka wyżej; to
  najłatwiejszy błąd do popełnienia w tej misji i jedyny, który zmieni wyniki o dokładnie jeden tick.
- **`runTicks` bez własnego `Diverter`** — dywerter musi żyć przez cały czas trwania pętli (żeby
  pamiętać swój stan między tickami), nie może być tworzony od nowa w każdej iteracji.
- **Sprawdzanie `diverter.isSettled()` przed `resolve()`** zamiast po — pamiętaj, `Plant::advance`
  patrzy na stan dywertera **po** tegorocznym kroku.

## Pytanie do zastanowienia

Dostarczony test celowo używa dwóch różnych scenariuszy — "chętnego" i "spóźnionego" — żeby w ogóle
zaobserwować oczekiwanie. Czy sam test integracyjny oparty tylko o `runTicks` (który zawsze decyduje
chętnie) wystarczyłby, żeby wykryć błąd w Twojej implementacji gałęzi `Diverting`? Dlaczego tak albo
dlaczego nie?

## Koniec modułu 2

Masz teraz system, w którym fizyczne urządzenie naprawdę ma swój własny czas reakcji, a przenośnik go
respektuje. To dokładnie ten sam krok, który ten projekt przeszedł na samym początku swojej
historii — w kolejnych modułach dołączą kolejne elementy: drugi aktuator (silnik przenośnika), tryby
pracy systemu i bezpieczeństwo.
