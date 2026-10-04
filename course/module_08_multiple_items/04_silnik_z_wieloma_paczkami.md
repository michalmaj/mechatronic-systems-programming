🇵🇱 Polski | [🇬🇧 English](04_silnik_z_wieloma_paczkami.en.md)

# 8.4 `Engine` z wieloma paczkami

## Problem

Nowy `Plant`, funkcja `advance()` i funkcje przypisujące wyniki pomiarów do paczek są już gotowe.
Nie zostały jednak połączone w `Engine::step()`. Kod startowy się kompiluje, lecz nie wywołuje
`advance()`, dlatego żadna paczka się nie przesuwa.

## Integracja elementów

W tej misji połączysz rozwiązania z modułów 6 i 7 z kodem napisanym w bieżącym module. Wszystkie
operacje muszą działać razem dla czterech stref podczas jednego wywołania `step()`.

**Czujniki i przypisanie odczytów do paczek.**

Każdy czujnik odczytuje strefę, przy której jest zamontowany. Jeśli znajduje się w niej paczka,
zaktualizuj jej stan przez `updatePresenceConfirmation()` albo `updateClassification()`. Najpierw
sprawdź, czy odpowiedni `std::optional<Item>` zawiera wartość. Bez tego nie można przekazać paczki do
funkcji.

`SensorSnapshot` ma także zapisać `ItemId` paczki, której dotyczył odczyt. Ustaw identyfikator tylko
wtedy, gdy strefa była zajęta, a odczyt miał status `Ok`. Odczytu `Stale` nie wolno przypisać paczce,
która znajduje się obecnie przy czujniku. Jest to wcześniej zapamiętana wartość, a nie nowy pomiar tej
paczki.

**Polecenie dywertera.**

Polecenie dywertera wyznacz wyłącznie z klasyfikacji paczki znajdującej się w
`plant_.diverting`. Nie korzystaj z paczki właśnie sklasyfikowanej w `plant_.weighing`, ponieważ
przed wywołaniem `advance()` nie znajduje się ona jeszcze przy dywerterze.

Dywerter może otrzymać polecenie, gdy jednocześnie:

- `decision.overrideActive` nie wymusza zatrzymania,
- `diverterMayMove(modeForTick)` zwraca `true`,
- `plant_.diverting` zawiera paczkę,
- ta paczka ma już klasyfikację.

Ten sam warunek określa wartość `routingReady` przekazywaną do `advance()`. Nie wyznaczaj osobno
gotowości do wydania polecenia i gotowości do skierowania paczki.

**Wymagana kolejność.**

Odczytaj czujniki, przypisz wyniki do paczek i wyznacz polecenie dywertera przed wywołaniem
`advance()`. W przeciwnym razie sprawdzisz strefy dopiero po przesunięciu paczek.

Następnie wywołaj `advance()` i zachowaj zwrócony `AdvanceResult`. Dopiero po tym wyznacz końcową
wartość `Mode`, ponieważ `reactToSystemEvent()` potrzebuje zdarzenia z wyniku `advance()`. Flagi
wejściowe, `latch_`, `decision`, pierwsze wyznaczenie `modeForTick` i warunek ruchu taśmy pozostają
takie jak w module 7.

**Zawartość `TickResult`.**

Wynik ticku ma zawierać stan wszystkich czterech stref, `event` i `departure` zwrócone przez
`advance()` oraz identyfikatory paczek przypisane do odczytów w `SensorSnapshot`. Testy i program
terminalowy korzystają wyłącznie z `TickResult`, bez odczytywania wewnętrznych pól `Engine` w trakcie
wykonywania ticku.

## Rozszerzony `TickResult` i `describe()`

`SensorSnapshot` zawiera teraz pola `presenceObservedItemId` i `weightObservedItemId`. Ustawiaj je
tylko dla odczytu ze statusem `Ok` wykonanego przy zajętej strefie. `Stale` oznacza powtórzenie
wcześniej zapamiętanej wartości, dlatego nie wskazuje paczki znajdującej się obecnie przy czujniku.

