🇵🇱 Polski | [🇬🇧 English](03_korelacja_per_paczka.en.md)

# 8.3 Korelacja per paczka

## Problem

`ControllerState` zakładał, że w danej chwili co najwyżej jedna paczka jest "w trakcie potwierdzania".
To założenie było prawdziwe, dopóki `Plant` mieścił jedną paczkę. Teraz jest fałszywe: odczyt obecności
i odczyt wagi w tym samym ticku mogą dotyczyć zupełnie różnych paczek — jednej w `presenceCheck`, innej
w `weighing`. Jedna, globalna zmienna stanu nie potrafi tego wyrazić poprawnie.

## Nowe elementy C++

```cpp
void updatePresenceConfirmation(Item& itemAtPresenceCheck, PresenceReading presence);
void updateClassification(Item& itemAtWeighing, WeightReading weight);
```

Dwie funkcje, nie jedna. Odczyt obecności dotyczy zawsze tej paczki, która akurat jest w
`presenceCheck`. Odczyt wagi dotyczy zawsze tej paczki, która akurat jest w `weighing` — i korzysta z
**jej własnego** `presenceConfirmed`, nie jakiegoś globalnego. To jest właściwa lekcja tej misji: gdy
paczek jest kilka, żaden odczyt czujnika nie może po cichu "przeciekać" do niewłaściwej paczki.

## Dokładna reguła

`updatePresenceConfirmation` ustawia `itemAtPresenceCheck.presenceConfirmed` na `true` wtedy i tylko
wtedy, gdy przekazany odczyt ma jednocześnie `status == ReadingStatus::Ok` i `occupied == true`. W
każdym innym przypadku — odczyt błędny albo `occupied == false` — pole zostaje bez zmian; funkcja
nigdy nie resetuje `presenceConfirmed` z powrotem na `false`.

`updateClassification` zapisuje wynik `decideClassification(weight)` do
`itemAtWeighing.classification` wyłącznie wtedy, gdy jednocześnie: paczka w `weighing` ma już
`presenceConfirmed == true`, i przekazany odczyt wagi ma `status == ReadingStatus::Ok`. Brak
któregokolwiek z tych dwóch warunków oznacza brak zapisu — `classification` zostaje takie, jakie
było wcześniej (dla świeżej paczki: `std::nullopt`).

`Engine::step()` (Misja 32) wywoła każdą z nich tylko wtedy, gdy odpowiedni slot jest faktycznie
zajęty — nie ma tu żadnej gałęzi "resetuj przy braku paczki", bo `Item` utworzony przez `spawnItem`
już zaczyna z domyślnym, czystym stanem (`presenceConfirmed = false`, `classification =
std::nullopt`) — nie ma nic do zerowania.

## Uproszczenie czujników

Skoro `PresenceSensor::read`/`WeightSensor::read` dostają teraz dokładnie ten slot, do którego są
fizycznie przypięte, stan rzeczywisty to po prostu `item.has_value()` — dawne porównanie `item->zone ==
Zone::PresenceCheck` nie tylko nie jest już potrzebne, jest niemożliwe (`Item` nie ma już `zone`).
Reszta logiki z Modułu 6 zostaje bez zmian, i to jest tu ważne: gdy slot jest pusty i nie ma usterki,
sensor nadal zwraca `{Ok, 0}`/`{Ok, false}`, **nie** aktualizując `lastKnownMass_`/`lastKnownOccupied_`.
Potwierdzony pusty odczyt to prawdziwa, zaufana obserwacja — nigdy nie jest traktowany jako "masa
wynosi zero" do celów pamięci.

## Co już masz gotowe

[`include/psm/controller.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/include/psm/controller.hpp) — deklaracje obu nowych funkcji,
gotowe. [`include/psm/presence_sensor.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/include/psm/presence_sensor.hpp)/[`weight_sensor.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/include/psm/weight_sensor.hpp)
— sygnatury bez zmian.

## Co masz napisać

- `updatePresenceConfirmation`/`updateClassification` w [`src/controller.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/controller.cpp)
  zgodnie z dokładną regułą powyżej.
- `PresenceSensor::read` w [`src/presence_sensor.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/presence_sensor.cpp) i
  `WeightSensor::read` w [`src/weight_sensor.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/weight_sensor.cpp): zamień stan rzeczywisty na
  `item.has_value()` (i `item->mass` dla wagi), zachowując resztę logiki Modułu 6 bez zmian.

## Sprawdź się

```bash
ctest --preset test -L misja-31
```

Oczekiwany wynik: `100% tests passed`. Sprawdza: obie funkcje korelacji bezpośrednio na gołych
`Item`ach, w tym że dwie różne, jednocześnie przetwarzane paczki dostają w pełni niezależną
klasyfikację; uproszczoną logikę obu czujników, w tym że potwierdzony pusty odczyt nigdy nie zeruje
pamięci.

## Częste błędy

- **Wywołanie `updateClassification` niezależnie od `presenceConfirmed` tej konkretnej paczki** — to
  właśnie ten global-state bug, którego ta misja uczy unikać.
- **Powrót do porównania `item->zone`** — niemożliwe do skompilowania, ale warto zauważyć,
  dlaczego: `Item` nie niesie już tej informacji.
- **Aktualizowanie `lastKnownMass_`/`lastKnownOccupied_` przy potwierdzonym pustym odczycie** — to
  właśnie regresja, przed którą ta sekcja explicite ostrzega.

## Pytanie do zastanowienia

Gdyby `updateClassification` przyjmowała `bool presenceConfirmed` jako osobny parametr zamiast czytać
je z `itemAtWeighing`, dawałoby to dokładnie ten sam wynik dzisiaj. Dlaczego mimo to czytanie go
bezpośrednio z paczki jest tu lepszym wyborem projektowym?

**Dalej:** [Misja 32: silnik z wieloma paczkami](./04_silnik_z_wieloma_paczkami.md).
