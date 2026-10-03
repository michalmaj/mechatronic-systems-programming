🇵🇱 Polski | [🇬🇧 English](03_przepiecie_na_silnik.en.md)

# 3.3 Przepięcie na silnik

## Problem

`main()` nadal sam wywołuje funkcje związane z `Plant`, `Diverter` i Controllerem. `Engine` zawiera
już tę samą kolejność, więc można usunąć duplikację z CLI.

## Nowe elementy C++

Żadnych nowych elementów składniowych — ta misja polega na **złożeniu** już poznanych elementów
(`Engine`, `describe`) w działający program, tak jak misja 6 w module 1 była Twoim pierwszym własnym
`main()`.

## Co już masz gotowe

[`apps/simulator_cli/main.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/apps/simulator_cli/main.cpp) nadal tworzy własny `Plant` i
`Diverter` oraz ręcznie powtarza kolejność ticka. Ten kod zastąpisz wywołaniami `Engine`.

## Co masz napisać

Przepisz `main()` tak, żeby:

1. stworzyć `psm::Engine`,
2. dodać jedną paczkę przez `engine.spawnItem(...)`,
3. w pętli (np. 8 ticków) wywoływać `engine.step()` i wypisywać wynik przez
   `std::cout << psm::describe(result) << '\n';`.

Logika wyboru trasy, ustawiania dywertera i przesuwania paczki znajduje się już w `Engine::step()`.
Po zmianie `main()` powinien być krótszy.

## Moment, w którym `runTicks` przestaje być używany

Od tej misji `runTicks` (w [`src/loop.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/src/loop.cpp)) nie ma już żadnego wywołania w
programie — `Engine` przejął jego rolę. Sam plik **zostaje** w repozytorium; jego fizyczne usunięcie
(i towarzysząca zmiana `CMakeLists.txt`) wykracza poza zakres tej misji i nie jest Twoim zadaniem.
Nieużywany, wciąż poprawnie skompilowany kod, który czeka na sprzątanie, to normalna, przejściowa
sytuacja w prawdziwych projektach — nie każda duplikacja znika tego samego dnia, w którym przestaje
być potrzebna.

## Sprawdź się

Ta misja nie ma osobnej etykiety `ctest` — tak jak w misji 6 modułu 1, prawdziwym testem jest
uruchomienie programu i przeczytanie wyniku:

```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

Sprawdź, czy wynik wygląda sensownie: paczka przechodzi przez kolejne strefy, w końcu dociera do
`OutputHeavy` albo `OutputLight`, a potem znika (`empty`).

## Koniec modułu — pełny zestaw testów

```bash
ctest --preset test
```

Oczekiwany wynik: wszystkie testy zielone — `misja-1` do `misja-4`, `misja-6` do `misja-11`. (Ta
misja, tak jak Misja 6, nie ma osobnej etykiety — patrz "Sprawdź się" wyżej.)

## Zapisz swoją pracę

```bash
git status
git add <pliki które zmieniłeś>
git commit -m "..."
```

## Częste błędy

- **Wywołanie `describe` z niekompletnym `TickResult`** — `engine.step()` zwraca już gotową,
  kompletną migawkę; nie musisz (i nie powinieneś) budować `TickResult` ręcznie w `main()`.
- **Pozostawienie starej logiki obok nowej** — `main()` po tej misji nie powinien już nigdzie
  odwoływać się bezpośrednio do `Plant`, `Diverter` ani `classify`/`toDiverterCommand`. Jeśli wciąż
  je widzisz w swoim `main.cpp`, coś zostało niedokończone.
- **Zapomniany `#include <psm/tick_result.hpp>`** (dla `psm::describe`) albo `<psm/engine.hpp>` (dla
  `psm::Engine`).

## Pytanie do zastanowienia

`runTicks` i `Engine::step()` implementują tę samą kolejność, ale jedno jest wolną funkcją operującą
na przekazanym `Plant&`, a drugie metodą klasy posiadającej własny `Plant`. Który z tych dwóch
projektów łatwiej byłoby dziś przypadkiem "rozjechać" z drugim, gdyby ktoś zmienił kolejność kroków
tylko w jednym miejscu? Dlaczego to jest właśnie ten problem, który `Engine` ostatecznie rozwiązuje?

## Koniec modułu 3

`Engine` jest teraz jedynym miejscem, które zna kolejność operacji w ticku. `main()` wywołuje tylko
`step()`. W kolejnych modułach `Engine` będzie
rósł: dołączą tryby pracy systemu, bezpieczeństwo i drugi aktuator (silnik przenośnika), którego
wprowadzenie odłożyliśmy jeszcze w module 2.
