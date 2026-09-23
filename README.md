🇵🇱 Polski | [🇬🇧 English](README.en.md)

# Programowanie systemów mechatronicznych

[![CI](https://github.com/michalmaj/mechatronic-systems-programming/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/michalmaj/mechatronic-systems-programming/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/github/license/michalmaj/mechatronic-systems-programming)](LICENSE)

Praktyczny kurs C++ dla mechatroników. Przez 35 misji budujesz, testujesz i rozbudowujesz jeden
deterministyczny symulator przemysłowej komórki sortującej — od pierwszego `enum class` po własny
projekt końcowy.

## Dla kogo

Dla studentów mechatroniki i pokrewnych kierunków, którzy chcą nauczyć się nowoczesnego C++ na
konkretnym, rosnącym systemie, a nie na oderwanych ćwiczeniach. Zakładamy pierwszy kontakt z C++;
podstawowa znajomość programowania w dowolnym języku wystarczy na start.

## Co zbudujesz

Symulator komórki sortującej: paczki wjeżdżają na taśmę, są ważone, klasyfikowane i kierowane na
właściwe wyjście. Po drodze dochodzą kolejne elementy — własna klasa aktuatora, orkiestrujący
`Engine`, tryby pracy, awaryjny stop, czujniki z niepewnym odczytem, obsługa usterek, wiele paczek
naraz i deklaratywne scenariusze testowe. Całość działa na zwykłym komputerze, bez żadnego sprzętu.

## Jak wygląda nauka

- Kurs dzieli się na rdzeń (Course Core, moduły 0–9), złożony z 35 misji.
- Każdy moduł ma swój punkt startowy jako tag Git: `module-XX-start`. Do każdego jest też gotowe
  rozwiązanie, `module-XX-solution` — możesz z niego korzystać, kiedy utkniesz, ale najwięcej
  zyskujesz, dochodząc do rozwiązania samodzielnie.
- Symulacja jest deterministyczna i sekwencyjna: ten sam scenariusz zawsze daje ten sam wynik, bez
  wątków i bez przypadkowości.
- CMake i CTest to gotowa infrastruktura — konfigurujesz ją i uruchamiasz, nie budujesz od zera.
  Każda misja ma własny test (`ctest -L misja-N`). Zielony test potwierdza wymagane zachowanie i
  kończy misję, ale sam w sobie nie dowodzi zrozumienia — to sprawdzają checkpointy, opisane w
  planie pracy.
- Kurs kończy się samodzielnym projektem, budowanym na tym, co już masz gotowe.

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

Działa na Windows, Linux i macOS. Do każdego kolejnego modułu zakładasz nową gałąź z odpowiedniego
`module-XX-start` — dokładny przebieg pracy opisuje plan pracy studenta.

## Gdzie dalej

- **[Plan pracy studenta](docs/roadmap.md)** — dokładna ścieżka od Modułu 0 do obrony projektu
  końcowego.
- **[Podręcznik](docs/handbook.md)** — spójny materiał do każdego modułu.
- **[Materiał modułów](course/README.md)** — zanim klikniesz link do kodu w treści misji, przeczytaj
  krótką uwagę o tym, kiedy te linki działają poprawnie.
- **[Projekt końcowy](https://github.com/michalmaj/mechatronic-systems-programming/blob/final-project-start/final_project/README.md)** *(dostępny od tagu `final-project-start` — ten link nie zadziała na `main`)* — bufor
  wejściowy dla linii sortującej.
- **[Zgłoszenia](../../issues)** — błędy w kodzie i testach, problemy w materiale, propozycje.

## Status

Kurs jest w fazie pilotażu. Rdzeń (moduły 0–9) jest zamknięty i stabilny; jeśli coś w kodzie, testach
lub materiale wygląda na niespójne, zgłoś to przez Issues.

## Licencja

Projekt jest udostępniany na licencji MIT. Szczegóły znajdują się w pliku [LICENSE](LICENSE).
