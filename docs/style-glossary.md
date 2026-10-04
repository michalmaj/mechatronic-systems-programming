# Słownik stylu dokumentacji polskiej

Wewnętrzny dokument dla osób redagujących `README.md`, `docs/roadmap.md`, `docs/handbook.md` i
podobne materiały. Nie jest to dokument dla studentów, dlatego nie odsyłamy do niego z README ani z
planu pracy.

Zasada ogólna: tekst ma brzmieć jak dobry prowadzący tłumaczący problem studentowi przy stanowisku,
nie jak dosłowne tłumaczenie z angielskiego. Nazwy z kodu, takie jak typy, funkcje, wartości typów
wyliczeniowych i polecenia, zawsze zostają po angielsku.

## Preferowane polskie terminy zamiast kalek

| Zamiast (kalka) | Preferuj | Uwagi |
|---|---|---|
| open-book | opisz wprost, np. „możesz z niego korzystać swobodnie” | nie zostawiaj etykiety „open-book” jako takiej |
| recovery | wznowienie pracy / powrót do pracy po awarii | zależnie od kontekstu |
| ownership | własność, kto jest właścicielem | |
| observation | to, co zgłasza/zaobserwował czujnik | opisz wprost zamiast rzeczownika „obserwacja” w oderwaniu |
| ground truth | stan rzeczywisty / to, co naprawdę jest | |
| runtime | w trakcie działania programu / w czasie wykonania | |
| no-op | nie robi nic / operacja bez efektu | |
| per-paczka | dla każdej paczki z osobna / na paczkę | |
| actuator / aktuator | element wykonawczy / napęd / nazwa konkretnego mechanizmu | identyfikatory `Diverter` i `BeltMotor` pozostają bez zmian |
| routing / routing deadline | skierowanie paczki / wybór trasy / limit czasu na ustawienie dywertera | dobierz określenie do opisywanej czynności |
| per-parcel correlation / korelacja dla paczki | powiązanie odczytu z paczką / przypisanie danych do właściwego `ItemId` | „korelacja” może zostać tylko w ściśle technicznym kontekście, gdy rzeczywiście opisuje korelację danych |
| scripted scenario | zaplanowany scenariusz / scenariusz zapisany w danych | nazwy typów `Scripted*` pozostają bez zmian |
| scenario replayer / odtwarzacz scenariusza | uruchamianie scenariusza / funkcja wykonująca scenariusz | przy odwołaniu do kodu użyj `runScenario()` |
| workflow | sposób pracy / przebieg pracy | |
| gating / bramkowanie | warunek działania / uzależnienie działania od... | opisz konkretnie, co dany warunek dopuszcza |
| slot | pole odpowiadające strefie / strefa | `slot` tylko wtedy, gdy jest świadomie wprowadzonym terminem technicznym |
| admission / admitować | przejście z bufora na Infeed / wprowadzić na linię | zależnie od zdania |
| drain the backlog | opróżnić bufor / obsłużyć oczekujące paczki | nie „drenować zaległość” |
| source of truth | punkt odniesienia / miejsce określające format | dobierz do kontekstu |
| defense (projekt zaliczeniowy) | rozmowa podsumowująca / rozmowa o projekcie | „obrona” tylko w znaczeniu formalnej obrony pracy dyplomowej lub rozprawy |
| checkpoint | rozmowa sprawdzająca / sprawdzenie postępów | dobierz nazwę do faktycznej formy zajęć |
| orchestration | koordynacja / ustalanie kolejności | „orkiestracja” tylko w utrwalonym znaczeniu technicznym |
| getter | metoda do odczytu / metoda zwracająca... | użyj angielskiego terminu tylko wtedy, gdy jest przedmiotem nauki |
| framework testowy | biblioteka testowa / narzędzie do testów | zależnie od opisywanego rozwiązania |
| public API | publiczny interfejs | `API` może zostać w nazwie sekcji technicznej lub bezpośrednim cytacie |
| safety-rated / functional safety | spełniający normy bezpieczeństwa / bezpieczeństwo funkcjonalne | nie zostawiaj angielskiego określenia bez wyjaśnienia |
| checked out (git) | opisz efekt wprost, np. „na właściwym tagu” | unikaj czasownikowej kalki „wyewidencjonować” |
| Course Core (po pierwszym wystąpieniu w dokumencie) | rdzeń kursu | pierwsze wystąpienie w dokumencie może zostać jako nazwa własna |
| Final Project (w zwykłej prozie) | projekt końcowy | nazwy plików/tagów (`final_project/`, `final-project-start`) zostają bez zmian |
| Project Kickoff (w zwykłej prozie) | pierwszy własny test / ćwiczenie przed projektem końcowym | nazwa własna tylko przy bezpośrednim odwołaniu do pliku `00_project_kickoff.md` |
| Roadmap (jako tytuł dokumentu) | plan pracy studenta | nazwa pliku (`roadmap.md`) zostaje bez zmian |

