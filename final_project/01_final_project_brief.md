# Final Project Brief: bufor wejściowy

## Kontekst

Paczki mogą pojawiać się szybciej, niż Infeed może je natychmiast przyjąć. System potrzebuje bufora
wejściowego przed linią sortującą.

Dziś (`module-09-solution`) `Scenario::arrivals` próbuje bezpośrednio wykonać `Engine::spawnItem()`.
Jeśli Infeed jest zajęty, `runScenario()` kończy się `std::nullopt` — cały scenariusz się nie liczy.
Twoim zadaniem jest to zmienić: samo chwilowe zajęcie Infeed nie powinno automatycznie oznaczać błędu.
Przybywająca paczka powinna móc poczekać przed linią.

To jest twój capstone po Course Core 0–9. Startujesz z `final-project-start` — funkcjonalnie tym samym
kodem symulatora co `module-09-solution`. Rozszerzasz go tam, gdzie to zadanie tego wymaga. Nie
przepisujesz systemu od zera i nie obchodzisz istniejących abstrakcji równoległą implementacją.

Ten brief opisuje **co** ma działać i **jakie ma być tego dowodzone**. Nie mówi **jak dokładnie to
zbudować** — architektura wewnątrz MUST jest w części twoją decyzją projektową (patrz sekcja DESIGN
CHOICE). Wiesz, że potrzebujesz struktury FIFO i że `std::deque` jest naturalnym kandydatem na
kontener — to, gdzie ta struktura mieszka i jak jest opakowana, decydujesz sam/a i uzasadniasz.

## Wspólny MUST

Niezależnie od twoich decyzji projektowych, poniższe musi być prawdziwe w twoim rozwiązaniu:

- **Bufor jest trwałym stanem domenowym**, niezależnym od `Scenario` — istnieje i działa tak samo,
  gdy sterujesz systemem wprost (imperatywnie), jak i przez `Scenario`. Nie jest prywatnym stanem
  wewnątrz `runScenario`.
- **Enkapsulacja.** FIFO i pojemność muszą być chronione przez typ z prywatnym kontenerem — nie przez
  goły, publiczny `std::deque<Item>`, który dowolny kod mógłby zmodyfikować w środku, łamiąc kolejność
  albo pojemność bez wiedzy reszty systemu.
- **Dwa oddzielne API.** `Engine::spawnItem(id, mass)` zostaje **dokładnie takie, jak jest dziś** —
  niskopoziomowe, wchodzi wprost na Infeed, odrzuca przy zajętości lub kolizji id. Dodajesz nowe,
  osobne API domenowe przyjmowania przybyć (nazwa do ciebie — `acceptArrival` jest sugerowana), które
  jako jedyne korzysta z bufora. To nowe API zastępuje `spawnItem` wewnątrz fazy „arrivals”
  `runScenario`.
- **FIFO.** Żadna później przyjęta paczka nie może wejść na Infeed przed wcześniejszą paczką wciąż
  oczekującą w buforze.
- **Ograniczona pojemność, minimum 2.** Bez górnej granicy bufor cicho pochłania to, co mogłoby być
  błędem — a to zakazane: ostatnia paczka nigdy „po prostu nie znika”. Przepełnienie musi być jawnym,
  obserwowalnym błędem.
- **Moment oceny przepełnienia.** Ocena „czy jest miejsce w buforze” odbywa się w fazie *arrival*,
  względem stanu bufora pozostawionego na koniec poprzedniego ticku — **przed** jakimkolwiek
  działaniem tego ticku. Miejsce, które zwolniłoby się dopiero w tym samym ticku, nie ratuje przybycia,
  które już zastało bufor pełny.
- **Kontrakt kolejności w ticku.** Admisja bufor→Infeed rozstrzyga się **po** rozstrzygnięciu
  przejścia Infeed→PresenceCheck tego samego ticku (może więc skorzystać z Infeed właśnie zwolnionego
  tym samym ticku). Paczka właśnie admitowana na Infeed **nie porusza się dalej w tym samym ticku**.
- **Bramkowanie gotowością pasa.** Przyjęcie paczki do bufora (enqueue) nie jest bramkowane trybem
  pracy — paczki mogą fizycznie napłynąć nawet gdy linia stoi. Admisja z bufora na Infeed **jest**
  bramkowana tą samą bramką gotowości pasa, co reszta linii (`beltMotor_.actualState() == Running`) —
  łącznie z opóźnieniem `RampingUp` (pas potrzebuje jednego ticku, zanim faktycznie zacznie się
  poruszać po starcie od `Stopped`). To oznacza: przybycie zbiegające się ze świeżym startem linii
  może poczekać w buforze o jeden tick dłużej, zanim wjedzie na Infeed — sprawdź to empirycznie w
  swoich testach, nie zakładaj.
- **Reguła „≤1 przybycie na tick” w `isValidScenario` zostaje bez zmian.** Nie dodawaj obsługi wielu
  przybyć na tym samym ticku do wspólnego rdzenia — to opcjonalne rozszerzenie (patrz niżej).
