# 8.4 Silnik z wieloma paczkami

## Problem

Wszystkie kawałki istnieją osobno — nowy `Plant`, `advance()`, dwie funkcje korelacji — ale nic w
`Engine::step()` jeszcze ich nie łączy. Startowy szkielet tej misji to świadomie tymczasowy
placeholder: kompiluje się, ale nigdy nie wywołuje `advance()`, więc żadna paczka się nie porusza.

## Rozszerzenie `step()`

Kroki 1–3 (flagi wejściowe, `latch_`, `decision`, pierwsze liczenie `modeForTick`) zostają dokładnie
takie, jak w Module 7. Zmienia się reszta:

**4. Czujniki + korelacja.** Każdy czujnik czyta swój slot dokładnie tak jak w Module 6/7
(`presenceSensor_.read(...)`, `weightSensor_.read(...)`), ale teraz trzeba dodatkowo zaktualizować
`ControllerState` paczki, która w danym slocie faktycznie jest — i tylko wtedy, gdy slot jest zajęty
(wołanie `updatePresenceConfirmation`/`updateClassification` na pustym `std::optional<Item>` się nie
skompiluje).

**5. Polecenie dywertera.** Musi pochodzić **wyłącznie** z paczki, która w tym momencie jest w
`plant_.diverting` — nigdy z paczki właśnie sklasyfikowanej w `plant_.weighing` w tym samym ticku
(Misja 30 wyjaśniła, dlaczego to jedyny poprawny wybór: taka paczka fizycznie nie może jeszcze być w
`diverting`). Bramka jest ta sama co w Module 7 (brak override'u, `diverterMayMove(modeForTick)`), z
dwoma dodatkowymi warunkami: slot `diverting` musi być zajęty, i ta paczka musi już mieć klasyfikację.
Ten sam warunek decyduje jednocześnie o tym, czy w ogóle wolno wydać polecenie dywertera, i o wartości
`routingReady`, którą trzeba przekazać niżej do `advance()` — to jeden warunek, nie dwa niezależne.

**6. Pas** — bramkowany przez `modeForTick`, jak w Module 7, bez zmian.

**7. `advance()`** — pod tą samą bramką co zawsze (pas faktycznie `Running`), teraz zwraca
`AdvanceResult` zamiast samego zdarzenia.

**8. Drugie liczenie `Mode`** — `reactToSystemEvent(modeForTick, ...)`, gdzie zdarzenie pochodzi teraz
z `AdvanceResult` z kroku 7.

**9. Korelacja id w `SensorSnapshot`.** `presenceObservedItemId`/`weightObservedItemId` mają być
ustawione tylko wtedy, gdy odczyt z kroku 4 był `Ok` **i** odpowiedni slot faktycznie jest zajęty —
odczyt `Stale` powtarza wcześniejszą zaufaną wartość i nie wolno go przypisać dzisiejszemu
okupantowi slotu, nawet jeśli ktoś tam akurat stoi.

**10. `TickResult`** — ze wszystkimi czterema slotami i `departure` z `AdvanceResult`.

## Rozszerzony `TickResult` i `describe()`

`SensorSnapshot` niesie teraz `presenceObservedItemId`/`weightObservedItemId` — ustawiane tylko, gdy
odczyt jest `Ok` **i** slot jest faktycznie zajęty. Odczyt `Stale` powtarza *wcześniejszą* zaufaną
wartość i nigdy nie jest przypisywany dzisiejszemu okupantowi slotu — nawet jeśli ktoś tam akurat stoi.

`describe(TickResult)` staje się jedynym źródłem prawdy o pełnym śladzie tekstowym ticku, w dokładnie
tym formacie (jedna linia):

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

- `Engine::step()` w [`src/engine.cpp`](../../src/engine.cpp) — pełna integracja opisana powyżej.
- `describe(TickResult)` w [`src/tick_result.cpp`](../../src/tick_result.cpp) — dokładny format
  powyżej.
- [`apps/simulator_cli/main.cpp`](../../apps/simulator_cli/main.cpp) — demonstracja co najmniej
  trzech paczek o różnych klasyfikacjach jednocześnie w locie (np. Light/Heavy/Light — zobacz ślad w
  materiałach Misji 30), zawierająca co najmniej jeden tick z widocznym "same-tick chain shift":
  odjazd jednej paczki i wejście kolejnej do właśnie zwolnionego slotu w tym samym ticku. Użyj
  `psm::describe()` do wypisywania każdego ticku.

## Sprawdź się

```bash
ctest --preset test -L misja-32
```

To dedykowany test tej misji (`multiple_items_engine_test`), sprawdzający, że trzy paczki o masach
100g/800g/150g, utworzone jedna po drugiej przez `spawnItem`, odjeżdżają w tej samej kolejności, każda z poprawnym,
niezależnym rutowaniem (Light/Heavy/Light) — złapie dokładnie regresję do współdzielonej klasyfikacji.

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

- **Wyliczanie polecenia dywertera z `plant_.weighing`** — dokładnie ta pomyłka, przed którą Misja 30
  i krok 5 powyżej ostrzegają.
- **Wołanie `updatePresenceConfirmation`/`updateClassification` bez sprawdzenia `.has_value()`** —
  wywołanie na pustym `std::optional<Item>` się nie skompiluje (brak czego dereferencjonować) — ale
  łatwo przeoczyć samą bramkę `if`, jeśli kopiuje się kod bez zastanowienia.
- **Ustawianie `presenceObservedItemId`/`weightObservedItemId` niezależnie od `.has_value()` slotu** —
  odczyt może być `Ok`, a slot i tak pusty (potwierdzony pusty odczyt); korelacja musi to rozróżniać.
- **Rozszerzenie `describe()` o inny format destination niż `"Light"`/`"Heavy"`** — test sprawdza
  dokładnie te dwa literały, nie pełne nazwy `Zone`.

## Koniec modułu 8

Symulator modeluje teraz to, co robi każda prawdziwa taśma sortująca: kilka paczek naraz, każda na
swoim etapie, każda ze swoim własnym stanem — a mimo to jeden wspólny dywerter i jeden wspólny pas
wciąż działają poprawnie, bo backpressure per-strefa i kolejność downstream-to-upstream eliminują
kolizje przez samą konstrukcję, bez żadnej dodatkowej logiki arbitrażu.
