🇵🇱 Polski | [🇬🇧 English](04_silnik_z_wieloma_paczkami.en.md)

# 8.4 Silnik z wieloma paczkami

## Problem

Wszystkie kawałki istnieją osobno — nowy `Plant`, `advance()`, dwie funkcje korelacji — ale nic w
`Engine::step()` jeszcze ich nie łączy. Startowy szkielet kompiluje się, ale nie wywołuje
`advance()`, więc paczki pozostają w miejscu.

## Integracja: odpowiedzialności i ograniczenia

Zadanie łączy mechanizmy z modułów 6 i 7 z kodem napisanym wcześniej w tym module. Tym razem muszą
zadziałać razem dla czterech stref w jednym wywołaniu `step()`.

**Czujniki i korelacja dla każdej paczki.** Każdy czujnik nadal odczytuje strefę, do której jest
fizycznie przypisany (misja 31). Dwie rzeczy muszą się przy tym zdarzyć: stan korelacji paczki
faktycznie obecnej w danej strefie musi się zaktualizować przez `updatePresenceConfirmation` lub
`updateClassification` (tylko gdy strefa jest zajęta — wywołanie na pustym `std::optional<Item>` się nie
skompiluje), a `SensorSnapshot` musi zapisać, której paczki
(`ItemId`) dotyczył dany odczyt — ale tylko gdy odczyt był `Ok` **i** strefa faktycznie zajęta. Odczyt
`Stale` nigdy nie jest przypisywany paczce znajdującej się obecnie w tej strefie —
to powtórzenie wcześniejszej zaufanej wartości, nie świeża obserwacja.

**Polecenie dywertera.** Musi pochodzić **wyłącznie** z paczki, która w tym momencie jest w
`plant_.diverting` — nigdy z paczki właśnie sklasyfikowanej w `plant_.weighing` w tym samym ticku
(misja 30 wyjaśniła, dlaczego to jedyny poprawny wybór: taka paczka fizycznie nie może jeszcze być w
`diverting`). Obowiązuje ten sam warunek co w module 7 (brak wymuszenia,
`diverterMayMove(modeForTick)`), uzupełniony o dwa wymagania: pole `diverting` musi być zajęte, a
paczka musi już mieć klasyfikację.
Ten sam warunek decyduje jednocześnie o tym, czy w ogóle wolno wydać polecenie dywertera, i o wartości
`routingReady`, którą trzeba przekazać do `advance()` — to jeden warunek, nie dwa niezależne.

**Kolejność, która ma znaczenie.** Odczyt czujników, korelacja i decyzja o poleceniu dywertera muszą
się zdarzyć, zanim `advance()` cokolwiek przesunie — inaczej sprawdzałbyś zajętość stref po ruchu,
nie przed nim. `advance()` z kolei musi się zdarzyć, zanim policzysz `Mode` po raz drugi — jak w
module 7, bo ten
drugi rachunek potrzebuje zdarzenia z `AdvanceResult`, którego `advance()` jeszcze nie zwrócił.
Wszystko inne (flagi wejściowe, `latch_`, `decision`, pierwsze liczenie `modeForTick`, warunek ruchu
pasa) pozostaje takie jak w module 7.

**Co musi zawierać `TickResult`.** Kompletny stan wszystkich czterech stref w tym ticku, wynik
`advance()` (`event`, `departure`), i pełną korelację `ItemId` w `SensorSnapshot` — dokładny format
opisuje sekcja niżej. To jedyny punkt odniesienia dla testów i CLI; żaden z nich nie sięga do
wewnętrznego stanu `Engine` w trakcie ticku.

## Rozszerzony `TickResult` i `describe()`

`SensorSnapshot` niesie teraz `presenceObservedItemId`/`weightObservedItemId` — ustawiane tylko, gdy
odczyt jest `Ok` **i** strefa jest faktycznie zajęta. Odczyt `Stale` powtarza *wcześniejszą* zaufaną
wartość i nigdy nie jest przypisywany paczce znajdującej się obecnie w tej strefie.

`describe(TickResult)` określa tekstową postać pełnego wyniku ticku. Jedna linia ma następujący
format:

