🇵🇱 Polski | [🇬🇧 English](00_wprowadzenie.en.md)

# 4.0 Wprowadzenie

Po module 3 klasa `Engine` jest jedynym miejscem, które określa kolejność operacji w ticku. Teraz
rozbudujesz ją o dwa elementy: silnik przenośnika (`BeltMotor`) oraz tryb pracy (`Mode`), który
opisuje stan **całego systemu**, a nie pojedynczej paczki.

## Skąd startujesz

```bash
git fetch --tags
git switch -c <nazwa-twojego-brancha> module-04-start
```

Do ostatniej misji nie zmieniaj `Engine::step()`. Najpierw zbudujesz i osobno przetestujesz
`BeltMotor` (misja 13) oraz `Mode` (misja 14). Połączysz je z `Engine` dopiero w misji 15.

## O teście `misja-11`

Test `misja-11` sprawdza teraz jedynie licznik ticków pustego `Engine`. Poprzednia wersja oczekiwała
konkretnych stref w kolejnych tickach, zakładając natychmiastowy ruch paczki. Po dodaniu napędu takie
założenie przestaje obowiązywać, dlatego test został zawężony.

## Mapa modułu

1. **Silnik przenośnika:** `BeltMotor`, czyli kolejny przykład rozdzielenia polecenia od
   rzeczywistego stanu urządzenia.
2. **Tryb pracy:** `Mode`, czyli stan całego systemu. Tym razem wystarczy typ wyliczeniowy i wolna
   funkcja, bez osobnej klasy przechowującej stan.
3. **Przenośnik pod kontrolą trybu:** połączenie nowych elementów z `Engine`. Paczka rusza dopiero,
   gdy silnik przenośnika osiągnie stan `Running`.

## Zanim zaczniesz

- Testy modułów 1–3 (`misja-1`–`misja-4`, `misja-6`–`misja-11`) są już obecne i przechodzą.
- Tak jak zawsze: nie edytujesz plików testowych ani `CMakeLists.txt`.

**Dalej:** [Misja 13: silnik przenośnika](./01_silnik_przenosnika.md).
