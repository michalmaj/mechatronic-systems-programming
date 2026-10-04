🇵🇱 Polski | [🇬🇧 English](01_zablokowany_dywerter.en.md)

# 7.1 Zablokowany dywerter

## Problem

W module 6 zasymulowaliśmy usterki czujników. Teraz zrobimy to samo dla dywertera. Jeśli mechanizm
jest zablokowany, jego rzeczywiste położenie nie może się zmienić. `Diverter::resolve()` musi to
uwzględniać.

## Nowy element C++

```cpp
enum class DiverterFaultKind { Blocked };
```

Usterki dywertera mają osobny typ. `SensorFaultKind` opisuje wyłącznie usterki czujników, takie jak
`Missing` i `Stale`. Dzięki temu kompilator nie pozwoli przekazać usterki dywertera do czujnika ani
usterki czujnika do dywertera. Jeden wspólny typ dopuszczałby bezsensowne połączenia, na przykład
zablokowany czujnik obecności.

```cpp
void resolve(std::optional<DiverterFaultKind> fault = std::nullopt);
```

## Dokładna reguła

Jeśli `fault == DiverterFaultKind::Blocked`, metoda `resolve()` kończy działanie bez zmiany
`actual_`. Dotyczy to również sytuacji, w której dywerter zatrzymał się w stanie `Moving`.

Bez usterki metoda ma działać dokładnie tak jak w module 2. Całą dotychczasową logikę trzech stanów
poprzedź jednym sprawdzeniem.

Nie zmieniaj `isSettled()`. Metoda porównuje rzeczywiste położenie dywertera z położeniem zadanym.
Jeśli mechanizm zostanie zablokowany przed osiągnięciem celu, zwróci `false`, czyli właściwy wynik.

## Co już masz gotowe

[`include/psm/diverter_fault_kind.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/include/psm/diverter_fault_kind.hpp)
zawiera gotowy typ usterki.

W pliku
[`include/psm/diverter.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/include/psm/diverter.hpp)
sygnatura `resolve()` jest już zaktualizowana.

W pliku
[`src/diverter.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/src/diverter.cpp)
znajdziesz dotychczasową logikę trzech stanów. Brakuje tylko sprawdzenia `Blocked` na początku
metody, w miejscu oznaczonym `// TODO`.

## Co masz napisać

Na początku `resolve()` sprawdź, czy `fault == DiverterFaultKind::Blocked`. Zrób to przed zmianą
któregokolwiek stanu dywertera.

## Sprawdź się

```bash
ctest --preset test -L misja-25
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test potwierdza, że przy usterce
`Blocked` metoda `resolve()` nie zmienia stanu dywertera, również po zatrzymaniu go w położeniu
`Moving`. Sprawdza też, czy bez usterki dywerter nadal działa tak jak w module 2.

## Częste błędy

- **Sprawdzenie samego `fault.has_value()` zamiast porównania z `DiverterFaultKind::Blocked`**:
  obecnie da taki sam wynik, bo typ ma tylko jedną wartość. Jawne porównanie jest jednak
  czytelniejsze i pozostanie poprawne po dodaniu kolejnych rodzajów usterek.
- **Sprawdzenie `Blocked` po dotychczasowej logice**: dywerter zdąży wtedy zmienić stan, zanim
  blokada zostanie uwzględniona.

## Pytanie do zastanowienia

`SensorFaultKind` ma dwie wartości (`Missing`, `Stale`), a `DiverterFaultKind` na razie tylko jedną
(`Blocked`). Dlaczego mimo to warto użyć osobnego typu wyliczeniowego zamiast parametru
`bool isBlocked`?

**Dalej:** [Misja 26: limit czasu na ustawienie dywertera](./02_termin_rutowania.md).
