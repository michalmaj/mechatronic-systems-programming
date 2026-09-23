# 6.2 Czujnik wagi

## Problem

Waga ma dokładnie tę samą fizyczną ograniczoność co czujnik obecności z poprzedniej misji — tyle że
zamontowana jest w innym miejscu: widzi paczkę wyłącznie w strefie `Weighing`.

## Nowy element C++

**`class WeightSensor`** — drugie zastosowanie wzorca z Misji 20, tym razem dla `Grams` zamiast
`bool`, mniej prowadzone.

```cpp
class WeightSensor {
public:
    WeightReading read(const std::optional<Item>& item, std::optional<FaultKind> fault);

private:
    std::optional<Grams> lastKnownMass_;
};
```

**Stan rzeczywisty:** `item.has_value() && item->zone == Zone::Weighing` → `item->mass`.

Reguła jest strukturalnie identyczna z `PresenceSensor`:
- Bez usterki, na wadze: zapisz do `lastKnownMass_`, zwróć `{Ok, item->mass}`.
- Bez usterki, nie na wadze: zwróć `{Ok, 0}` — nic tu teraz nie ma do zważenia; **nie** aktualizuj
  pamięci (zero nie jest prawdziwą masą paczki, tylko brakiem pomiaru).
- `Missing`: `{Missing, 0}`, pamięć nietknięta.
- `Stale`: `{Stale, *lastKnownMass_}` jeśli pamięć ma wartość, w przeciwnym razie `{Missing, 0}`.

## Co już masz gotowe

[`include/psm/weight_sensor.hpp`](../../include/psm/weight_sensor.hpp) — deklaracja kompletna.

[`src/weight_sensor.cpp`](../../src/weight_sensor.cpp) — pusty szkielet.

## Co masz napisać

Uzupełnij ciało `WeightSensor::read` zgodnie z regułą powyżej.

## Sprawdź się

```bash
ctest --preset test -L misja-21
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`.

## Częste błędy

- Te same, co w Misji 20: aktualizowanie pamięci z odczytu "nie na wadze"; brak degradacji `Stale`
  do `Missing` bez wcześniejszej historii.
- **Pomylenie "zero gramów" z "brak pomiaru"** — `{Ok, 0}` dla nieobecnej paczki to poprawny,
  bieżący odczyt, ale nigdy nie powinien trafić do `lastKnownMass_` jako "prawdziwa" masa.

## Pytanie do zastanowienia

Gdyby ktoś zaproponował, żeby `WeightSensor` w ogóle nie sprawdzał strefy — po prostu zwracał
`item->mass`, kiedykolwiek paczka istnieje, niezależnie gdzie — co dokładnie by to zepsuło w dalszych
misjach tego modułu?

**Dalej:** [Misja 22: klasyfikacja odporna na awarie](./03_klasyfikacja_odporna_na_awarie.md).