```text
tick <N>: mode=<M> belt=<B> latch=<L> diverter=<cmd>/<pos>@<id|-> event=<e|-> infeed=<id|->
presenceCheck=<id|-> weighing=<id|-> diverting=<id|-> departure=<id->dest|->
```

gdzie `<dest>` to `"Light"` dla `Zone::OutputLight` albo `"Heavy"` dla `Zone::OutputHeavy` (nie pełna
nazwa `Zone`). Test `tick_result_test.cpp` zawiera dokładne przykłady tego formatu — to Twój ostateczny
kontrakt.

## Co już masz gotowe

Wszystkie typy (`Item`, `Plant`, `TickResult`, `SensorSnapshot`, `Engine`) — kompletne kształty, bez
zmian do wprowadzenia. Cała reszta `Engine`'a poza `step()` (flagi wejściowe, `injectSensorFault`/
`clearSensorFault`/`injectDiverterFault`/`clearDiverterFault`, `spawnItem`) — gotowa i niezmieniona.

## Co masz napisać

- `Engine::step()` w [`src/engine.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/engine.cpp) — pełna integracja opisana powyżej.
- `describe(TickResult)` w [`src/tick_result.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/tick_result.cpp) — format
  opisany powyżej.
- [`apps/simulator_cli/main.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/apps/simulator_cli/main.cpp) — demonstracja co najmniej
  trzech paczek o różnych klasyfikacjach jednocześnie w locie (np. Light/Heavy/Light — zobacz ślad w
  materiałach misji 30), zawierająca co najmniej jeden tick z widocznym przesunięciem łańcuchowym:
  odjazd jednej paczki i wejście kolejnej do właśnie zwolnionej strefy w tym samym ticku. Użyj
  `psm::describe()` do wypisywania każdego ticku.

## Sprawdź się

```bash
ctest --preset test -L misja-32
```

To dedykowany test tej misji (`multiple_items_engine_test`), sprawdzający, że trzy paczki o masach
100g/800g/150g, utworzone jedna po drugiej przez `spawnItem`, odjeżdżają w tej samej kolejności, każda
z poprawnym, niezależnym rutowaniem (Light/Heavy/Light) — wykryje powrót do współdzielonej
klasyfikacji.

Uzupełnienie `Engine::step()` jednocześnie odblokowuje każdy pozostały test na poziomie `Engine` —
uruchom też pełny zestaw:

```bash
ctest --preset test
```

Oczekiwany wynik: wszystkie testy zielone (`misja-1`, `misja-3`–`misja-4`, `misja-6`–`misja-22`,
`misja-24`–`misja-32`).

Uruchom też program naprawdę:
```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

## Zapisz swoją pracę

```bash
git status
git add <pliki które zmieniłeś>
git commit -m "..."
```

## Częste błędy

- **Wyliczanie polecenia dywertera z `plant_.weighing`** — przed tą pomyłką ostrzega misja 30
  i sekcja "Polecenie dywertera" powyżej ostrzegają.
- **Wołanie `updatePresenceConfirmation`/`updateClassification` bez sprawdzenia `.has_value()`** —
  wywołanie na pustym `std::optional<Item>` się nie skompiluje (brak czego dereferencjonować) — ale
  łatwo przeoczyć samą bramkę `if`, jeśli kopiuje się kod bez zastanowienia.
- **Ustawianie `presenceObservedItemId`/`weightObservedItemId` bez sprawdzenia `.has_value()`** —
  odczyt może być `Ok`, choć strefa jest pusta; korelacja musi to rozróżniać.
- **Inny zapis miejsca docelowego w `describe()` niż `"Light"`/`"Heavy"`** — test sprawdza
  te dwa literały, a nie pełne nazwy `Zone`.

## Koniec modułu 8

Symulator modeluje teraz to, co robi każda prawdziwa taśma sortująca: kilka paczek naraz, każda na
swoim etapie, każda ze swoim własnym stanem — a mimo to jeden wspólny dywerter i jeden wspólny pas
wciąż działają poprawnie, bo ograniczenie "co najwyżej jedna paczka na strefę" i kolejność
przetwarzania od wyjścia do wejścia eliminują kolizje przez samą konstrukcję, bez żadnej dodatkowej
logiki arbitrażu.
