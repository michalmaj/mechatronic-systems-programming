🇵🇱 Polski | [🇬🇧 English](README.en.md)

# Programowanie systemów mechatronicznych

[![CI](https://github.com/michalmaj/mechatronic-systems-programming/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/michalmaj/mechatronic-systems-programming/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/github/license/michalmaj/mechatronic-systems-programming)](LICENSE)

Praktyczny kurs C++ dla mechatroników: przez 35 misji budujesz, testujesz i rozbudowujesz **jeden,
deterministyczny symulator przemysłowej komórki sortującej** — od pierwszego `enum class` po własny
projekt końcowy.

## Dla kogo

Dla studentów mechatroniki i pokrewnych kierunków, którzy chcą nauczyć się nowoczesnego C++ na
konkretnym, rosnącym systemie — nie na oderwanych ćwiczeniach. Zakładamy pierwszy kontakt z C++;
podstawowa znajomość programowania (dowolny język) wystarczy na start.

## Co zbudujesz

Symulator komórki sortującej: paczki wjeżdżają na taśmę, są ważone, klasyfikowane i kierowane na
właściwe wyjście. Po drodze dochodzą: własna klasa aktuatora, orkiestrujący `Engine`, tryby pracy,
E‑Stop, czujniki z niepewnością odczytu, obsługa usterek, wiele paczek jednocześnie i deklaratywne
scenariusze testowe. Całość działa na zwykłym komputerze — **żaden sprzęt nie jest wymagany**.

## Jak wygląda nauka

- Kurs dzieli się na **Course Core (Moduły 0–9)**, złożony z **35 misji**.
- Każdy moduł ma swój punkt startowy jako tag Git: `module-XX-start`. Referencyjne rozwiązanie to
  `module-XX-solution` — materiał open-book, dostępny gdy utkniesz, ale największą wartość daje
  samodzielne dojście do rozwiązania.
- Symulacja jest **deterministyczna i sekwencyjna** — ten sam scenariusz zawsze daje ten sam wynik.
  Żadnych wątków, żadnej przypadkowości.
- `cmake`/`ctest` to dostarczona infrastruktura — konfigurujesz i uruchamiasz, nie budujesz jej od
  zera. Każda misja ma własny test (`ctest -L misja-N`), który jest jej ostatecznym kontraktem: misja
  jest zrobiona, gdy jej test jest zielony.
- Kurs kończy się samodzielnym **projektem końcowym** na bazie tego, co już zbudowałeś/aś.

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

Działa na Windows, Linux i macOS. Do każdego kolejnego modułu tworzysz nową gałąź z odpowiedniego
`module-XX-start` (dokładny workflow: patrz Roadmap).

## Gdzie dalej

- **[Roadmap / Plan pracy studenta](docs/roadmap.md)** — dokładna ścieżka od Modułu 0 do obrony
  projektu końcowego.
- **[Handbook / Podręcznik](docs/handbook.md)** — spójny materiał dydaktyczny do każdego modułu.
- **[Final Project](https://github.com/michalmaj/mechatronic-systems-programming/blob/final-project-start/final_project/README.md)** *(dostępne od tagu `final-project-start` — ten link nie zadziała na `main`)* — projekt
  końcowy: bufor wejściowy dla linii sortującej.
- **[Issues](../../issues)** — błędy w kodzie/testach, problemy w materiale, propozycje.

## Licencja i status

Kurs jest w fazie pilotażu. Materiał Course Core 0–9 jest zamknięty i stabilny; jeśli coś w kodzie,
testach lub materiale wygląda na niespójne — zgłoś przez Issues.
