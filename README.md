🇵🇱 Polski | [🇬🇧 English](README.en.md)

# Programowanie systemów mechatronicznych

[![CI](https://github.com/michalmaj/mechatronic-systems-programming/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/michalmaj/mechatronic-systems-programming/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/github/license/michalmaj/mechatronic-systems-programming)](LICENSE)

Praktyczny kurs C++ dla studentów mechatroniki. W 35 misjach krok po kroku powstaje symulator
przemysłowej sortowni: od pierwszego `enum class` aż po samodzielny projekt końcowy.

## Dla kogo

Kurs jest przeznaczony dla studentów mechatroniki i kierunków pokrewnych. Nie wymaga wcześniejszej
znajomości C++. Wystarczą podstawy programowania w dowolnym języku. Zamiast serii niezależnych
ćwiczeń przez cały semestr rozwijasz ten sam system.

## Co zbudujesz

Paczki wjeżdżają na taśmę, przechodzą przez czujniki obecności i wagi, a dywerter kieruje je do
odpowiedniego wyjścia. W kolejnych modułach dodasz napęd taśmy, tryby pracy, awaryjny stop, obsługę
błędnych odczytów i usterek, ruch wielu paczek oraz scenariusze testowe. Wszystko działa na zwykłym
komputerze; do wykonania ćwiczeń nie potrzeba sprzętu laboratoryjnego.

## Jak wygląda nauka

- Rdzeń kursu obejmuje moduły 0–9, czyli łącznie 35 misji.
- Każdy moduł ma swój punkt startowy zapisany jako tag Git: `module-XX-start`. Tag
  `module-XX-solution` zawiera gotowe rozwiązanie. Możesz do niego zajrzeć, kiedy utkniesz, choć
  najwięcej nauczysz się, dochodząc do rozwiązania samodzielnie. Dokumentację czytaj na `main`,
  gdzie nanosimy poprawki i uzupełnienia. Tagi przechowują niezmienne wersje kodu dla poszczególnych
  modułów. Więcej informacji znajdziesz w [planie pracy](docs/roadmap.md#skąd-czytać-skąd-brać-kod).
- Symulacja jest sekwencyjna i powtarzalna: ten sam scenariusz zawsze daje ten sam wynik. Nie ma tu
  wątków ani losowości.
- CMake i CTest są już skonfigurowane. Korzystasz z nich, ale nie tworzysz konfiguracji od zera.
  Każda misja ma własny test (`ctest -L misja-N`). Wynik testu potwierdza wymagane zachowanie i
  kończy misję. Krótkie rozmowy opisane w planie pracy sprawdzają również zrozumienie rozwiązania.
- Na koniec samodzielnie rozbudujesz ukończony w trakcie kursu symulator.

## Szybki start

```bash
git clone https://github.com/michalmaj/mechatronic-systems-programming.git
cd mechatronic-systems-programming
git fetch --tags

git switch -c my-work module-01-start

cmake --preset dev
cmake --build --preset dev
ctest --preset test
```

Projekt działa na Windows, Linux i macOS. Każdy kolejny moduł zaczynasz na nowej gałęzi utworzonej
z odpowiedniego tagu `module-XX-start`. Szczegóły znajdziesz w planie pracy studenta.

## Gdzie dalej

- **[Plan pracy studenta](docs/roadmap.md):** kolejność pracy od modułu 0 do rozmowy podsumowującej
  projekt końcowy.
- **[Podręcznik](docs/handbook.md):** omówienie zagadnień z każdego modułu.
- **[Materiał modułów](course/README.md):** treść wszystkich misji i zasady pracy z kodem.
- **[Projekt końcowy](final_project/README.md):** bufor wejściowy dla linii sortującej. Materiał
  czytasz na `main`, a kod startowy bierzesz z tagu `final-project-start-v3`, tak jak przy
  modułach.
- **[Zgłoszenia](../../issues):** błędy w kodzie i testach, problemy w materiale oraz propozycje zmian.

## Status

Kurs jest w fazie pilotażu. Moduły 0–9 są ukończone i stabilne. Jeśli znajdziesz niespójność w
kodzie, testach lub materiale, zgłoś ją przez Issues.

## Licencja

Projekt jest udostępniany na licencji MIT. Szczegóły znajdują się w pliku [LICENSE](LICENSE).
