🇵🇱 Polski | [🇬🇧 English](roadmap.en.md)

# Plan pracy studenta

← [README](../README.md) · [Podręcznik](handbook.md)

Tutaj znajdziesz kolejność pracy: od przygotowania środowiska aż po rozmowę o gotowym projekcie.
Wyjaśnienia pojęć są w [Podręczniku](handbook.md); ten dokument skupia się na organizacji kursu.

## Ścieżka

```
Przygotowanie / Moduł 0
      ↓
Moduł 1
      ↓
Moduł 2
      ↓
Moduł 3
      ↓
Rozmowa sprawdzająca 1
      ↓
Moduł 4
      ↓
Moduł 5
      ↓
Moduł 6
      ↓
Moduł 7
      ↓
Rozmowa sprawdzająca 2
      ↓
Moduł 8
      ↓
Moduł 9
      ↓
Rozmowa sprawdzająca 3
      ↓
Pierwszy własny test
      ↓
Projekt końcowy
      ↓
Rozmowa podsumowująca
```

Na przejście całej obowiązkowej części, od modułu 0 do rozmowy podsumowującej, warto zarezerwować
około 30 godzin. To wartość orientacyjna, która zostanie jeszcze sprawdzona podczas pilotażu.

## Skąd czytać, skąd brać kod

Dokumentację z katalogów `course/` i `docs/` czytaj na gałęzi **`main`**. Tam znajduje się jej
najnowsza wersja. Kod pisz na własnej gałęzi utworzonej z punktu startowego danego modułu.

- **`module-XX-start`:** punkt startowy modułu. Załóż z niego własną gałąź
  (`git switch -c my-work module-XX-start`) i tam pisz swój kod.
- **`module-XX-solution`:** kompletne rozwiązanie danego modułu, z którym możesz porównać swoją
  pracę. Korzystaj z niego swobodnie, kiedy utkniesz, albo zajrzyj do niego po ukończeniu modułu.
- Oba rodzaje tagów są publiczne i **niezmienne**. Nie przesuwamy ich ani nie nadpisujemy po
  opublikowaniu. Dzięki temu kod przypisany do danego etapu kursu zawsze pozostaje taki sam.
- Pliki `.md` wewnątrz tych tagów są kopią z chwili utworzenia tagu. Mogą nie zawierać późniejszych
  poprawek redakcyjnych, które trafiły na `main` już po jego publikacji. Jeśli
  treść na `main` różni się od tego, co widzisz w danym tagu, traktuj wersję na `main` jako aktualną.

W skrócie: **dokumentację czytaj na `main`, kod bierz z tagów.** Odsyłacze do kodu w opisach misji
prowadzą do odpowiedniego tagu na GitHubie. Nie są to odsyłacze do plików w lokalnym katalogu.

## Jak pracować w każdym module

Ten sam schemat powtarza się w modułach 1–9:

1. `git fetch --tags`, żeby mieć najnowsze tagi.
2. Załóż własną gałąź z punktu startowego modułu: `git switch -c my-work module-XX-start`.
3. Na `main` przeczytaj odpowiedni rozdział [Podręcznika](handbook.md) i wprowadzenie
   `course/module_XX_.../00_wprowadzenie.md`. Odsyłacze zawarte w tych plikach otwierają kod z
   właściwego tagu na GitHubie.
4. Rozwiązuj misje modułu po kolei. Każda znajduje się w osobnym pliku
   `course/module_XX_.../NN_*.md` z konkretnym zadaniem.
5. Po każdej misji uruchom jej test: `ctest --preset test -L misja-N`.
6. Commituj po każdym sensownym etapie pracy. Nie odkładaj całego modułu do jednego commita.
7. Na koniec modułu uruchom pełny `ctest --preset test` i sprawdź, czy wszystkie testy przechodzą.

W tagu `module-XX-solution` znajdziesz gotowe rozwiązanie modułu. Sięgnij do niego, jeśli utkniesz,
albo porównaj z nim własny kod po zakończeniu pracy. Samo przepisanie rozwiązania niewiele uczy.

## Moduły

