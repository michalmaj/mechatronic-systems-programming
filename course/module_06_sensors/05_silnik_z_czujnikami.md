🇵🇱 Polski | [🇬🇧 English](05_silnik_z_czujnikami.en.md)

# 6.5 `Engine` z czujnikami

## Problem

Oba czujniki oraz `decideClassification` i `ControllerState` są już gotowe, ale działająca
symulacja jeszcze z nich nie korzysta. `Engine` nadal odczytuje `Item::mass` bezpośrednio.

## Nowe elementy C++

**`Engine::injectFault(FaultTarget, FaultKind)` i `clearFault(FaultTarget)`** różnią się od
dotychczasowych metod wejściowych. Wprowadzona usterka jest **trwała**. Nie znika po jednym ticku,
lecz pozostaje aktywna do wywołania `clearFault`. Model nie zakłada więc, że usterka czujnika usunie
się samoczynnie.

Nowe pola prywatne `presenceSensor_`, `weightSensor_`, `controllerState_`, `presenceFault_`,
`weightFault_`. Dwa ostatnie mają typ `std::optional<FaultKind>`.

## Brak klasyfikacji nie może oznaczać `HoldStraight`

Przed napisaniem kodu rozważ błędne podejście: „jeśli
`controllerState_.classification` ma wartość, użyj jej; w przeciwnym razie zostaw domyślną komendę
dywertera (`HoldStraight`)”. **To jest błąd.** `HoldStraight` jest prawidłowym poleceniem. Dywerter
ustawiłby się, a paczka **pojechałaby dalej**, mimo że nie została wiarygodnie sklasyfikowana. Bez
klasyfikacji paczka nie może zostać skierowana do żadnego wyjścia.

Dlatego `Plant::advance` (od tej misji) przyjmuje trzeci parametr:

```cpp
void advance(Plant& plant, const Diverter& diverter, bool routingReady = true);
```

Parametr jest używany **wyłącznie** w gałęzi `Diverting`. Zachowanie pozostałych stref się nie
zmienia. Gdy `routingReady` ma wartość `false`, paczka pozostaje w `Diverting`, tak samo jak przy
nieustawionym jeszcze dywerterze.

## Rozszerzona kolejność `step()`

Kroki 1–4, czyli obsługa żądań, `latch_`, ścieżki awaryjnej i `Mode`, pozostają bez zmian względem
modułu 5. Nowe kroki zaczynają się od punktu 5:

5. **Odczytaj oba czujniki w każdym ticku**, niezależnie od `Mode` i `overrideActive`:
   `presenceSensor_.read(plant_.item, presenceFault_)`, `weightSensor_.read(plant_.item,
   weightFault_)`. Odczyt czujnika nie zmienia stanu symulacji.
6. **Wywołaj `updateControllerState`** z misji 23, przekazując bieżącą paczkę i nowe odczyty.
7. **Oblicz `routingReady`:**
   `!decision.overrideActive && diverterMayMove(mode_) && controllerState_.classification.has_value()`.
8. **Wyznacz polecenie dywertera** tylko przy
   `!decision.overrideActive && diverterMayMove(mode_)`, tak jak wcześniej. Tym razem użyj jednak
   `controllerState_.classification`, a nie bezpośrednio `classify(item->mass)`.
9. **Wywołaj `psm::advance(plant_, diverter_, routingReady)`**. Jest to pierwsze wywołanie z
   trzema argumentami.
10. Utwórz `TickResult`, tym razem z polem `sensors`, a następnie zwiększ `tick_`.

## Przykładowy przebieg w programie konsolowym

Status `Stale` pozwala pokazać, że nawet wiarygodnie wyglądająca wartość nie może posłużyć do nowej
klasyfikacji. Najpierw przepuść jedną paczkę bez usterki, aby czujnik wagi zapamiętał jej masę.
Następnie ustaw usterkę typu `Stale` w czujniku wagi i dodaj drugą paczkę. Czujnik powtórzy masę
pierwszej paczki, ale `decideClassification` odrzuci ten odczyt ze względu na status. Druga paczka
dotrze do `Diverting` i tam się zatrzyma.

## Co już masz gotowe

`include/psm/engine.hpp` ma już wszystkie potrzebne pola i deklaracje. W `src/engine.cpp` znajdziesz
puste szkielety `injectFault` i `clearFault`. Ciało `step()` nadal odpowiada wersji z modułu 5, którą
teraz rozszerzysz.

## Co masz napisać

- `Engine::injectFault(FaultTarget, FaultKind)`: zapisz `kind` do `presenceFault_` albo
  `weightFault_`, zależnie od `target`.
- `Engine::clearFault(FaultTarget)`: wyczyść odpowiednie pole, przypisując `std::nullopt`.
- `Engine::step()`: dodaj kroki 5–10 opisane wyżej.
- `apps/simulator_cli/main.cpp`: zaimplementuj przykładowy przebieg z dwiema paczkami i usterką między
  nimi oraz wypisuj `sensors` obok istniejącego wyjścia.

## Sprawdź się

```bash
ctest --preset test -L misja-24
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

Oczekiwany wynik: przechodzą wszystkie testy od `misja-1` do `misja-4` oraz od `misja-6` do
`misja-24`.

## Zapisz swoją pracę

```bash
git status
git add <pliki które zmieniłeś>
git commit -m "..."
```

## Częste błędy

- **Zamiana `nullopt` na `HoldStraight`:** sprawdź, czy wartość `routingReady` jest przekazywana do
  `psm::advance`.
- **Sprawdzenie tylko `Mode`, bez `controllerState_.classification.has_value()`:** `routingReady`
  musi łączyć wszystkie trzy warunki.
- **Aktualizowanie `ControllerState` tylko czasami** (np. tylko gdy `diverterMayMove` jest
  prawdziwe): odczyt czujników i aktualizacja stanu muszą odbywać się w każdym ticku. Osobno
  podejmowana jest decyzja o dopuszczeniu ruchu.

## Pytanie do zastanowienia

Ta misja wprowadza `routingReady` jako trzeci, ogólny parametr `Plant::advance`, a nie
`WeightClass` ani `DiverterCommand`. Dlaczego to jest właściwy poziom szczegółowości dla tej
granicy między `Plant` a sterownikiem, skoro `Plant` od modułu 1 nie zna szczegółów
klasyfikacji?

## Koniec modułu 6

System korzysta teraz tylko z wiarygodnych odczytów i łączy dwa pomiary wykonane w kolejnych
strefach w jedną decyzję. W następnych modułach pojawią się zdarzenia systemowe i usterki elementów
wykonawczych, na podstawie których system będzie mógł przechodzić do `Mode::Fault`.
