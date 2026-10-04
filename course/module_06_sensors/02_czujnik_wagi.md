🇵🇱 Polski | [🇬🇧 English](02_czujnik_wagi.en.md)

# 6.2 Czujnik wagi

## Problem

Czujnik wagi ma takie samo ograniczenie jak czujnik obecności z poprzedniej misji, ale jest
zamontowany w innym miejscu. Może zważyć paczkę wyłącznie w strefie `Weighing`.

## Nowy element C++

**`class WeightSensor`** stosuje rozwiązanie z misji 20 do wartości typu `Grams` zamiast `bool`.
Tym razem instrukcja jest mniej szczegółowa.

```cpp
class WeightSensor {
public:
    WeightReading read(const std::optional<Item>& item, std::optional<FaultKind> fault);

private:
    std::optional<Grams> lastKnownMass_;
};
```

Warunek wykonania rzeczywistego pomiaru:
`item.has_value() && item->zone == Zone::Weighing`. Jego wynikiem jest `item->mass`.

Reguła jest strukturalnie identyczna z `PresenceSensor`:
- Bez usterki, na wadze: zapisz do `lastKnownMass_`, zwróć `{Ok, item->mass}`.
- Bez usterki, paczka poza wagą albo nieobecna: zwróć `{Ok, 0}`, ale **nie** aktualizuj pamięci.
  Zero oznacza tutaj brak pomiaru, a nie rzeczywistą masę paczki.
- `Missing`: `{Missing, 0}`, pamięć nietknięta.
- `Stale`: `{Stale, *lastKnownMass_}` jeśli pamięć ma wartość, w przeciwnym razie `{Missing, 0}`.

## Co już masz gotowe

[`include/psm/weight_sensor.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/include/psm/weight_sensor.hpp)
zawiera kompletną deklarację klasy.

W pliku
[`src/weight_sensor.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/src/weight_sensor.cpp)
znajdziesz pusty szkielet metody.

## Co masz napisać

Uzupełnij ciało `WeightSensor::read` zgodnie z regułą powyżej.

## Sprawdź się

```bash
ctest --preset test -L misja-21
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`.

## Częste błędy

- **Aktualizowanie pamięci na podstawie odczytu poza wagą:** tak jak w misji 20, pamięć można
  zmieniać tylko na podstawie pomiaru w odpowiedniej strefie.
- **Zwrócenie `Stale` bez wcześniejszego wiarygodnego pomiaru:** w tej sytuacji zwróć `Missing`.
- **Pomylenie zera gramów z brakiem pomiaru:** `{Ok, 0}` dla nieobecnej paczki nie powinno trafić do
  `lastKnownMass_` jako rzeczywista masa.

## Pytanie do zastanowienia

Co zepsułoby się w dalszych misjach, gdyby `WeightSensor` zwracał `item->mass` zawsze, gdy paczka
istnieje, bez sprawdzania jej strefy?

**Dalej:** [Misja 22: klasyfikacja odporna na awarie](./03_klasyfikacja_odporna_na_awarie.md).