| Moduł | Problem | Czego się uczysz (C++) | Co powstaje | Start | Misje | Kiedy jest gotowe |
|---|---|---|---|---|---|---|
| **0. Orientacja i narzędzia** | Środowisko gotowe do pracy | korzystanie z gotowej konfiguracji CMake i CTest | działające budowanie i CLI | *(brak tagu, praca na `main`)* | brak | `cmake --build` i `ctest` działają lokalnie |
| **1. Podstawy sterowania** | Jedna paczka, jedna strefa na raz | `enum class`, `struct`, `std::optional`, funkcje, pierwsza pętla sterowania | `Plant`, `Item`, klasyfikacja, pierwsza pętla sterowania | `module-01-start` | 1–6 | testy `misja-1`…`misja-6` przechodzą |
| **2. Klasa Diverter** | Polecenie to nie to samo co stan fizyczny | pierwsza własna klasa, enkapsulacja, polecenie i stan rzeczywisty | `Diverter` | `module-02-start` | 7–9 | testy `misja-7`…`misja-9` przechodzą |
| **3. Engine** | Koordynacja całego kroku symulacji | kompozycja, własność obiektów, wolne funkcje jako logika (`Controller`), `Tick` i `TickResult` | `Engine::step()` | `module-03-start` | 10–12 | testy `misja-10`…`misja-12` przechodzą |
| **4. Pas i tryb pracy** | Linia potrzebuje trybu pracy i napędu, który się rozpędza | kolejna klasa, kilka współpracujących maszyn stanów, warunki dopuszczające ruch | `BeltMotor`, `Mode` | `module-04-start` | 13–15 | testy `misja-13`…`misja-15` przechodzą |
| **5. Awaryjny stop** | Niezależna ścieżka decyzji związanych z bezpieczeństwem | osobny stan (zatrzask), priorytety, dwie niezależne ścieżki | `EStopLatchState`, `Mode::EStopped` | `module-05-start` | 16–19 | testy `misja-16`…`misja-19` przechodzą |
| **6. Czujniki** | Dane z czujników nie zawsze są wiarygodne | statusy odczytu, ostatnia zaufana wartość, stan rzeczywisty i wskazanie czujnika | `PresenceSensor`, `WeightSensor`, klasyfikacja odporna na awarie | `module-06-start` | 20–24 | testy `misja-20`…`misja-24` przechodzą |
| **7. Tryb awarii** | Wykrywanie problemu mechanicznego przez system | zewnętrzne wejście, zdarzenie rozpoznane przez system, `Mode::Fault`, wznowienie pracy | `SystemEventKind`, wykrywanie i obsługa usterek | `module-07-start` | 25–28 | testy `misja-25`…`misja-28` przechodzą |
| **8. Wiele paczek** | Więcej niż jedna paczka na linii naraz | osobne pola dla stref, przetwarzanie od wyjścia do wejścia, przypisywanie danych do paczek za pomocą `ItemId` | wieloobiektowy `Plant`, `Engine` | `module-08-start` | 29–32 | testy `misja-29`…`misja-32` przechodzą |
| **9. Scenariusze zapisane w danych** | Powtarzalne eksperymenty bez ręcznego sterowania krok po kroku | typ opisujący scenariusz, sprawdzanie poprawności, wykonywanie opisu przez `runScenario` | `Scenario`, `isValidScenario`, `runScenario` | `module-09-start` | 33–35 | testy `misja-33`…`misja-35` przechodzą |

W module 8 wiele paczek znajduje się na linii jednocześnie, ale program nadal działa w jednym wątku.
Symulator pozostaje sekwencyjny i powtarzalny.

Moduł 5 przedstawia uproszczony model awaryjnego stopu i niezależnej ścieżki bezpieczeństwa w
kodzie. Nie jest projektem rzeczywistego układu spełniającego normy bezpieczeństwa przemysłowego.

## Rozmowy sprawdzające

Po modułach 3, 7 i 9 odbywa się krótka, indywidualna rozmowa z prowadzącym. Przechodzące testy są
warunkiem koniecznym, ale rozmowa sprawdza też, czy potrafisz wyjaśnić własne decyzje i
zastosować to, czego się nauczyłeś, w drobnej, wcześniej niewidzianej sytuacji. Dokładny format
ustala prowadzący.

## Pierwszy własny test

Przed projektem końcowym po raz pierwszy napiszesz od zera test znanej już funkcji. To krótkie
ćwiczenie przygotowujące, a nie osobny moduł. Materiał:
[`final_project/00_project_kickoff.md`](../final_project/00_project_kickoff.md).

## Projekt końcowy

Opis projektu czytaj na `main` w pliku
[`final_project/README.md`](../final_project/README.md). Kod startowy znajduje się w osobnym tagu:

```bash
git fetch --tags
git switch -c my-final-project final-project-start-v3
```

Rozbudujesz ten sam symulator, nad którym pracujesz podczas kursu, więc nie zaczynasz od zera.
Gotowego rozwiązania nie publikujemy. W miejscach wskazanych w opisie samodzielnie podejmiesz decyzje
projektowe.

## Rozmowa podsumowująca

Po oddaniu projektu odbędzie się krótka, indywidualna rozmowa. Wyjaśnisz własne decyzje oraz
rozwiążesz niewielkie, wcześniej nieomawiane zadanie na podstawie swojego kodu. Szczegóły znajdziesz
w sekcji „Rozmowa podsumowująca” w
[`final_project/01_final_project_brief.md`](../final_project/01_final_project_brief.md).
