# 1.6 Pierwszy przebieg

## Problem

Wszystko, co napisałeś do tej pory, sprawdzały za Ciebie testy — uruchamiane po cichu, bez żadnego
widocznego wyjścia. Czas zobaczyć to na własne oczy: napisać program, który tworzy jedną paczkę,
przepuszcza ją przez system i pokazuje na konsoli, co się z nią dzieje, krok po kroku.

## Nowe elementy C++

**`main()`** — punkt wejścia programu. Widziałeś go już w `toolchain_check` (Module 0) i w gotowym
podglądzie `simulator_cli`, ale to pierwszy raz, kiedy piszesz go sam, do własnego kodu.

**`std::cout`** — wypisywanie tekstu na konsolę, ten sam mechanizm co w `toolchain_check`.

## Co już masz gotowe

[`apps/simulator_cli/main.cpp`](../../apps/simulator_cli/main.cpp):

```cpp
#include <iostream>

#include <psm/controller.hpp>
#include <psm/plant.hpp>

int main() {
    // TODO (Misja 6: pierwszy_przebieg): ...
    return 0;
}
```

## Co masz napisać

W `main()`:

1. Stwórz `Plant` i dodaj do niego jedną paczkę przez `spawnItem` — wybierz dowolną masę (np. `750`
   gramów, żeby zobaczyć trasę przez `OutputHeavy`, albo coś poniżej `500`, żeby zobaczyć
   `OutputLight`).
2. Napisz pętlę (np. na 6 ticków), która w każdym obiegu: jeśli `plant.item` ma wartość, liczy
   `WeightClass` i `DiverterPosition` (dokładnie tak, jak w `runTicks` z Misji 5), wywołuje
   `advance(plant, ...)`, a następnie **wypisuje** numer ticku oraz aktualną strefę paczki (albo
   napis w rodzaju `"empty"`, jeśli `plant.item` już nie ma wartości). Do zamiany strefy na tekst
   użyj `psm::toString(zone)` z Misji 1 — pamiętaj o `#include <psm/zone.hpp>`.

Ta pętla **celowo nie wywołuje `runTicks`** z Misji 5 — `runTicks` nic nie wypisuje (jest czysty,
łatwy do przetestowania), a Ty właśnie potrzebujesz wypisywania po każdym kroku. Napisz własną,
niewielką pętlę w `main()`, korzystając z tych samych `classify`/`toDiverterPosition`/`advance`, co
poprzednio.

Przykładowy kształt jednej linii wyjścia (dokładny format zależy od Ciebie):

```
tick 0: zone=PresenceCheck
tick 1: zone=Weighing
...
tick 4: empty
```

## Self-check

Najpierw smoke test — sprawdza tylko, że program się uruchamia i kończy bez błędu:

```bash
ctest --preset test -L misja-6
```

Ale to nie wystarczy — uruchom program naprawdę i **przeczytaj wynik**:

```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

(na Windows: `.\build\dev\apps\simulator_cli\Debug\simulator_cli.exe`, dokładna ścieżka zależy od
generatora Twojego IDE).

Sprawdź, czy widzisz sensowną sekwencję stref, kończącą się dotarciem do `OutputLight` albo
`OutputHeavy` (zależnie od masy, którą wybrałeś), a potem `empty`.

## Koniec modułu — pełny zestaw testów

Teraz, gdy wszystkie sześć misji jest zrobionych, uruchom cały zestaw naraz, bez żadnego filtra:

```bash
ctest --preset test
```

Oczekiwany wynik: **wszystkie testy przechodzą** (`100% tests passed`).

## Zapisz swoją pracę

Jeśli jeszcze tego nie zrobiłeś w trakcie modułu:

```bash
git status
git add <pliki które zmieniłeś>
git commit -m "..."
```

## Częste błędy

- **Wywołanie `plant.item->mass` bez sprawdzenia `has_value()`** — tak jak w Misji 5, to
  niezdefiniowane zachowanie, gdy przenośnik jest pusty.
- **Program kończy się natychmiast bez żadnego wyjścia** — sprawdź, czy naprawdę wypisujesz coś
  wewnątrz pętli, a nie tylko raz na końcu (albo wcale).
- **Brak `#include <psm/zone.hpp>`** przy próbie użycia `psm::toString` — `plant.hpp` go nie
  dociąga automatycznie w sposób, na którym warto polegać; dołącz go jawnie.

## Pytanie do zastanowienia

Spójrz na kod w `main()` i na `runTicks` z Misji 5 obok siebie. Co dokładnie się powtarza między
nimi, a co jest różne? Czy ta odrobina duplikacji Ci przeszkadza — i dlaczego (albo dlaczego nie) na
tym etapie projektu jest to akceptowalne?

## Koniec modułu 1

Masz teraz działający, kompletny (choć mały) system: paczkę poruszającą się przez strefy i sortowaną
według wagi, ze stanem oddzielonym od decyzji. To jest dokładnie ta sama architektura, do której ten
projekt doszedł na samym początku swojej historii — w kolejnych modułach będziesz ją rozbudowywać
o kolejne elementy: aktuatory z własnym stanem, tryby pracy, bezpieczeństwo, czujniki i usterki.