## Terminy pozostawione po angielsku

- **Wszystkie nazwy z kodu**: typy, klasy, funkcje, typy wyliczeniowe i ich wartości, np. `Plant`,
  `Item`, `Engine`, `TickResult`, `Scenario`, `Diverter`, `BeltMotor`, `Mode`, `EStopLatchState`,
  `ReadingStatus`, `SystemEventKind` i `ItemId`. Nigdy nie tłumaczymy identyfikatorów.
- **Nazwy poleceń i narzędzi**: `cmake`, `ctest`, `git`, `git switch`, `CLI`. Słowo `build` zostaje
  tylko w nazwach poleceń, presetów i elementów interfejsu. W zwykłym tekście piszemy „budowanie”.
- **`tick`** zostaje jako termin używany w kursie. Nie tłumaczymy go na „takt”, ponieważ słowo
  jest już częścią nazw z kodu (`Tick`, `TickResult`).
- **`commit`, `branch`, `merge`** w kontekście Git to standardowy żargon polskich programistów,
  brzmi naturalnie, zostaje.
- **Nazwy tagów i plików**: `module-XX-start`, `module-XX-solution`, `final-project-start`,
  `final_project/`, `course/`. Są identyfikatorami, więc ich nie tłumaczymy.
- **`E-Stop` jako nazwa z kodu** (`Mode::EStopped`) zostaje bez zmian. W zwykłej prozie preferujemy
  „awaryjny stop”, ale nazwa enumeratora się nie zmienia.

## Inne zasady stylu stosowane w tej redakcji

- Bez seryjnego znakowania obu rodzajów („zrobiłeś/aś”, „gotowy/a”). Zdania zapisujemy neutralnie,
  bez zwracania się po rodzaju w ogóle, tam gdzie to możliwe („masz gotowe”, „piszesz”, zamiast
  „zrobiłeś/aś”).
- Pogrubienie tylko tam, gdzie ma funkcję strukturalną, np. w etykietach i metadanych modułu. Nie
  służy jako rytmiczne podkreślenie w prozie.
- Unikaj powtarzalnych znaczników w rodzaju „kluczowe”, „dokładnie”, „to celowo”, „to ważne”. Mogą
  zostać tylko tam, gdzie faktycznie coś wnoszą, np. w zwrocie „dokładnie to znaczy X”
  jako idiom, nie jako tik.
- Unikaj konstrukcji „nie X, tylko Y” jako domyślnego sposobu kontrastowania. Zdania kontrastowe
  formułuj różnymi sposobami zamiast jednego powtarzalnego szablonu.
- Nie opowiadaj o intencji autora, jeśli można podać wymaganie albo skutek. Zamiast „test celowo
  sprawdza X” napisz „test sprawdza X”; zamiast „to nie przypadek” wyjaśnij zależność przyczynową.
- Preferuj krótsze zdania z jednym głównym wątkiem.
- Znaku „—” używaj wyjątkowo. W zwykłej prozie wybierz kropkę, przecinek albo dwukropek. Nie zastępuj
  nim automatycznie angielskiego myślnika ani nie buduj za jego pomocą wielopiętrowych dopowiedzeń.
  Znak „–” pozostaje poprawny w zakresach liczbowych, np. „moduły 0–9”.
