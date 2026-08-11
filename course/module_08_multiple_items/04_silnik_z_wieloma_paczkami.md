# 8.4 Silnik z wieloma paczkami

## Problem

Wszystkie kawałki istnieją osobno — nowy `Plant`, `advance()`, dwie funkcje korelacji — ale nic w
`Engine::step()` jeszcze ich nie łączy. Startowy szkielet tej misji to świadomie tymczasowy
placeholder: kompiluje się, ale nigdy nie wywołuje `advance()`, więc żadna paczka się nie porusza.

## Rozszerzone `step()`

```cpp
TickResult Engine::step() {
    // 1-3. flagi, latch_, decision, modeForTick -- bez zmian od Modułu 7.
    ...

    // 4. czujniki + korelacja -- tylko gdy odpowiedni slot jest zajęty.
    const PresenceReading presence = presenceSensor_.read(plant_.presenceCheck, presenceFault_);
    if (plant_.presenceCheck.has_value()) {
        updatePresenceConfirmation(*plant_.presenceCheck, presence);
    }
    const WeightReading weight = weightSensor_.read(plant_.weighing, weightFault_);
    if (plant_.weighing.has_value()) {
        updateClassification(*plant_.weighing, weight);
    }

    // 5. polecenie dywertera -- WYŁĄCZNIE z plant_.diverting, nigdy z plant_.weighing.
    const bool routingReady = !decision.overrideActive && diverterMayMove(modeForTick)
                               && plant_.diverting.has_value()
                               && plant_.diverting->classification.has_value();
    DiverterCommand diverterCommand = DiverterCommand::HoldStraight;
    std::optional<ItemId> diverterCommandItemId;
    if (!decision.overrideActive && diverterMayMove(modeForTick) && plant_.diverting.has_value()
        && plant_.diverting->classification.has_value()) {
        diverterCommand = toDiverterCommand(*plant_.diverting->classification);
        diverterCommandItemId = plant_.diverting->id;
        diverter_.setCommand(diverterCommand);
        diverter_.resolve(diverterFault_);
    }

    // 6. pas -- bramkowany przez modeForTick, jak w Module 7.
    ...

    // 7. advance() -- pod tą samą bramką co zawsze.
    AdvanceResult advanceResult;
    if (beltMotor_.actualState() == BeltMotorState::Running) {
        advanceResult = psm::advance(plant_, diverter_, routingReady);
    }

    // 8. Mode, krok drugi.
    mode_ = reactToSystemEvent(modeForTick, advanceResult.event);

    // 9. korelacja id w SensorSnapshot -- tylko gdy Ok I paczka faktycznie obecna.
    const std::optional<ItemId> presenceObservedItemId =
        (presence.status == ReadingStatus::Ok && plant_.presenceCheck.has_value())
            ? std::optional<ItemId>{plant_.presenceCheck->id} : std::nullopt;
    const std::optional<ItemId> weightObservedItemId =
        (weight.status == ReadingStatus::Ok && plant_.weighing.has_value())
            ? std::optional<ItemId>{plant_.weighing->id} : std::nullopt;

    // 10. TickResult, ze wszystkimi czterema slotami i departure.
    ...
}
```

Zwróć uwagę na krok 5: `routingReady` i polecenie dywertera pochodzą wyłącznie z `plant_.diverting`
sprzed wywołania `advance()` — nigdy z paczki właśnie sklasyfikowanej w `plant_.weighing` w tym samym
ticku (Misja 30 wyjaśniła, dlaczego to jedyny poprawny wybór: taka paczka fizycznie nie może jeszcze
być w `diverting`).

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
nazwa `Zone`). Test `tick_result_test.cpp` zawiera dokładne, zamrożone przykłady tego formatu — to Twój
ostateczny kontrakt.

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

## Self-check

```bash
ctest --preset test -L misja-32
```

To dedykowany test tej misji (`multiple_items_engine_test`), sprawdzający, że trzy paczki o masach
100g/800g/150g, zespawnowane jedna po drugiej, odjeżdżają w tej samej kolejności, każda z poprawnym,
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