`describe(TickResult)` tworzy jednowierszowy opis wyniku w następującym formacie:

```text
tick <N>: mode=<M> belt=<B> latch=<L> diverter=<cmd>/<pos>@<id|-> event=<e|-> infeed=<id|->
presenceCheck=<id|-> weighing=<id|-> diverting=<id|-> departure=<id->dest|->
```

W miejsce `<dest>` wpisz `"Light"` dla `Zone::OutputLight` albo `"Heavy"` dla
`Zone::OutputHeavy`. Nie używaj pełnej nazwy wartości `Zone`. Dokładne przykłady formatu znajdziesz w
`tick_result_test.cpp`.

## Co już masz gotowe

Typy `Item`, `Plant`, `TickResult`, `SensorSnapshot` i `Engine` mają już docelowe definicje. Nie
musisz ich zmieniać. Gotowe są również flagi wejściowe, `spawnItem()` oraz metody obsługi usterek
czujników i dywertera. Do uzupełnienia pozostało `Engine::step()`.

## Co masz napisać

- Zintegruj opisane elementy w `Engine::step()` w pliku
  [`src/engine.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/engine.cpp).
- Zaimplementuj `describe(TickResult)` w
  [`src/tick_result.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/tick_result.cpp)
  zgodnie z podanym formatem.
- W
  [`apps/simulator_cli/main.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/apps/simulator_cli/main.cpp)
  pokaż działanie co najmniej trzech paczek o różnych klasyfikacjach, na przykład Light, Heavy i
  Light. W przebiegu ma wystąpić tick, w którym jedna paczka odjeżdża, a następna wchodzi do
  zwolnionej strefy. Do wypisywania każdego wyniku użyj `psm::describe()`.

## Sprawdź się

```bash
ctest --preset test -L misja-32
```

Test `multiple_items_engine_test` sprawdza trzy paczki o masach 100 g, 800 g i 150 g, dodane kolejno
przez `spawnItem()`. Paczki mają odjechać w tej samej kolejności i trafić odpowiednio do wyjść Light,
Heavy i Light. Test wykryje między innymi ponowne użycie jednej wspólnej klasyfikacji.

Uzupełnienie `Engine::step()` umożliwi też wykonanie pozostałych testów dotyczących `Engine`.
Uruchom pełny zestaw:

```bash
ctest --preset test
```

Oczekiwany wynik: przechodzą testy `misja-1`, `misja-3`–`misja-4`, `misja-6`–`misja-22` oraz
`misja-24`–`misja-32`.

Zbuduj i uruchom również program:

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

- **Wyznaczenie polecenia dywertera na podstawie `plant_.weighing`**: właściwa paczka znajduje się w
  `plant_.diverting`.
- **Wywołanie `updatePresenceConfirmation()` albo `updateClassification()` bez sprawdzenia
  `.has_value()`**: pusty `std::optional<Item>` nie zawiera paczki, którą można przekazać do funkcji.
- **Ustawienie `presenceObservedItemId` lub `weightObservedItemId` bez sprawdzenia zajętości strefy**:
  czujnik może zwrócić `Ok` także dla pustej strefy, ale taki wynik nie dotyczy żadnej paczki.
- **Inny zapis miejsca docelowego w `describe()` niż `"Light"` lub `"Heavy"`**: test oczekuje tych
  dwóch napisów, a nie pełnych nazw wartości `Zone`.

## Koniec modułu 8

Symulator obsługuje teraz kilka paczek znajdujących się na różnych etapach procesu. Każda z nich ma
własny stan, a wszystkie korzystają ze wspólnej taśmy i jednego dywertera. Ograniczenie do jednej
paczki w strefie oraz przetwarzanie stref od wyjścia do wejścia zapobiegają kolizjom bez dodatkowego
mechanizmu rozstrzygania pierwszeństwa.
