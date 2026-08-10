# 6.1 Czujnik obecności

## Problem

Prawdziwy czujnik obecności to nie wszechwiedzące oko widzące cały przenośnik naraz — to urządzenie
zamontowane w jednym konkretnym miejscu. Nasz czujnik obecności "widzi" paczkę wyłącznie, gdy jest
ona w strefie `PresenceCheck`. Gdziekolwiek indziej — nawet jeśli paczka fizycznie istnieje gdzieś na
przenośniku — ten konkretny czujnik nic tam nie widzi.

## Nowy element C++

**`class PresenceSensor`** — czwarta klasa w kursie, i pierwsza z zupełnie innym niezmiennikiem niż
`Diverter`/`BeltMotor` (komenda-a-rzeczywistość). Tutaj niezmiennik brzmi: **nigdy nie udawaj, że
pamiętasz coś, czego nigdy naprawdę nie zaobserwowałeś.**

```cpp
class PresenceSensor {
public:
    PresenceReading read(const std::optional<Item>& item, std::optional<FaultKind> fault);

private:
    std::optional<bool> lastKnownOccupied_;
};
```

Zwróć uwagę: `lastKnownOccupied_` to `std::optional<bool>`, nie goły `bool`. Gdyby to był zwykły
`bool`, musiałby mieć jakąś wartość początkową (`false`?) — ale `false` oznaczałoby zarówno "sensor
naprawdę zaobserwował brak paczki", jak i "sensor jeszcze niczego nie zaobserwował" — dwie zupełnie
różne sytuacje, którym `bool` nie pozwala się odróżnić. `std::optional<bool>` rozwiązuje to wprost:
pusty `std::optional` znaczy "nigdy nic wiarygodnego nie widziałem."

## Dokładna reguła

**Ground truth** (prawda fizyczna, jaką ten czujnik może w ogóle zaobserwować):
`item.has_value() && item->zone == Zone::PresenceCheck`.

- **Bez usterki, paczka w `PresenceCheck`:** zapisz do `lastKnownOccupied_` i zwróć `{Ok, true}`.
- **Bez usterki, paczka gdzie indziej (albo jej nie ma):** zwróć `{Ok, false}` — to prawdziwy,
  bieżący odczyt ("nic tu teraz nie ma"), ale **nie jest to wartość do zapamiętania** — nie
  aktualizuj `lastKnownOccupied_`.
- **`Missing`:** zwróć `{Missing, false}`, nie dotykaj pamięci.
- **`Stale`:** jeśli `lastKnownOccupied_` ma wartość, zwróć `{Stale, *lastKnownOccupied_}`. **Jeśli
  nie ma** (nigdy nie było wiarygodnego odczytu do powtórzenia), zwróć `{Missing, false}` —
  degradacja `Stale` bez historii do `Missing`.

## Co już masz gotowe

[`include/psm/reading_status.hpp`](../../include/psm/reading_status.hpp),
[`include/psm/sensor_snapshot.hpp`](../../include/psm/sensor_snapshot.hpp),
[`include/psm/fault_kind.hpp`](../../include/psm/fault_kind.hpp),
[`include/psm/fault_target.hpp`](../../include/psm/fault_target.hpp) — wszystkie typy gotowe.

[`include/psm/presence_sensor.hpp`](../../include/psm/presence_sensor.hpp) — deklaracja klasy
kompletna, jak wyżej.

[`src/presence_sensor.cpp`](../../src/presence_sensor.cpp) — pusty szkielet z komentarzem `// TODO`.

## Co masz napisać

Uzupełnij ciało `PresenceSensor::read` zgodnie z dokładną regułą powyżej.

## Self-check

```bash
ctest --preset test -L misja-20
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test celowo sprawdza, że odczyt "gdzie
indziej" (paczka w `Weighing`, nie `PresenceCheck`) **nie** psuje wcześniej zapamiętanego dobrego
odczytu — dopiero potem `Stale` powtarza tamtą, wcześniejszą wartość.

## Częste błędy

- **Aktualizowanie `lastKnownOccupied_` na `false`, gdy paczka jest gdzie indziej** — to dokładnie
  błąd, którego ten test wyłapuje. Pamięć aktualizuje się wyłącznie z odczytu **w** `PresenceCheck`.
- **`Stale` zwracające jakąś wartość domyślną** zamiast degradować do `Missing`, gdy
  `lastKnownOccupied_` jest puste.
- **Sprawdzanie `item.has_value()` bez sprawdzenia strefy** — sam fakt istnienia paczki nigdzie w
  systemie to za mało; musi być akurat w `PresenceCheck`.

## Pytanie do zastanowienia

`Diverter` (Moduł 2) i `PresenceSensor` (ta misja) to obie klasy z prywatnym stanem. Czym różni się
**rodzaj** niezmiennika, który każda z nich chroni? Spróbuj sformułować to jednym zdaniem dla każdej.

**Dalej:** [Misja 21: czujnik wagi](./02_czujnik_wagi.md).
