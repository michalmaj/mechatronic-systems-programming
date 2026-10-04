🇵🇱 Polski | [🇬🇧 English](06_pierwszy_przebieg.en.md)

# 1.6 Pierwszy przebieg

## Problem

Do tej pory działanie kodu sprawdzały testy, które nie pokazywały przebiegu programu. Czas zobaczyć
go na własne oczy. Napiszesz program, który tworzy jedną paczkę, przepuszcza ją przez system i po
każdym kroku pokazuje jej stan na konsoli.

## Nowe elementy C++

**`main()`** jest punktem wejścia programu. Znasz go już z `toolchain_check` (moduł 0) i z gotowego
podglądu `simulator_cli`, ale teraz po raz pierwszy napiszesz go samodzielnie.

**`std::cout`** służy do wypisywania tekstu na konsolę, tak jak w `toolchain_check`.

## Co już masz gotowe

[`apps/simulator_cli/main.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/apps/simulator_cli/main.cpp):

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

1. Utwórz `Plant` i dodaj do niego jedną paczkę przez `spawnItem`. Wybierz dowolną masę, np. `750`
   gramów, żeby zobaczyć trasę przez `OutputHeavy`, albo coś poniżej `500`, żeby zobaczyć
   `OutputLight`.
2. Napisz pętlę (np. na 6 ticków), która w każdym obiegu: jeśli `plant.item` ma wartość, liczy
   `WeightClass` i `DiverterPosition` (tak jak `runTicks` z misji 5), wywołuje
   `advance(plant, ...)`, a następnie **wypisuje** numer ticku oraz aktualną strefę paczki (albo
   napis w rodzaju `"empty"`, jeśli `plant.item` już nie ma wartości). Do zamiany strefy na tekst
   użyj `psm::toString(zone)` z misji 1. Pamiętaj o `#include <psm/zone.hpp>`.

Nie korzystaj tutaj z `runTicks`, ponieważ ta funkcja nie udostępnia wyniku po każdym kroku. Napisz
w `main()` niewielką pętlę, która używa tych samych funkcji `classify`, `toDiverterPosition` i
`advance`, a po każdym ticku wypisuje stan.

Przykładowy format wyjścia (możesz wybrać inny):

```
tick 0: zone=PresenceCheck
tick 1: zone=Weighing
...
tick 4: empty
```

## Sprawdź się

Najpierw uruchom test podstawowy. Sprawdza on tylko, czy program uruchamia się i kończy bez błędu:

```bash
ctest --preset test -L misja-6
```

Następnie uruchom program i **przeczytaj wynik**:

```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

(na Windows: `.\build\dev\apps\simulator_cli\Debug\simulator_cli.exe`; ścieżka zależy od
generatora Twojego IDE).

Sprawdź, czy widzisz sensowną sekwencję stref, kończącą się dotarciem do `OutputLight` albo
`OutputHeavy` (zależnie od masy, którą wybrałeś), a potem `empty`.

## Koniec modułu: pełny zestaw testów

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

- **Wywołanie `plant.item->mass` bez sprawdzenia `has_value()`:** tak jak w misji 5, to
  niezdefiniowane zachowanie, gdy przenośnik jest pusty.
- **Program kończy się natychmiast i niczego nie wypisuje:** sprawdź, czy używasz `std::cout`
  wewnątrz pętli, a nie tylko raz na końcu.
- **Brak `#include <psm/zone.hpp>`** przy próbie użycia `psm::toString`: `plant.hpp` go nie
  dociąga automatycznie w sposób, na którym warto polegać; dołącz go jawnie.

## Pytanie do zastanowienia

Porównaj kod w `main()` z `runTicks` z misji 5. Co się w nich powtarza, a co jest różne? Czy ta
niewielka ilość powtórzonego kodu Ci przeszkadza? Dlaczego na tym etapie projektu jest jeszcze
akceptowalna?

## Koniec modułu 1

Masz teraz działający, kompletny (choć mały) system, w którym paczka porusza się przez kolejne
strefy i jest sortowana według wagi. Stan systemu jest oddzielony od logiki podejmowania decyzji.
W kolejnych modułach rozbudujesz tę architekturę o aktuatory z własnym stanem, tryby pracy,
bezpieczeństwo, czujniki i usterki.
