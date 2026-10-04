🇵🇱 Polski | [🇬🇧 English](02_termin_rutowania.en.md)

# 7.2 Limit czasu na ustawienie dywertera

W tej misji ważna jest nie tylko implementacja licznika, lecz także ustalenie, kiedy powinien on
być zerowany. Przeczytaj opis reguły przed rozpoczęciem pracy.

## Problem

W strefie `Diverting` obiekt `Plant` czeka, aż dywerter osiągnie właściwe położenie. Obecnie nie ma
żadnego limitu tego oczekiwania. Jeśli dywerter zostanie zablokowany, paczka pozostanie w tej strefie,
a program nie zgłosi przyczyny problemu.

## Nowy element C++

```cpp
enum class SystemEventKind { DiverterNotReady, RoutingDeadlineMissed };
```

Typ pojawia się razem z pierwszym miejscem, w którym jest potrzebny.

`Plant` otrzymuje licznik:

```cpp
struct Plant {
    std::optional<Item> item;
    int divertingWaitTicks = 0;
};
```

Typ zwracany przez `advance()` zmienia się z `void` na `std::optional<SystemEventKind>`. Istniejące
wywołania tej funkcji nadal są poprawne. W C++ można zignorować zwracaną wartość, jeśli w danym
miejscu nie jest potrzebna. Dlatego nie musisz zmieniać dotychczasowych wywołań w
`plant_test.cpp`, `plant_diverter_test.cpp`, `loop.cpp` ani `Engine::step()`.

## Dokładna reguła

W gałęzi `Diverting`:

- Jeśli `!routingReady`, wyzeruj `divertingWaitTicks` i zwróć `std::nullopt`. Nie pozostawiaj
  dotychczasowej wartości licznika. Powód opisujemy poniżej.
- Jeśli `routingReady`, ale `!diverter.isSettled()`, zwiększ `divertingWaitTicks`. Zwróć
  `DiverterNotReady`, dopóki licznik jest mniejszy lub równy `1`. Po przekroczeniu tej wartości zwróć
  `RoutingDeadlineMissed`.
- Jeśli dywerter jest ustawiony, skieruj paczkę do odpowiedniego wyjścia i wyzeruj licznik.

Licznik należy wyzerować także przy przejściu `Weighing` → `Diverting`. Każda paczka zaczyna w ten
sposób z pełnym limitem czasu, niezależnie od przebiegu poprzedniego cyklu.

## Dlaczego licznik obejmuje tylko kolejne aktywne próby

Załóżmy, że przy `!routingReady` licznik zachowuje swoją wartość. Przerwa spowodowana naciśnięciem
przycisku awaryjnego, zmianą trybu pracy albo brakiem klasyfikacji skracałaby wtedy czas dostępny
dywerterowi. Program mógłby zgłosić `RoutingDeadlineMissed`, choć sam dywerter nie miał jeszcze
wystarczającej liczby kolejnych prób ustawienia się. Jeden sygnał opisywałby wówczas kilka różnych
przyczyn zatrzymania.

Zerowanie licznika przy `!routingReady` zapobiega takiej sytuacji. Licznik obejmuje tylko kolejne
ticki, w których dywerter może zmienić położenie i skierować paczkę do właściwego wyjścia.

## Co już masz gotowe

[`include/psm/system_event_kind.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/include/psm/system_event_kind.hpp)
zawiera gotowy typ zdarzenia.

W pliku
[`include/psm/plant.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/include/psm/plant.hpp)
znajdziesz `divertingWaitTicks` i nową sygnaturę `advance()`.

W pliku
[`src/plant.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/src/plant.cpp)
gotowe są gałęzie `Infeed`, `PresenceCheck`, `Weighing` i `Output*`, w tym zerowanie licznika przy
przejściu z `Weighing` do `Diverting`. Do uzupełnienia pozostała gałąź `Diverting`, oznaczona
`// TODO`.

## Co masz napisać

Uzupełnij gałąź `Diverting` w `advance()` zgodnie z regułą opisaną powyżej.

## Sprawdź się

```bash
ctest --preset test -L misja-26
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza, czy pierwsza
nieudana próba powoduje `DiverterNotReady`, a druga z rzędu `RoutingDeadlineMissed`. Potwierdza też,
że `!routingReady` zeruje licznik, wznowienie pracy rozpoczyna liczenie od początku, a poprawnie
ustawiony dywerter kieruje paczkę do wyjścia i zeruje licznik.

## Częste błędy

- **Zachowanie wartości licznika przy `!routingReady`**: do limitu zostałyby wliczone ticki, w
  których skierowanie paczki nie było możliwe.
- **Zwrócenie `RoutingDeadlineMissed` po pierwszej nieudanej próbie**: dla wartości `<= 1` wynikiem
  ma być `DiverterNotReady`. Dopiero wartość `> 1` oznacza przekroczenie limitu.
- **Brak zerowania licznika po skierowaniu paczki do wyjścia**: kolejna paczka rozpoczęłaby pracę z
  wartością pozostałą po poprzedniej.

## Pytanie do zastanowienia

Dopuszczalne oczekiwanie wynosi jeden tick. Dla `divertingWaitTicks <= 1` program zwraca
`DiverterNotReady`, a dopiero dla `divertingWaitTicks > 1` zwraca `RoutingDeadlineMissed`. Dywerter
bez usterki potrzebuje najwyżej dwóch wywołań `resolve()`, aby osiągnąć cel z dowolnego stanu.
Prześledź, dlaczego żaden poprawny scenariusz z modułów 1–6 nie zgłosi
`RoutingDeadlineMissed`.

**Dalej:** [Misja 27: tryb awarii](./03_tryb_awarii.md).
