🇵🇱 Polski | [🇬🇧 English](03_przepiecie_na_silnik.en.md)

# 3.3 Przejście na silnik

## Problem

`main()` nadal sam wywołuje funkcje związane z `Plant`, `Diverter` i sterownikiem. `Engine` wykonuje
już te operacje we właściwej kolejności, więc można usunąć powtórzony kod z programu konsolowego.

## Nowe elementy C++

W tej misji nie pojawiają się nowe elementy składni. Połączysz poznane już `Engine` i `describe` w
działający program, podobnie jak w misji 6 modułu 1, w której powstał Twój pierwszy `main()`.

## Co już masz gotowe

[`apps/simulator_cli/main.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/apps/simulator_cli/main.cpp) nadal tworzy własny `Plant` i
`Diverter` oraz ręcznie powtarza kolejność ticka. Ten kod zastąpisz wywołaniami `Engine`.

## Co masz napisać

Przepisz `main()` tak, żeby:

1. utworzyć `psm::Engine`,
2. dodać jedną paczkę przez `engine.spawnItem(...)`,
3. w pętli (np. 8 ticków) wywoływać `engine.step()` i wypisywać wynik przez
   `std::cout << psm::describe(result) << '\n';`.

Logika wyboru trasy, ustawiania dywertera i przesuwania paczki znajduje się już w `Engine::step()`.
Po zmianie `main()` powinien być krótszy.

## Moment, w którym `runTicks` przestaje być używany

Od tej misji program nie wywołuje już funkcji `runTicks` z pliku
[`src/loop.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/src/loop.cpp).
Jej rolę przejął `Engine`. Plik **zostaje** jednak w repozytorium, ponieważ usunięcie go wraz z
odpowiednią zmianą w `CMakeLists.txt` wykracza poza zakres tej misji.
W projektach często przez pewien czas pozostaje nieużywany, ale nadal poprawnie kompilowany kod.
Nie każdy zbędny fragment trzeba usuwać w tej samej zmianie, w której przestał być potrzebny.

## Sprawdź się

Ta misja nie ma osobnej etykiety `ctest`. Tak jak w misji 6 modułu 1, sprawdzisz wynik, uruchamiając
program:

```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

Sprawdź, czy wynik wygląda sensownie: paczka przechodzi przez kolejne strefy, w końcu dociera do
`OutputHeavy` albo `OutputLight`, a potem znika (`empty`).

## Koniec modułu: pełny zestaw testów

```bash
ctest --preset test
```

Oczekiwany wynik: przechodzą wszystkie testy od `misja-1` do `misja-4` oraz od `misja-6` do
`misja-11`. Ta misja, podobnie jak misja 6, nie ma osobnej etykiety.

## Zapisz swoją pracę

```bash
git status
git add <pliki które zmieniłeś>
git commit -m "..."
```

## Częste błędy

- **Wywołanie `describe` z niekompletnym `TickResult`:** `engine.step()` zwraca już kompletny wynik.
  Nie musisz (i nie powinieneś) tworzyć `TickResult` ręcznie w `main()`.
- **Pozostawienie starej logiki obok nowej:** `main()` po tej misji nie powinien już nigdzie
  odwoływać się bezpośrednio do `Plant`, `Diverter` ani `classify`/`toDiverterCommand`. Jeśli wciąż
  je widzisz w swoim `main.cpp`, coś zostało niedokończone.
- **Zapomniany `#include <psm/tick_result.hpp>`** (dla `psm::describe`) albo `<psm/engine.hpp>` (dla
  `psm::Engine`).

## Pytanie do zastanowienia

`runTicks` i dawna wersja `main()` osobno realizowały tę samą kolejność operacji. Co mogłoby się
stać, gdyby ktoś zmienił ją tylko w jednym z tych miejsc? W jaki sposób przeniesienie całego cyklu
do `Engine::step()` zapobiega takiej sytuacji?

## Koniec modułu 3

`Engine` jest teraz jedynym miejscem, które zna kolejność operacji w ticku. `main()` wywołuje tylko
`step()`. W kolejnych modułach rozbudujesz `Engine` o tryby pracy systemu, zabezpieczenia i napęd
taśmy, którego wprowadzenie odłożyliśmy w module 2.