- **Active-ItemId uniqueness.** Publiczne API `Engine` (`spawnItem` i twoje nowe API arrival)
  gwarantuje, że dwie aktywne paczki o tym samym `ItemId` nie mogą jednocześnie istnieć w buforze ani w
  żadnej strefie — niezależnie od tego, czy wywołanie pochodzi ze `Scenario`, czy z kodu
  imperatywnego.
- **Obserwowalność.** Ślad (`TickResult`) musi udostępniać co najmniej: liczbę paczek aktualnie
  czekających w buforze oraz `ItemId` paczki na czele kolejki. To minimum wystarczające do
  jednoznacznego zweryfikowania FIFO na przestrzeni śladu — bez rozdymania `TickResult` ponad to.
- **Determinizm.** Ten sam, poprawny `Scenario` uruchomiony dwa razy musi dać semantycznie identyczny
  ślad.
- **Brak nowej semantyki bezpieczeństwa.** Przyjmowanie przybyć do bufora podczas E-Stop/Fault to
  świadome, dydaktyczne założenie o zewnętrznym strumieniu wejściowym — nie jest to model
  rzeczywistego, safety-rated zachowania instalacji przemysłowej. Poza tym, co jawnie opisane wyżej,
  semantyka E-Stop/Fault nie zmienia się.

## Niezmienniki (sprawdzane, nie tylko deklarowane)

1. Każdy `ItemId` oznacza dokładnie jedną paczkę — teraz licząc bufor i cztery strefy razem.
2. FIFO bufora, dokładnie jak wyżej.
3. Żadna utrata paczki bez jawnie obserwowalnego wyniku (admisja, wyjazd, albo `nullopt` ze
   `runScenario` przy przepełnieniu).
4. Co najwyżej jeden ruch danej paczki w fizycznej linii na tick — teraz pięć punktów zamiast czterech
   (Diverting, Weighing→Diverting, PresenceCheck→Weighing, Infeed→PresenceCheck, Bufor→Infeed). Samo
   przyjęcie zewnętrzne do bufora **nie jest** „ruchem w linii” i nie liczy się do tego niezmiennika —
   liczone punkty zaczynają się dopiero od Bufor→Infeed.
5. Istniejące bezpieczeństwo korelacji ItemId w czujnikach (`presenceObservedItemId` /
   `weightObservedItemId`) — bez zmian, bufor leży w całości przed PresenceCheck.
6. Brak obejścia semantyki E-Stop/Fault/RampingUp.
7. Determinizm zachowany.
8. Kolejność elementów w `Scenario::arrivals` nadal nie niesie znaczenia.
9. Active-ItemId uniqueness na publicznym API `Engine`, jak wyżej.

## Kryteria akceptacji

Twoje rozwiązanie musi przejść wspólny scenariusz akceptacyjny (opis zachowania, nie gotowy kod — sam
go zbudujesz jako `Scenario`):

1. Linia startuje, po niej niemal natychmiast E-Stop.
2. Dokładnie dwa przybycia w trakcie zatrzymania — obie paczki muszą trafić do bufora, bo linia stoi.
3. Zwolnienie E-Stop, reset, ponowny start — system zaczyna drenować backlog.
4. Kolejna, trzecia paczka przybywa już po wznowieniu.
5. Wszystkie trzy paczki docierają ostatecznie do poprawnych wyjść (zgodnie z ich masą/klasyfikacją).
6. Ślad jawnie pokazuje: paczki admitowane są w kolejności przybycia (FIFO), żadna nie ginie, żadna nie
   zostaje pominięta podczas zatrzymania.

**To musi działać już przy minimalnej dozwolonej pojemności bufora (2)** — nie projektuj scenariusza
akceptacyjnego (ani jego przejścia) w sposób zależny od większej, wybranej przez ciebie pojemności.

Uruchom ten sam scenariusz dwa razy — ślady muszą być semantycznie identyczne (ten sam determinizm, co
wymagany od Modułu 9).

## Własne testy

Minimalne wymaganie: **co najmniej 3 małe testy, pokrywające co najmniej 3 różne kategorie** z listy
poniżej (nie 3 warianty tego samego przypadku):

- FIFO pod obciążeniem buforowym.
- Przybycie przy zajętym-ale-niepełnym buforze (sukces, tam gdzie dziś byłaby porażka).
- Przepełnienie bufora — łącznie z momentem oceny (pre-step): miejsce zwolnione dopiero w tym samym
  ticku nie ratuje odrzuconego przybycia.
- Active-ItemId uniqueness na publicznym API Engine.
- Zachowanie bufora przez stop/E-Stop/Fault i recovery — bez cichej utraty paczek.
- Powtarzalność: ten sam `Scenario` dwa razy → identyczny ślad.

Zastosuj dokładnie tę samą dyscyplinę, którą ćwiczyłeś/aś w Project Kickoff: każdy test musi umieć
faktycznie zaczerwienić się na zepsutym kodzie, nie tylko przejść na poprawnym.

## Demonstracja przez Scenario

Poza testami jednostkowymi, twoje rozwiązanie musi zawierać **demonstrację przez `Scenario`** — pełny,
uruchamialny scenariusz (analogicznie do `recoveryDemoScenario`/`multiParcelDemoScenario` z Modułu 9),
który pokazuje działanie bufora w praktyce. Scenariusz akceptacyjny z sekcji wyżej może pełnić tę
rolę.

