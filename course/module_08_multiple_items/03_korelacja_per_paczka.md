🇵🇱 Polski | [🇬🇧 English](03_korelacja_per_paczka.en.md)

# 8.3 Powiązanie odczytów z paczkami

## Problem

`ControllerState` zakładał, że w danej chwili przetwarzana jest tylko jedna paczka. Założenie było
poprawne, dopóki `Plant` przechowywał pojedynczy `Item`. Teraz w jednym ticku czujnik obecności może
badać paczkę w `presenceCheck`, a czujnik masy inną paczkę w `weighing`. Wspólny stan nie pozwala
jednoznacznie przypisać obu wyników.

## Nowe elementy C++

```cpp
void updatePresenceConfirmation(Item& itemAtPresenceCheck, PresenceReading presence);
void updateClassification(Item& itemAtWeighing, WeightReading weight);
```

Każda funkcja otrzymuje paczkę znajdującą się przy odpowiednim czujniku. Odczyt obecności zmienia
stan paczki w `presenceCheck`, a odczyt masy służy do klasyfikacji paczki w `weighing`. Druga funkcja
korzysta z `presenceConfirmed` zapisanej w tej samej paczce. Dzięki temu wynik pomiaru nie zostanie
przypisany do innego `ItemId`.

## Dokładne reguły

`updatePresenceConfirmation()` ustawia `itemAtPresenceCheck.presenceConfirmed` na `true` tylko wtedy,
gdy `status == ReadingStatus::Ok` oraz `occupied == true`. Przy błędnym odczycie albo
`occupied == false` nie zmienia pola. Raz potwierdzona obecność nie jest przez tę funkcję ponownie
ustawiana na `false`.

`updateClassification()` zapisuje wynik `decideClassification(weight)` w
`itemAtWeighing.classification` tylko po spełnieniu obu warunków:

- paczka ma już `presenceConfirmed == true`,
- odczyt masy ma `status == ReadingStatus::Ok`.

Jeśli którykolwiek warunek nie jest spełniony, `classification` zachowuje poprzednią wartość. Dla
nowej paczki pozostaje więc `std::nullopt`.

W misji 32 `Engine::step()` wywoła każdą z tych funkcji tylko przy zajętej strefie. Nie potrzebujesz
osobnej gałęzi zerującej stan przy braku paczki. `Item` utworzony przez `spawnItem()` zaczyna z
`presenceConfirmed = false` i `classification = std::nullopt`.

## Uproszczenie czujników

`PresenceSensor::read()` i `WeightSensor::read()` otrzymują teraz pole odpowiadające strefie, przy
której zamontowano dany czujnik. Obecność paczki można więc sprawdzić przez `item.has_value()`.
Porównanie `item->zone == Zone::PresenceCheck` nie jest już możliwe, ponieważ `Item` nie ma pola
`zone`.

Pozostałe reguły z modułu 6 nie zmieniają się. Jeśli strefa jest pusta i nie wystąpiła usterka,
czujnik zwraca odpowiednio `{Ok, 0}` albo `{Ok, false}`. Nie aktualizuje przy tym
`lastKnownMass_` ani `lastKnownOccupied_`. Prawidłowy odczyt pustej strefy informuje o braku paczki,
ale wartość zero nie jest zapamiętywana jako ostatnia masa paczki.

## Co już masz gotowe

W pliku
[`include/psm/controller.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/include/psm/controller.hpp)
znajdziesz deklaracje obu nowych funkcji.

Pliki
[`include/psm/presence_sensor.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/include/psm/presence_sensor.hpp)
i
[`include/psm/weight_sensor.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/include/psm/weight_sensor.hpp)
mają już właściwe deklaracje metod `read()`.

## Co masz napisać

- Zaimplementuj `updatePresenceConfirmation()` i `updateClassification()` w
  [`src/controller.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/controller.cpp)
  zgodnie z regułami opisanymi powyżej.
- W
  [`src/presence_sensor.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/presence_sensor.cpp)
  oraz
  [`src/weight_sensor.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/weight_sensor.cpp)
  określ stan rzeczywisty za pomocą `item.has_value()`, a w przypadku masy także `item->mass`.
  Pozostałą logikę z modułu 6 pozostaw bez zmian.

## Sprawdź się

```bash
ctest --preset test -L misja-31
```

Oczekiwany wynik: `100% tests passed`. Test sprawdza obie funkcje na osobnych obiektach `Item`, w tym
niezależną klasyfikację dwóch paczek przetwarzanych w tym samym ticku. Sprawdza także uproszczoną
logikę czujników i potwierdza, że odczyt pustej strefy nie nadpisuje zapamiętanej wartości.

## Częste błędy

- **Wywołanie `updateClassification()` bez sprawdzenia `presenceConfirmed` właściwej paczki**:
  prowadzi do przypisania wyniku bez wcześniejszego potwierdzenia obecności tej paczki.
- **Ponowne użycie `item->zone`**: kod się nie skompiluje, ponieważ położenie paczki wynika teraz z
  pola `Plant`, a nie z jej własnego pola.
- **Aktualizacja `lastKnownMass_` lub `lastKnownOccupied_` po prawidłowym odczycie pustej strefy**:
  taki odczyt nie zawiera nowej wartości dotyczącej paczki i nie powinien zastępować poprzedniej.

## Pytanie do zastanowienia

`updateClassification()` mogłaby otrzymywać `presenceConfirmed` jako osobny parametr. Dlaczego
odczytanie tej wartości bezpośrednio z `itemAtWeighing` lepiej chroni powiązanie danych z właściwą
paczką?

**Dalej:** [Misja 32: `Engine` z wieloma paczkami](./04_silnik_z_wieloma_paczkami.md).
