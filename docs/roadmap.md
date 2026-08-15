🇵🇱 Polski | [🇬🇧 English](roadmap.en.md)

# Roadmap — plan pracy studenta

← [README](../README.md) · [Handbook](handbook.md)

Ten dokument to operacyjna mapa: co robisz, w jakiej kolejności, i skąd wiesz, że jesteś gotowy/a na
kolejny krok. Pełne wyjaśnienia pojęć są w [Handbooku](handbook.md) — tu tylko trasa.

## Ścieżka

```
Setup / Moduł 0
      ↓
Moduł 1
      ↓
Moduł 2
      ↓
Moduł 3
      ↓
Checkpoint 1
      ↓
Moduł 4
      ↓
Moduł 5
      ↓
Moduł 6
      ↓
Moduł 7
      ↓
Checkpoint 2
      ↓
Moduł 8
      ↓
Moduł 9
      ↓
Checkpoint 3
      ↓
Project Kickoff
      ↓
Final Project
      ↓
Obrona
```

**Orientacyjny budżet czasu całej obowiązkowej ścieżki (Setup → obrona projektu końcowego): około
30 godzin pracy z repozytorium.** To cel planistyczny, nie twarda granica — realny czas różni się
między studentami i będzie kalibrowany na podstawie pilotażu.

## Workflow każdego modułu

Ten sam schemat powtarza się dla Modułów 1–9:

1. `git fetch --tags`, żeby mieć najnowsze tagi.
2. Utwórz własną gałąź z punktu startowego modułu: `git switch -c my-work module-XX-start`.
3. Przeczytaj materiał modułu w [Handbooku](handbook.md) (odpowiedni rozdział) oraz wprowadzenie w
   `course/module_XX_.../00_wprowadzenie.md`.
4. Wykonuj misje modułu po kolei — każda misja to plik `course/module_XX_.../NN_*.md` z konkretnym
   zadaniem.
5. Po każdej misji uruchom jej test: `ctest --preset test -L misja-N`.
6. Rób małe, częste commity — jeden sensowny krok na commit, nie jeden gigantyczny commit na koniec
   modułu.
7. Na koniec modułu uruchom pełny `ctest --preset test` i upewnij się, że wszystko jest zielone.

`module-XX-solution` to referencyjne rozwiązanie modułu — materiał **open-book**: dostępny, gdy
utkniesz, albo żeby porównać swoje podejście po fakcie. Największą wartość edukacyjną ma jednak
samodzielne dojście do rozwiązania, nie kopiowanie go od razu.

## Moduły

