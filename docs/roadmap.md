🇵🇱 Polski | [🇬🇧 English](roadmap.en.md)

# Plan pracy studenta

← [README](../README.md) · [Podręcznik](handbook.md)

Ten dokument to mapa operacyjna: co robisz, w jakiej kolejności i po czym poznasz, że możesz przejść
dalej. Pełne wyjaśnienia pojęć są w [Podręczniku](handbook.md) — tu tylko trasa.

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
Pierwszy własny test
      ↓
Projekt końcowy
      ↓
Obrona
```

Orientacyjny budżet czasu całej obowiązkowej ścieżki, od Modułu 0 do obrony projektu, to około 30
godzin pracy z repozytorium. To punkt odniesienia do planowania, nie sztywna granica — realny czas
różni się między studentami i będzie doprecyzowany na podstawie pilotażu.

## Skąd czytać, skąd brać kod

Ten dokument i cały pozostały materiał kursu (`course/`, `docs/`) czytasz na gałęzi **`main`** — to
jest kanoniczne, aktualne źródło instrukcji. Kod piszesz gdzie indziej: na własnej gałęzi, założonej
z punktu startowego danego modułu.

- **`module-XX-start`** — punkt startowy modułu. Załóż z niego własną gałąź
  (`git switch -c my-work module-XX-start`) i tam pisz swój kod.
- **`module-XX-solution`** — checkpoint referencyjny: kompletne, poprawne rozwiązanie danego modułu.
  Korzystaj z niego swobodnie, kiedy utkniesz, albo porównaj po fakcie swoje podejście.
- Oba typy tagów są publiczne i **niezmienne** — nie przesuwamy ich ani nie nadpisujemy, nawet gdy
  poprawiamy materiał na `main`. Kod w tych tagach pozostaje właściwym, aktualnym punktem odniesienia
  dla danego etapu kursu.
- Pliki `.md` wewnątrz tych tagów to **zrzut historyczny** z chwili, gdy dany tag powstał — mogą nie
  zawierać późniejszych poprawek redakcyjnych, które trafiły na `main` już po jego publikacji. Jeśli
  treść na `main` różni się od tego, co widzisz w danym tagu, traktuj wersję na `main` jako aktualną.

Krótko: **dokumentację czytaj na `main`, kod bierz z tagów.** Linki do kodu źródłowego wewnątrz
materiałów misji prowadzą wprost do właściwego tagu na GitHubie — nie muszą działać jako linki
lokalne w Twoim checkoucie, żeby były poprawne.

## Jak pracować w każdym module

Ten sam schemat powtarza się w Modułach 1–9:

1. `git fetch --tags`, żeby mieć najnowsze tagi.
2. Załóż własną gałąź z punktu startowego modułu: `git switch -c my-work module-XX-start`.
3. Przeczytaj materiał modułu w [Podręczniku](handbook.md) (odpowiedni rozdział) oraz wprowadzenie w
   `course/module_XX_.../00_wprowadzenie.md` — czytaj je na `main`, to kanoniczne, aktualne źródło
   (zobacz [wyżej](#skąd-czytać-skąd-brać-kod)). Linki do kodu wewnątrz tych plików prowadzą wprost
   do właściwego tagu, więc działają poprawnie niezależnie od tego, skąd je klikasz.
4. Rozwiązuj misje modułu po kolei — każda to osobny plik `course/module_XX_.../NN_*.md` z
   konkretnym zadaniem.
5. Po każdej misji uruchom jej test: `ctest --preset test -L misja-N`.
6. Commituj często, małymi krokami — jeden sensowny krok na commit, a nie jeden duży commit na
   koniec modułu.
7. Na koniec modułu uruchom pełny `ctest --preset test` i sprawdź, że wszystko jest zielone.

`module-XX-solution` to gotowe rozwiązanie danego modułu. Możesz z niego korzystać swobodnie —
sprawdzić, kiedy utkniesz, albo porównać po fakcie swoje podejście. Najwięcej daje jednak samodzielne
dojście do rozwiązania, nie przepisanie gotowego kodu.

## Moduły

| Moduł | Problem | Czego się uczysz (C++) | Co powstaje | Start | Misje | Kiedy jest gotowe |
|---|---|---|---|---|---|---|
| **0 — Orientacja i narzędzia** | Środowisko gotowe do pracy | CMake i CTest jako narzędzia — korzystasz z nich, nie budujesz ich | działający build i CLI | *(brak tagu — `main`)* | — | `cmake --build` i `ctest` działają lokalnie |
| **1 — Podstawy sterowania** | Jedna paczka, jedna strefa na raz | `enum class`, `struct`, `std::optional`, funkcje, pierwsza pętla sterowania | `Plant`, `Item`, klasyfikacja, pierwsza pętla sterowania | `module-01-start` | 1–6 | testy `misja-1`…`misja-6` zielone |
| **2 — Klasa Diverter** | Polecenie to nie to samo, co stan fizyczny | pierwsza własna klasa, enkapsulacja, polecenie kontra stan rzeczywisty | `Diverter` | `module-02-start` | 7–9 | testy `misja-7`…`misja-9` zielone |
| **3 — Engine** | Kto orkiestruje cały krok symulacji | kompozycja, własność obiektów, wolne funkcje jako logika (`Controller`), `Tick`/`TickResult` | `Engine::step()` | `module-03-start` | 10–12 | testy `misja-10`…`misja-12` zielone |
| **4 — Pas i tryb pracy** | Linia potrzebuje trybu pracy i aktuatora, który się rozpędza | kolejna klasa, kilka współpracujących maszyn stanów, bramkowanie | `BeltMotor`, `Mode` | `module-04-start` | 13–15 | testy `misja-13`…`misja-15` zielone |
| **5 — Awaryjny stop** | Bezpieczeństwo jako niezależna ścieżka decyzji, nie kolejny `if` | osobny stan (zatrzask), priorytety, dwie niezależne ścieżki | `EStopLatchState`, `Mode::EStopped` | `module-05-start` | 16–19 | testy `misja-16`…`misja-19` zielone |
| **6 — Czujniki** | Dane z czujników nie zawsze są wiarygodne | statusy odczytu, ostatnia zaufana wartość, stan rzeczywisty a to, co zgłasza czujnik | `PresenceSensor`, `WeightSensor`, klasyfikacja odporna na awarie | `module-06-start` | 20–24 | testy `misja-20`…`misja-24` zielone |
| **7 — Tryb awarii** | Coś idzie nie tak mechanicznie — jak system to zauważa | zewnętrzne wejście a zdarzenie, które system sam wywnioskował; `Mode::Fault`; wznowienie pracy | `SystemEventKind`, wykrywanie i obsługa usterek | `module-07-start` | 25–28 | testy `misja-25`…`misja-28` zielone |
| **8 — Wiele paczek** | Więcej niż jedna paczka na linii naraz | nazwane sloty zamiast jednego obiektu, przetwarzanie od wyjścia do wejścia, `ItemId` jako klucz korelacji | wieloobiektowy `Plant`, `Engine` | `module-08-start` | 29–32 | testy `misja-29`…`misja-32` zielone |
| **9 — Scenariusze skryptowane** | Powtarzalne eksperymenty bez ręcznego sterowania krok po kroku | typ deklaratywny, walidacja, `runScenario` jako odtwarzacz | `Scenario`, `isValidScenario`, `runScenario` | `module-09-start` | 33–35 | testy `misja-33`…`misja-35` zielone |

Moduł 8: wiele paczek naraz nie znaczy wielowątkowości. Rdzeń symulatora zostaje w pełni
sekwencyjny i deterministyczny — po prostu w danej chwili może być w locie więcej niż jedna paczka.

Moduł 5: to uproszczony, dydaktyczny model awaryjnego stopu, pokazujący wzorzec niezależnej ścieżki
bezpieczeństwa w kodzie. Nie jest to projekt rzeczywistego systemu safety-rated ani pełny kurs
functional safety.

## Checkpointy

Po Modułach 3, 7 i 9 jest krótki, indywidualny checkpoint z prowadzącym. Zielone testy są warunkiem
koniecznym, ale nie wystarczającym — checkpoint sprawdza też, czy potrafisz wyjaśnić własne decyzje i
zastosować to, czego się nauczyłeś, w drobnej, wcześniej niewidzianej sytuacji. Dokładny format
ustala prowadzący.

## Pierwszy własny test

Krótkie ćwiczenie przed projektem końcowym (kod źródłowy nazywa je Project Kickoff) — pierwszy raz
piszesz własny test od zera, dla znanej Ci już funkcji z kursu. To nie jest kolejny moduł. Materiał:
[`final_project/00_project_kickoff.md`](../final_project/00_project_kickoff.md).

## Projekt końcowy

Materiał (ten sam czytany model, co reszta kursu) jest tutaj, na `main`:
[`final_project/README.md`](../final_project/README.md). Kod startowy — jak zawsze — bierzesz z
osobnego tagu:

```bash
git fetch --tags
git switch -c my-final-project final-project-start-v2
```

Projekt rozbudowuje dokładnie ten sam symulator, który budowałeś przez cały kurs — nie zaczynasz od
zera. Gotowe rozwiązanie projektu nie jest publikowane. To zadanie do samodzielnego rozwiązania, z
przestrzenią na własne decyzje projektowe tam, gdzie opis wprost na to pozwala.

## Obrona

Po złożeniu projektu jest krótka, indywidualna obrona: wyjaśniasz własne decyzje i rozwiązujesz małą,
wcześniej niewidzianą zmianę lub analizę na bazie własnego kodu. Szczegóły w sekcji „Obrona” w
[`final_project/01_final_project_brief.md`](../final_project/01_final_project_brief.md).
