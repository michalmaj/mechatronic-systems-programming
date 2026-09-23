# Słownik stylu — dokumentacja PL

Wewnętrzny dokument dla osób redagujących `README.md`, `docs/roadmap.md`, `docs/handbook.md` i
podobny materiał. Nie jest to dokument studencki — nie linkujemy go z README ani z planu pracy.

Zasada ogólna: tekst ma brzmieć jak dobry prowadzący tłumaczący problem studentowi przy stanowisku,
nie jak dosłowne tłumaczenie z angielskiego. Nazwy z kodu (typy, funkcje, enumy, komendy) zawsze
zostają po angielsku — tego się nie tłumaczy.

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
| workflow | sposób pracy / przebieg pracy | |
| checked out (git) | opisz efekt wprost, np. „na właściwym tagu” | unikaj czasownikowej kalki „wyewidencjonować” |
| Course Core (po pierwszym wystąpieniu w dokumencie) | rdzeń kursu | pierwsze wystąpienie w dokumencie może zostać jako nazwa własna |
| Final Project (w zwykłej prozie) | projekt końcowy | nazwy plików/tagów (`final_project/`, `final-project-start`) zostają bez zmian |
| Project Kickoff (w zwykłej prozie) | pierwszy własny test / ćwiczenie przed projektem końcowym | nazwa własna tylko przy bezpośrednim odwołaniu do pliku `00_project_kickoff.md` |
| Roadmap (jako tytuł dokumentu) | plan pracy studenta | nazwa pliku (`roadmap.md`) zostaje bez zmian |

## Terminy pozostawione po angielsku

- **Wszystkie nazwy z kodu**: typy, klasy, funkcje, enumy i ich składowe — `Plant`, `Item`, `Engine`,
  `TickResult`, `Scenario`, `Diverter`, `BeltMotor`, `Mode`, `EStopLatchState`, `ReadingStatus`,
  `SystemEventKind`, `ItemId` i tak dalej. Nigdy nie tłumaczymy identyfikatorów.
- **Nazwy komend i narzędzi**: `cmake`, `ctest`, `git`, `git switch`, `CLI`, `build`.
- **`tick`** — zostaje jako termin domenowy kursu; nie tłumaczymy na „takt” ani podobne, bo słowo
  jest już częścią nazw z kodu (`Tick`, `TickResult`).
- **`commit`, `branch`, `merge`** w kontekście Git — standardowy żargon polskich programistów,
  brzmi naturalnie, zostaje.
- **Nazwy tagów i plików**: `module-XX-start`, `module-XX-solution`, `final-project-start`,
  `final_project/`, `course/` — to identyfikatory, nie tłumaczymy ich.
- **`E-Stop` jako nazwa z kodu** (`Mode::EStopped`) zostaje bez zmian. W zwykłej prozie preferujemy
  „awaryjny stop”, ale nazwa enumeratora się nie zmienia.

## Inne zasady stylu stosowane w tej redakcji

- Bez seryjnego znakowania obu rodzajów („zrobiłeś/aś”, „gotowy/a”) — zdania przepisane neutralnie,
  bez zwracania się po rodzaju w ogóle, tam gdzie to możliwe („masz gotowe”, „piszesz”, zamiast
  „zrobiłeś/aś”).
- Bold tylko tam, gdzie ma funkcję strukturalną (etykiety, metadane modułu) — nie jako rytmiczne
  podkreślenie w prozie.
- Unikaj powtarzalnych znaczników w rodzaju „kluczowe”, „dokładnie”, „to celowo”, „to ważne” jako
  pustych wzmacniaczy — zostają tylko tam, gdzie faktycznie coś wnoszą (np. „dokładnie to znaczy X”
  jako idiom, nie jako tik).
- Unikaj konstrukcji „nie X, tylko Y” jako domyślnego sposobu kontrastowania — zdania kontrastowe
  formułuj różnymi sposobami zamiast jednego powtarzalnego szablonu.