| Moduł | Problem | Uczysz się (C++) | Co powstaje | Start | Misje | „Done” |
|---|---|---|---|---|---|---|
| **0 — Orientacja i narzędzia** | Środowisko gotowe do pracy | CMake/CTest jako narzędzia (nie budujesz ich) | działający build + CLI | *(brak tagu — `main`)* | — | `cmake --build` i `ctest` działają lokalnie |
| **1 — Podstawy sterowania** | Jedna paczka, jedna strefa na raz | `enum class`, `struct`, `std::optional`, funkcje, pierwsza pętla sterowania | `Plant`, `Item`, klasyfikacja, pierwszy control loop | `module-01-start` | 1–6 | testy `misja-1`…`misja-6` zielone |
| **2 — Klasa Diverter** | Polecenie to nie to samo, co stan fizyczny | pierwsza własna klasa, enkapsulacja, command vs. actual | `Diverter` | `module-02-start` | 7–9 | testy `misja-7`…`misja-9` zielone |
| **3 — Engine** | Kto orkiestruje cały krok symulacji? | kompozycja/ownership, wolne funkcje jako logika (`Controller`), `Tick`/`TickResult` | `Engine::step()` | `module-03-start` | 10–12 | testy `misja-10`…`misja-12` zielone |
| **4 — Pas i tryb pracy** | Linia potrzebuje trybu pracy i aktuatora, który się rozpędza | kolejna klasa, kilka współpracujących maszyn stanów, bramkowanie | `BeltMotor`, `Mode` | `module-04-start` | 13–15 | testy `misja-13`…`misja-15` zielone |
| **5 — E-Stop** | Bezpieczeństwo jako niezależna ścieżka decyzji, nie kolejny `if` | osobny stan (latch), priorytety, dwie niezależne ścieżki | `EStopLatchState`, `Mode::EStopped` | `module-05-start` | 16–19 | testy `misja-16`…`misja-19` zielone |
| **6 — Czujniki** | Dane z czujników nie zawsze są wiarygodne | statusy odczytu, last-known-good, ground truth vs. observation | `PresenceSensor`, `WeightSensor`, klasyfikacja odporna na awarie | `module-06-start` | 20–24 | testy `misja-20`…`misja-24` zielone |
| **7 — Tryb awarii** | Coś idzie nie tak mechanicznie — jak system to zauważa | zewnętrzne wejście vs. zdarzenie emergentne, `Mode::Fault`, recovery | `SystemEventKind`, wykrywanie i obsługa usterek | `module-07-start` | 25–28 | testy `misja-25`…`misja-28` zielone |
| **8 — Wiele paczek** | Więcej niż jedna paczka na linii jednocześnie | nazwane sloty zamiast jednego obiektu, przetwarzanie downstream→upstream, `ItemId` jako klucz korelacji | wieloobiektowy `Plant`, `Engine` | `module-08-start` | 29–32 | testy `misja-29`…`misja-32` zielone |
| **9 — Scenariusze skryptowane** | Powtarzalne eksperymenty bez ręcznego sterowania krok po kroku | typ deklaratywny, walidacja, `runScenario` jako odtwarzacz | `Scenario`, `isValidScenario`, `runScenario` | `module-09-start` | 33–35 | testy `misja-33`…`misja-35` zielone |

**Uwaga o Module 8:** wiele paczek naraz nie oznacza wielowątkowości — cały rdzeń symulatora pozostaje
sekwencyjny i deterministyczny, tylko na raz „w locie” może być więcej niż jedna paczka.

**Uwaga o Module 5:** to uproszczony, dydaktyczny model E-Stop, zbudowany po to, by pokazać wzorzec
niezależnej ścieżki bezpieczeństwa w kodzie — **nie jest to projekt rzeczywistego systemu
safety-rated** ani pełny kurs functional safety.

## Checkpointy

Po Modułach 3, 7 i 9 następuje krótki, indywidualny checkpoint z prowadzącym. Zielone testy są
warunkiem koniecznym, ale nie wystarczającym — checkpoint sprawdza też, czy potrafisz wyjaśnić własne
decyzje i zastosować to, czego się nauczyłeś/aś, w drobnej, wcześniej niewidzianej sytuacji. Dokładny
format ustala prowadzący/a.

## Project Kickoff

Krótkie ćwiczenie pomostowe przed projektem końcowym — pierwszy raz piszesz własny test od zera dla
znanej funkcji kursu. Nie jest to kolejny moduł kursu. Materiał: `final_project/00_project_kickoff.md`
(dostępny od tagu `final-project-start`).

## Final Project

```bash
git fetch --tags
git switch -c my-final-project final-project-start
```

Pełny opis: [`final_project/README.md`](https://github.com/michalmaj/mechatronic-systems-programming/blob/final-project-start/final_project/README.md)
(dostępne dopiero po `git switch` na `final-project-start` — na `main` ten katalog jeszcze nie istnieje).
Projekt rozszerza dokładnie ten
sam symulator, który budowałeś/aś przez Course Core — nie zaczynasz od zera. Referencyjne rozwiązanie
projektu **nie jest publikowane** — to zadanie do samodzielnego rozwiązania, z przestrzenią na własne
decyzje projektowe tam, gdzie brief to wprost dopuszcza.

## Obrona

Po złożeniu projektu odbywa się krótka, indywidualna obrona — wyjaśnienie własnych decyzji i mała,
wcześniej niewidziana zmiana lub analiza na podstawie własnego kodu. Szczegóły: sekcja „Obrona” w
`final_project/01_final_project_brief.md`.
