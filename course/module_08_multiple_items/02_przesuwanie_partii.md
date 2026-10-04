🇵🇱 Polski | [🇬🇧 English](02_przesuwanie_partii.en.md)

# 8.2 Przesuwanie paczek

W tej misji kolejność operacji ma bezpośredni wpływ na działanie symulacji. Przeczytaj wszystkie
reguły przed rozpoczęciem implementacji.

## Problem

`Plant` może teraz przechowywać kilka paczek. Jedno wywołanie `advance()` powinno przesunąć każdą z
nich najwyżej raz. Jednocześnie paczka może od razu wejść do strefy zwolnionej w tym samym ticku
przez paczkę znajdującą się przed nią.

## Nowe elementy C++

```cpp
struct ItemDeparture {
    ItemId id;
    Zone destination;
};

struct AdvanceResult {
    std::optional<SystemEventKind> event;
    std::optional<ItemDeparture> departure;
};

AdvanceResult advance(Plant& plant, const Diverter& diverter, bool routingReady = true);
```

`event` dotyczy paczki w `diverting`, która nie opuściła układu w bieżącym ticku. `departure` jest
ustawiane tylko wtedy, gdy paczka została skierowana do wyjścia. Te dwa pola nigdy nie zawierają
jednocześnie wartości.

## Kolejność przejść

`advance()` wykonuje cztery przejścia. Każde z nich wykonaj dokładnie raz, w kolejności od wyjścia
do wejścia.

**1. Obsługa strefy `diverting`.**

Ten krok wykonaj tylko wtedy, gdy strefa jest zajęta. Obowiązuje reguła limitu czasu z misji 26, ale
licznik `divertingWaitTicks` jest teraz polem konkretnej paczki.

- Jeśli `routingReady` ma wartość `false`, wyzeruj licznik paczki. Pozostaw ją w `diverting` i nie
  zgłaszaj zdarzenia.
- Jeśli `routingReady` ma wartość `true`, ale `!diverter.isSettled()`, zwiększ licznik o jeden. Przy
  pierwszym takim ticku ustaw `SystemEventKind::DiverterNotReady`. Przy każdym następnym ustaw
  `SystemEventKind::RoutingDeadlineMissed`.
- Jeśli dywerter osiągnął zadane położenie, paczka opuszcza układ. Zapisz w `result.departure` jej
  identyfikator oraz strefę docelową. Dla położenia `Straight` jest to `Zone::OutputLight`, a w
  przeciwnym razie `Zone::OutputHeavy`. Następnie zwolnij pole `diverting`.

**2–4. Przejścia między sąsiednimi strefami.**

Wykonaj kolejno:

1. `weighing` → `diverting`,
2. `presenceCheck` → `weighing`,
3. `infeed` → `presenceCheck`.

Dla każdego przejścia sprawdź, czy strefa docelowa jest pusta, a źródłowa zajęta. Jeśli tak,
przenieś paczkę i zwolnij strefę źródłową. Uwzględniaj aktualny stan pól, ponieważ wcześniejszy krok
tego samego wywołania mógł właśnie zwolnić miejsce.

Przy wejściu paczki do `diverting` wyzeruj jej `divertingWaitTicks`. Jest to początek nowego
odliczania dla tej paczki.

## Dlaczego paczka przesuwa się najwyżej raz w ticku

Decyduje o tym kolejność od wyjścia do wejścia. Paczka przeniesiona do następnej strefy nie zostanie
obsłużona przez wcześniejsze przejście, ponieważ ten krok już się zakończył. Przykładowo paczka
przeniesiona z `weighing` do `diverting` nie może w tym samym wywołaniu opuścić układu, ponieważ
obsługa `diverting` odbyła się wcześniej.

Jednocześnie każdy krok sprawdza bieżącą zajętość strefy docelowej. Jeżeli wcześniejszy krok zwolnił
tę strefę, następna paczka może od razu do niej wejść. W jednym ticku może więc przesunąć się kilka
paczek, ale każda z nich tylko o jedną strefę.