## Design rationale (krótkie, pisemne)

Zanim zaczniesz kodować: napisz kilka zdań uzasadnienia dla dwóch decyzji, które podejmiesz (patrz
DESIGN CHOICE niżej):

- **Gdzie mieszka bufor** (jaki byt jest jego właścicielem) — i dlaczego to miejsce, a nie inne.
  Rozważ oba kierunki symetrycznie, nie tylko jeden: czy to stan fizyczny czy stan eksperymentu; czy
  bufor pasuje bardziej do stref, którymi już włada `Plant`, czy do komponentów „urządzeniowych”,
  którymi włada `Engine` (jak `Diverter`/`BeltMotor`); czy bufor istnieje także w trybie imperatywnym
  (bez `Scenario`) w obu wariantach; jak każdy z wariantów wpływa na `Engine::step()` i na
  testowalność — w jednym miejscu ten wpływ jest inny niż w drugim, i to jest właśnie coś, co masz
  sam/a ocenić i uzasadnić, nie coś z góry przesądzonego przez ten brief.
- **Jak dobrałeś/aś pojemność** i sposób jej ustawienia.

To krótki dokument (kilkanaście zdań wystarczy), nie osobny raport — ale będzie punktem wyjścia do
indywidualnej obrony (patrz niżej).

## DESIGN CHOICE — decydujesz i uzasadniasz

- **Miejsce własności bufora.** Bufor jako część `Plant` (obok istniejących stref) — albo jako osobny
  komponent, którym włada `Engine`, analogicznie do `Diverter`/`BeltMotor`, które `Engine` już dziś
  posiada. Obie opcje są tu celowo równorzędne — żadna nie jest sugerowana jako „bardziej poprawna”;
  wybierasz i uzasadniasz sam/a (patrz Design rationale wyżej). Jedyna wykluczona opcja to bufor jako
  prywatny stan wyłącznie wewnątrz `runScenario`/`Scenario`, czyli niewidoczny dla trybu
  imperatywnego.
- **Dokładna wartość pojemności powyżej minimum 2** i sposób jej ustawienia (stała czy pole
  konfigurowalne).
- **Dokładna nazwa nowego API arrival** (`acceptArrival` jest sugerowane, ale to twoja decyzja).
- **Dokładne nazwy i typy nowych pól `TickResult`** — informacja (liczba czekających + id głowy
  kolejki) jest wymagana, nazewnictwo nie.
- **Wewnętrzna reprezentacja bufora** — o ile pozostaje prywatna i chroni FIFO/pojemność.
- **Nazewnictwo i dekompozycja** nowych funkcji pomocniczych.

## OPTIONAL EXTENSIONS — poza wspólnym MUST

Poniższe **nie są wymagane** do pełnej podstawowej oceny i leżą poza budżetem obowiązkowej ścieżki.
Podejmij je tylko, jeśli masz na to czas i ochotę — nierówna trudność między nimi jest zamierzona,
nie próbuj robić wszystkich:

- Wiele przybyć w tym samym ticku, z jawną, deterministyczną regułą rozstrzygania kolejności (nigdy
  opartą o przypadkową pozycję w `std::vector`).
- Bogatsza sygnalizacja lub konfigurowalna strategia przepełnienia.
- Generator `ItemId`.
- Zróżnicowane priorytety przybyć.
- Serializacja `Scenario`.
- Bogatszy ślad/statystyki (np. maksymalna głębokość bufora w przebiegu).

## Zasady submission / Git

- Startujesz z tagu `final-project-start` na osobistej gałęzi.
- Commituj tak, żeby historia pokazywała realny postęp — kilka commitów odzwierciedlających kolejne
  kroki (np. szkic typu bufora, integracja z resztą linii, rozszerzenie śladu, własne testy, scenariusz
  akceptacyjny), nie jeden gigantyczny commit na końcu. Nie jest wymagany żaden zaawansowany workflow
  Git (bez wymuszania rebase/squash) — wystarczy uczciwa, czytelna historia na własnej gałęzi.
  Bezpośrednie commity do gałęzi współdzielonej są niedozwolone.
- W połowie pracy warto zrobić sobie lekki, nieformalny self-check: czy wybrany kierunek architektury
  faktycznie daje się uzasadnić (sekcja Design rationale) — łatwiej skorygować kurs wcześnie niż
  późno.
- Finalne złożenie: czysta gałąź/PR zawierająca implementację, własne testy, scenariusz demonstracyjny
  i krótkie design rationale.

## Obrona

Zielone testy nie są dowodem zrozumienia. Po złożeniu projektu odbędzie się krótka, **indywidualna**
obrona (nawet jeśli projekt powstał w parze) — wyjaśnienie własnych decyzji, przewidywanie zachowania
systemu w nowych sytuacjach, i mały, wcześniej niewidziany problem do rozwiązania na podstawie
własnego kodu. Pisz kod, który potrafisz wytłumaczyć, nie tylko taki, który przechodzi testy.
