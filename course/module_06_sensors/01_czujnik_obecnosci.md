🇵🇱 Polski | [🇬🇧 English](01_czujnik_obecnosci.en.md)

# 6.1 Czujnik obecności

## Problem

Czujnik obecności jest zamontowany w jednym miejscu i nie obserwuje całego przenośnika. Wykrywa
paczkę wyłącznie wtedy, gdy znajduje się ona w strefie `PresenceCheck`. Poza tą strefą jego bieżący
odczyt nie potwierdza obecności paczki, nawet jeśli paczka znajduje się w innym miejscu systemu.

## Nowy element C++

**`class PresenceSensor`** chroni inny rodzaj niezmiennika niż `Diverter` i `BeltMotor`. Zapamiętana
wartość może pochodzić wyłącznie z rzeczywistego, wiarygodnego odczytu w strefie czujnika.

```cpp
class PresenceSensor {
public:
    PresenceReading read(const std::optional<Item>& item, std::optional<FaultKind> fault);

private:
    std::optional<bool> lastKnownOccupied_;
};
```

Pole `lastKnownOccupied_` ma typ `std::optional<bool>`, a nie zwykły `bool`. Początkowe `false`
mogłoby oznaczać zarówno wiarygodny odczyt braku paczki, jak i brak jakiegokolwiek wcześniejszego
odczytu. Są to dwie różne sytuacje. Pusty `std::optional` jednoznacznie informuje, że czujnik nie ma
jeszcze zapamiętanej wiarygodnej wartości.

## Dokładna reguła

Warunek rzeczywistej obecności paczki w miejscu pomiaru:
`item.has_value() && item->zone == Zone::PresenceCheck`.

- **Bez usterki, paczka w `PresenceCheck`:** zapisz do `lastKnownOccupied_` i zwróć `{Ok, true}`.
- **Bez usterki, paczka gdzie indziej albo jej nie ma:** zwróć `{Ok, false}`. Jest to bieżący odczyt
  dla strefy czujnika, ale **nie należy go zapamiętywać**. Nie aktualizuj `lastKnownOccupied_`.
- **`Missing`:** zwróć `{Missing, false}`, nie dotykaj pamięci.
- **`Stale`:** jeśli `lastKnownOccupied_` ma wartość, zwróć `{Stale, *lastKnownOccupied_}`. **Jeśli
  jej nie ma**, zwróć `{Missing, false}`, ponieważ czujnik nie ma wcześniejszego odczytu, który
  mógłby powtórzyć.

## Co już masz gotowe

[`include/psm/reading_status.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/include/psm/reading_status.hpp),
[`include/psm/sensor_snapshot.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/include/psm/sensor_snapshot.hpp),
[`include/psm/fault_kind.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/include/psm/fault_kind.hpp),
[`include/psm/fault_target.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/include/psm/fault_target.hpp)
zawierają gotowe typy używane w tej misji.

[`include/psm/presence_sensor.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/include/psm/presence_sensor.hpp)
zawiera kompletną deklarację klasy pokazaną wyżej.

W pliku
[`src/presence_sensor.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/src/presence_sensor.cpp)
znajdziesz pusty szkielet metody z komentarzem `// TODO`.

## Co masz napisać

Uzupełnij ciało `PresenceSensor::read` zgodnie z dokładną regułą powyżej.

## Sprawdź się

```bash
ctest --preset test -L misja-20
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza również, że odczyt poza
strefą czujnika, na przykład dla paczki w `Weighing`, **nie** nadpisuje wcześniej zapamiętanej
wartości. Późniejszy odczyt `Stale` powinien ją powtórzyć.

## Częste błędy

- **Aktualizowanie `lastKnownOccupied_` na `false`, gdy paczka jest gdzie indziej:** pamięć może
  być aktualizowana wyłącznie na podstawie odczytu **w** `PresenceCheck`.
- **Zwrócenie przez `Stale` wartości domyślnej**, gdy `lastKnownOccupied_` jest puste: w tej
  sytuacji należy zwrócić `Missing`.
- **Sprawdzanie `item.has_value()` bez sprawdzenia strefy:** sama obecność paczki w systemie nie
  wystarcza. Musi znajdować się w `PresenceCheck`.

## Pytanie do zastanowienia

`Diverter` z modułu 2 i `PresenceSensor` to klasy z prywatnym stanem. Czym różni się
**rodzaj** niezmiennika, który każda z nich chroni? Spróbuj sformułować to jednym zdaniem dla każdej.

**Dalej:** [Misja 21: czujnik wagi](./02_czujnik_wagi.md).