## Która paczka wyznacza polecenie dywertera

W `Engine::step()` polecenie dywertera zawsze wynika z klasyfikacji paczki, która już znajduje się w
`diverting`. Nie korzystaj z klasyfikacji paczki znajdującej się w `weighing`, nawet jeśli została
wyznaczona w tym samym ticku. Decyzja o położeniu dywertera zapada przed wywołaniem `advance()`, więc
ta paczka nie zdążyła jeszcze wejść do `diverting`.

## Przykładowy przebieg: trzy paczki Light/Heavy/Light

```text
tick 0: infeed=2 presenceCheck=1 weighing=- diverting=-  cmd=Hold(for -)      event=-                 departure=-
tick 1: infeed=3 presenceCheck=2 weighing=1 diverting=-  cmd=Hold(for -)      event=-                 departure=-
tick 2: infeed=- presenceCheck=3 weighing=2 diverting=1  cmd=Hold(for -)      event=-                 departure=-
tick 3: infeed=- presenceCheck=- weighing=3 diverting=2  cmd=Hold(for 1)      event=-                 departure=1->Light
tick 4: infeed=- presenceCheck=- weighing=3 diverting=2  cmd=Divert(for 2)    event=DiverterNotReady  departure=-
tick 5: infeed=- presenceCheck=- weighing=- diverting=3  cmd=Divert(for 2)    event=-                 departure=2->Heavy
tick 6: infeed=- presenceCheck=- weighing=- diverting=3  cmd=Hold(for 3)      event=DiverterNotReady  departure=-
tick 7: infeed=- presenceCheck=- weighing=- diverting=-  cmd=Hold(for 3)      event=-                 departure=3->Light
```

Ten przebieg pokazuje bezpośrednie wywołania `advance()` dla samego `Plant`, bez czasu potrzebnego
na rozpędzenie taśmy przez `Engine`. W pełnej symulacji numery ticków będą inne, ale kolejność zdarzeń
pozostanie taka sama.

W ticku 3 paczka 1 opuszcza układ, a paczka 2 wchodzi do zwolnionego pola `diverting`. Polecenie
dywertera nadal dotyczy paczki 1, ponieważ zostało wyznaczone przed wywołaniem `advance()`.

## Co już masz gotowe

W pliku
[`include/psm/plant.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/include/psm/plant.hpp)
znajdziesz typy `ItemDeparture` i `AdvanceResult` oraz deklarację `advance()`. Masz też własną
implementację `spawnItem()` z misji 29.

## Co masz napisać

Zaimplementuj `advance()` w
[`src/plant.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/plant.cpp).
Wykonaj cztery opisane przejścia, każde dokładnie raz i w podanej kolejności.

## Sprawdź się

```bash
ctest --preset test -L misja-30
```

Oczekiwany wynik: `100% tests passed`. `plant_diverter_test` sprawdza przejście paczki przez cztery
strefy, gdy polecenie dywertera pojawia się z opóźnieniem. `plant_deadline_test` sprawdza licznik
`divertingWaitTicks`, w tym jego zerowanie przy `!routingReady`.

## Częste błędy

- **Obsługa stref od wejścia do wyjścia**: paczka mogłaby wtedy przejść przez kilka stref podczas
  jednego wywołania `advance()`.
- **Brak zerowania `divertingWaitTicks` przy wejściu do `diverting`**: odliczanie dla nowego pobytu w
  tej strefie nie zaczęłoby się od zera.
- **Wyznaczenie polecenia dywertera na podstawie paczki w `weighing`**: polecenie ma dotyczyć paczki,
  która już znajduje się w `diverting`.

## Pytanie do zastanowienia

Co stałoby się z paczkami w pokazanym przebiegu, gdyby `advance()` obsługiwało strefy od wejścia do
wyjścia? Wskaż tick, w którym wynik zacząłby się różnić.

**Dalej:** [Misja 31: powiązanie odczytów z paczkami](./03_korelacja_per_paczka.md).
