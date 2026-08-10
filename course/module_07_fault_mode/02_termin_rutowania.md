# 7.2 Termin rutowania

To najważniejsza misja tego modułu pod względem teorii. Przeczytaj ją całą, zanim zaczniesz pisać
kod.

## Problem

`Plant` czeka w strefie `Diverting`, aż dywerter się ustawi — dziś bez żadnego limitu czasu i bez
żadnego sposobu, żeby ktokolwiek się dowiedział, że czeka za długo. Zablokowany dywerter z Misji 25
zamraża paczkę na zawsze, po cichu.

## Nowy element C++

```cpp
enum class SystemEventKind { DiverterNotReady, RoutingDeadlineMissed };
```

Wprowadzamy ten typ dokładnie tutaj, gdzie od razu znajduje zastosowanie — nie jako osobna, pusta
misja sama w sobie.

`Plant` zyskuje licznik:

```cpp
struct Plant {
    std::optional<Item> item;
    int divertingWaitTicks = 0;
};
```

`advance()` zamienia swój typ zwracany z `void` na `std::optional<SystemEventKind>` — to zmiana
darmowa i wsteczna: każde dotychczasowe wywołanie (`plant_test.cpp`, `plant_diverter_test.cpp`,
`loop.cpp`, `Engine::step()`) woła `advance(...)` jako samodzielną instrukcję, odrzucając wynik — a
to zostaje legalnym C++ niezależnie od typu zwracanego. Żadne z tych wywołań nie wymaga edycji.

## Dokładna reguła

W gałęzi `Diverting`:

- Jeśli `!routingReady`: **zresetuj `divertingWaitTicks` do `0`** i zwróć `std::nullopt`. **Nie**
  zamrażaj licznika — patrz niżej, dlaczego to ważne.
- Jeśli `routingReady`, ale `!diverter.isSettled()`: zwiększ `divertingWaitTicks`; zwróć
  `DiverterNotReady`, jeśli licznik wciąż `<= 1`, `RoutingDeadlineMissed`, jeśli licznik przekroczył
  `1`.
- Jeśli ustawiony: rutuj normalnie, zresetuj licznik do `0`.

Licznik resetuje się też przy przejściu `Weighing` → `Diverting` — świeża paczka zaczyna z czystym
licznikiem, niezależnie od tego, co zostało po poprzedniej.

## Dlaczego termin mierzy wyłącznie *ciągłą, aktywną* próbę

To jest sedno tej misji. Wyobraź sobie, że `!routingReady` **zamrażałoby** licznik zamiast go
zerować. Wtedy przerwa niezwiązana z samym dywerterem — e-stop, tryb, brakująca jeszcze klasyfikacja
— po cichu zjadałaby część limitu czasu dywertera. Parcela mogłaby dostać `RoutingDeadlineMissed` nie
dlatego, że dywerter naprawdę utknął, tylko dlatego, że coś zupełnie innego wstrzymało próby
rutowania na kilka ticków wcześniej. To pomieszałoby dwie zupełnie różne przyczyny w jeden sygnał.

Zerowanie przy `!routingReady` gwarantuje, że termin mierzy dokładnie to, co ma mierzyć: ile ticków
z rzędu dywerter dostawał **realną** szansę się ustawić i jej nie wykorzystał.

## Co już masz gotowe

[`include/psm/system_event_kind.hpp`](../../include/psm/system_event_kind.hpp) — typ gotowy.

[`include/psm/plant.hpp`](../../include/psm/plant.hpp) — `divertingWaitTicks` i nowa sygnatura
`advance()` już obecne.

[`src/plant.cpp`](../../src/plant.cpp) — gałęzie `Infeed`/`PresenceCheck`/`Weighing`/`Output*`
kompletne i niezmienione (w tym reset licznika przy `Weighing`→`Diverting`); tylko własna logika
gałęzi `Diverting` jest `// TODO`.

## Co masz napisać

Uzupełnij gałąź `Diverting` w `advance()` zgodnie z dokładną regułą powyżej.

## Self-check

```bash
ctest --preset test -L misja-26
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza: `DiverterNotReady`
przy pierwszej nieudanej aktywnej próbie, `RoutingDeadlineMissed` przy drugiej z rzędu; że
`!routingReady` zeruje licznik zamiast go zamrażać (i że rutowanie wznowione po takiej przerwie
dostaje pełny, świeży termin); oraz że ustawiony dywerter rutuje normalnie i zeruje licznik.

## Częste błędy

- **Zamrażanie licznika zamiast zerowania go przy `!routingReady`** — dokładnie problem opisany
  wyżej.
- **Zwracanie `RoutingDeadlineMissed` już przy pierwszej nieudanej próbie** — sprawdź uważnie
  kolejność: `<= 1` to `DiverterNotReady`, dopiero `> 1` to przekroczenie terminu.
- **Zapominanie o zerowaniu licznika po udanym rutowaniu** — kolejna paczka odziedziczyłaby cudzy,
  częściowo zużyty licznik.

## Pytanie do zastanowienia

Termin karencji wynosi efektywnie jeden tick (`DiverterNotReady` przy count `<= 1`,
`RoutingDeadlineMissed` dopiero przy count `> 1`). Dywerter bez usterki potrzebuje najwyżej dwóch
wywołań `resolve()`, żeby się ustawić z dowolnego stanu. Prześledź, dlaczego to sprawia, że żaden
istniejący, nieusterkowy scenariusz z Modułów 1–6 nigdy nie zgłosi `RoutingDeadlineMissed`.

**Dalej:** [Misja 27: tryb awarii](./03_tryb_awarii.md).
