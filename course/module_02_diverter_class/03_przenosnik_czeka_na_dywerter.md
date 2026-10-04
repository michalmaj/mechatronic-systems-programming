🇵🇱 Polski | [🇬🇧 English](03_przenosnik_czeka_na_dywerter.en.md)

# 2.3 Przenośnik czeka na dywerter

## Problem

Masz już `DiverterCommand` (Misja 7) i działający `Diverter` (Misja 8). Ale `Plant` wciąż nic o nich
nie wie. W module 1 dostawał gotową pozycję i natychmiast kierował paczkę. Teraz rozjazd może
potrzebować kilku ticków, żeby się ustawić, więc `Plant` musi uwzględnić czas jego ruchu. Jeśli
paczka dotarła do `Diverting`, a dywerter jeszcze się nie ustawił, paczka **czeka**.

## Nowe elementy C++

**Przekazywanie obiektu klasy przez `const&`.** `Plant::advance` przyjmuje teraz
`const Diverter& diverter` zamiast surowej wartości `DiverterPosition`. `const&` oznacza: dostęp do
tego konkretnego obiektu, bez kopiowania i bez prawa jego zmiany. `Plant` tylko **pyta** dywerter o
jego stan, nigdy go nie modyfikuje.

**Wywoływanie metod składowych.** Zamiast porównywać surową wartość typu wyliczeniowego, wywołujesz
`diverter.isSettled()` i `diverter.actualPosition()`.

## Kolejność operacji w jednym ticku

Nie mamy jeszcze docelowego silnika symulacji, ale musimy ustalić jednoznaczną kolejność operacji.
Zarówno `runTicks`, jak i `main()` wykonują w tym module następujące kroki:

1. **Decyzja sterownika:** jeśli `plant.item` ma wartość, wywołaj `classify(mass)`, a potem
   `toDiverterCommand(...)`.
2. **`diverter.setCommand(...)`:** przekaż dywerterowi wybrane polecenie.
3. **`diverter.resolve()`:** dywerter wykonuje jeden fizyczny krok w tym ticku.
4. **`advance(plant, diverter)`:** `Plant` reaguje na stan dywertera **po** wywołaniu `resolve()`,
   nie sprzed niego.
5. **Wypisanie wyniku** (tylko w `main()`): `runTicks`, tak jak w module 1, nie wypisuje nic na
   konsolę.

Kolejność kroków 3 i 4 nie jest przypadkowa: gdybyś je zamienił, `Plant::advance` widziałby pozycję
dywertera sprzed tego ticku zamiast bieżącej. Paczka trafiałaby do strefy wyjściowej o jeden tick
później.

## Co już masz gotowe

W [`src/plant.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-02-start/src/plant.cpp) gałęzie `Infeed`/`PresenceCheck`/`Weighing` (przesuwanie
przez `advanceZone`) oraz `OutputLight`/`OutputHeavy` (czyszczenie `plant.item`) są już gotowe i
działają. To niezmieniona logika z modułu 1. Brakuje tylko gałęzi `Diverting`:

```cpp
case Zone::Diverting:
    // TODO (Misja 9: przenosnik_czeka_na_dywerter): jeśli !diverter.isSettled(), paczka
    // czeka (nic nie rób -- zostaje w Diverting). Jeśli diverter.isSettled(), skieruj
    // paczkę do OutputLight albo OutputHeavy na podstawie diverter.actualPosition().
    (void)diverter;
    return;
```

`src/loop.cpp` i `apps/simulator_cli/main.cpp` mają puste szkielety całych funkcji. W obu trzeba
utworzyć własny obiekt `Diverter` i wykonać operacje w opisanej wyżej kolejności.

## Co masz napisać

1. **Gałąź `Diverting` w `src/plant.cpp`:** zaimplementuj zachowanie opisane w komentarzu TODO.
2. **`runTicks` w `src/loop.cpp`:** dla każdego z `tickCount` ticków wykonaj pierwsze cztery kroki z
   sekcji „Kolejność operacji w jednym ticku”. `runTicks` nic nie wypisuje.
3. **`main()` w `apps/simulator_cli/main.cpp`:** utwórz `Plant` i `Diverter`, dodaj paczkę przez
   `spawnItem`, i w pętli (np. 8 ticków) wykonaj wszystkie pięć kroków, wypisując numer ticku, strefę
   paczki (`psm::toString`) i `diverter.actualPosition()`.

## Dlaczego test wymaga polecenia wydanego z opóźnieniem

Jeśli sterownik wydaje polecenie **od razu**, już w ticku 0 (tak jak robi to `runTicks`), dywerter
zawsze zdąży się ustawić, zanim paczka dotrze do `Diverting`. Paczka potrzebuje na to trzech
ticków, a dywerter ustawia się w dwa. W takim scenariuszu oczekiwanie nigdy nie jest widoczne z
zewnątrz. Dlatego test, oprócz zwykłej ścieżki (4 ticki do `OutputHeavy`, jak w module 1), zawiera
też scenariusz, w którym sterownik wydaje polecenie `Divert` **dopiero wtedy, gdy paczka czeka już w
`Diverting`**. Tylko w ten sposób można sprawdzić, czy przenośnik czeka na urządzenie.

## Sprawdź się

```bash
ctest --preset test -L misja-9
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`.

Uruchom też program:

```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

## Koniec modułu: pełny zestaw testów

```bash
ctest --preset test
```

Oczekiwany wynik: przechodzą wszystkie testy od `misja-1` do `misja-4` oraz od `misja-6` do `misja-9`.

## Zapisz swoją pracę

```bash
git status
git add <pliki które zmieniłeś>
git commit -m "..."
```

## Częste błędy

- **Zamieniona kolejność `resolve()` i `advance()`:** spójrz ponownie na kolejność ticka. Taki błąd
  przesunie wyniki o jeden tick.
- **`runTicks` bez własnego `Diverter`:** dywerter musi istnieć przez cały czas trwania pętli, aby
  pamiętać swój stan między tickami. Nie może być tworzony od nowa w każdej iteracji.
- **Sprawdzanie `diverter.isSettled()` przed `resolve()`** zamiast po: `Plant::advance`
  patrzy na stan dywertera **po** tym kroku.

## Pytanie do zastanowienia

Test używa dwóch scenariuszy: z poleceniem wydanym wcześnie i z poleceniem spóźnionym. Czy test
integracyjny oparty wyłącznie na `runTicks`, które zawsze wydaje polecenie wcześnie, wykryłby błąd w
gałęzi `Diverting`? Dlaczego?

## Koniec modułu 2

Masz teraz system, w którym fizyczne urządzenie ma własny czas reakcji, a przenośnik musi go
uwzględnić. W kolejnych modułach dołączą następne elementy: drugi aktuator (silnik przenośnika),
tryby pracy systemu i bezpieczeństwo.
