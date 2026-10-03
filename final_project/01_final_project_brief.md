🇵🇱 Polski | [🇬🇧 English](01_final_project_brief.en.md)

# Projekt końcowy: bufor wejściowy

## Kontekst

Paczki mogą pojawiać się szybciej, niż Infeed jest w stanie je natychmiast przyjąć. System potrzebuje
bufora wejściowego przed linią sortującą.

W `module-09-solution` każde przybycie z `Scenario::arrivals` wywołuje `Engine::spawnItem()`.
Jeżeli Infeed jest zajęty, `runScenario()` zwraca `std::nullopt` i przerywa scenariusz. W projekcie
końcowym dodasz miejsce, w którym przybywająca paczka może poczekać na zwolnienie wejścia.

Projekt wykonujesz po ukończeniu modułów 0–9. Tag `final-project-start-v3` zawiera funkcjonalnie ten
sam symulator co `module-09-solution`. Rozszerz istniejący kod; nie twórz obok niego drugiej,
niezależnej implementacji.

Opis określa wymagane zachowanie i sposób jego sprawdzenia. Część architektury pozostaje do
samodzielnego zaprojektowania. Potrzebujesz struktury FIFO, dla której naturalnym kontenerem jest
`std::deque`; zdecyduj, kto ma być jej właścicielem i jaki interfejs ją osłoni.

## Wymagania obowiązkowe

Niezależnie od podjętych decyzji projektowych, poniższe musi być prawdziwe w rozwiązaniu:

- **Bufor jest trwałym stanem domenowym**, niezależnym od `Scenario` — istnieje i działa tak samo,
  zarówno przy sterowaniu bezpośrednim, jak i przez `Scenario`. Nie jest prywatnym stanem
  wewnątrz `runScenario`.
- **Enkapsulacja.** FIFO i pojemność muszą być chronione przez typ z prywatnym kontenerem — nie przez
  goły, publiczny `std::deque<Item>`, który dowolny kod mógłby zmodyfikować w środku, łamiąc kolejność
  albo pojemność bez wiedzy reszty systemu.
- **Dwa oddzielne API.** `Engine::spawnItem(id, mass)` zachowuje obecne działanie: dodaje paczkę
  bezpośrednio na Infeed i odrzuca ją przy zajętości lub kolizji id. Dochodzi nowe,
  osobne API domenowe do przyjmowania przybyć (nazwa dowolna — sugerowana jest `acceptArrival`), które
  jako jedyne korzysta z bufora. To nowe API zastępuje `spawnItem` wewnątrz fazy przybyć w
  `runScenario`.
- **FIFO.** Żadna później przyjęta paczka nie może wejść na Infeed przed wcześniejszą paczką wciąż
  oczekującą w buforze.
- **Ograniczona pojemność, minimum 2.** Bufor musi mieć górną granicę. Próba jej przekroczenia jest
  obserwowalnym błędem; paczka nie może zniknąć bez informacji o odrzuceniu.
- **Moment oceny przepełnienia.** Ocena „czy jest miejsce w buforze” odbywa się w fazie przybyć,
  względem stanu bufora pozostawionego na koniec poprzedniego ticku — **przed** jakimkolwiek
  działaniem tego ticku. Miejsce, które zwolniłoby się dopiero w tym samym ticku, nie ratuje przybycia,
  które już zastało bufor pełny.
- **Kolejność w ticku.** Przejście bufor→Infeed następuje **po** rozstrzygnięciu
  przejścia Infeed→PresenceCheck tego samego ticku (może więc skorzystać z Infeed właśnie zwolnionego
  tym samym ticku). Paczka właśnie przeniesiona na Infeed **nie porusza się dalej w tym samym ticku**.
- **Zależność od gotowości pasa.** Przyjęcie paczki do bufora (enqueue) nie zależy od trybu pracy:
  paczki mogą napłynąć także wtedy, gdy linia stoi. Przejście z bufora na Infeed wymaga natomiast
  gotowego pasa (`beltMotor_.actualState() == Running`), tak jak pozostały ruch na linii —
  łącznie z opóźnieniem `RampingUp` (pas potrzebuje jednego ticku, zanim faktycznie zacznie się
  poruszać po starcie od `Stopped`). Oznacza to: przybycie zbiegające się ze świeżym startem linii
  może poczekać w buforze o jeden tick dłużej, zanim wjedzie na Infeed — warto sprawdzić to empirycznie
  we własnych testach, nie zakładać.
- **Reguła „co najwyżej jedno przybycie na tick” w `isValidScenario` zostaje bez zmian.** Nie należy
  dodawać obsługi wielu przybyć na tym samym ticku do wspólnego rdzenia — to rozszerzenie opcjonalne
  (patrz niżej).
- **Unikalność aktywnego `ItemId`.** Publiczne API `Engine` (`spawnItem` i nowe API przybyć)
  gwarantuje, że dwie aktywne paczki o tym samym `ItemId` nie mogą jednocześnie istnieć w buforze ani w
  żadnej strefie — niezależnie od tego, czy wywołanie pochodzi ze `Scenario`, czy z kodu
  bezpośredniego.
- **Obserwowalność — wymagany kontrakt nazw.** Ślad (`TickResult`) musi udostępniać dwa nowe pola,
  pod nazwami `bufferedCount` (liczba paczek aktualnie czekających w buforze) oraz
  `bufferHeadItemId` (`std::optional<ItemId>` — id paczki na czele kolejki, `std::nullopt` gdy bufor
  jest pusty). To minimum wystarczające do jednoznacznego zweryfikowania FIFO na przestrzeni śladu —
  bez rozdymania `TickResult` ponad to.
- **Determinizm.** Ten sam, poprawny `Scenario` uruchomiony dwa razy musi dać semantycznie identyczny
  ślad.
- **Brak nowej semantyki bezpieczeństwa.** Przyjmowanie przybyć do bufora podczas E-Stop/Fault to
  świadome, dydaktyczne założenie o zewnętrznym strumieniu wejściowym — nie jest to model
  rzeczywistego, safety-rated zachowania instalacji przemysłowej. Poza tym, co jawnie opisane wyżej,
  semantyka E-Stop/Fault się nie zmienia.

## Niezmienniki

1. Każdy `ItemId` oznacza jedną paczkę w całym aktywnym systemie: buforze i czterech strefach razem.
2. Bufor zachowuje kolejność FIFO opisaną wyżej.
3. Żadna utrata paczki bez widocznego wyniku (przejście na Infeed, wyjazd albo `nullopt` ze
   `runScenario` przy przepełnieniu).
4. Co najwyżej jeden ruch danej paczki w fizycznej linii na tick — teraz pięć punktów zamiast czterech
   (Diverting, Weighing→Diverting, PresenceCheck→Weighing, Infeed→PresenceCheck, Bufor→Infeed). Samo
   przyjęcie zewnętrzne do bufora **nie jest** „ruchem w linii” i nie liczy się do tego niezmiennika —
   liczone punkty zaczynają się dopiero od Bufor→Infeed.
5. Istniejące bezpieczeństwo korelacji `ItemId` w czujnikach (`presenceObservedItemId` /
   `weightObservedItemId`) — bez zmian, bufor leży w całości przed PresenceCheck.
6. Brak obejścia semantyki E-Stop/Fault/RampingUp.
7. Determinizm zachowany.
8. Kolejność elementów w `Scenario::arrivals` nadal nie niesie znaczenia.
9. Unikalność aktywnego `ItemId` na publicznym API `Engine`, jak wyżej.

## Kryteria akceptacji

Rozwiązanie musi przejść wspólny scenariusz akceptacyjny (opis zachowania, nie gotowy kod — buduje się
go samodzielnie jako `Scenario`):

1. Linia startuje, po niej niemal natychmiast E-Stop.
2. Dwie paczki przybywają w trakcie zatrzymania i trafiają do bufora.
3. Zwolnienie E-Stop, reset, ponowny start — system zaczyna opróżniać bufor.
4. Kolejna, trzecia paczka przybywa już po wznowieniu.
5. Wszystkie trzy paczki docierają ostatecznie do poprawnych wyjść (zgodnie z ich masą/klasyfikacją).
6. Ślad pokazuje, że paczki wchodzą na linię w kolejności przybycia (FIFO), żadna nie ginie i żadna nie
   zostaje pominięta podczas zatrzymania.

**To musi działać już przy minimalnej dozwolonej pojemności bufora (2)** — nie warto projektować
scenariusza akceptacyjnego (ani jego przejścia) w sposób zależny od większej, wybranej pojemności.

Uruchomienie tego samego scenariusza dwa razy musi dać ślady semantycznie identyczne — ten sam
determinizm, co wymagany od modułu 9.

## Własne testy

Minimalne wymaganie: **co najmniej 3 małe testy, pokrywające co najmniej 3 różne kategorie** z listy
poniżej (nie 3 warianty tego samego przypadku):

- FIFO pod obciążeniem buforowym.
- Przybycie przy zajętym, ale niepełnym buforze (sukces, tam gdzie dziś byłaby porażka).
- Przepełnienie bufora — łącznie z momentem oceny (przed zmianami danego ticku): miejsce zwolnione
  dopiero w tym samym ticku nie ratuje odrzuconego przybycia.
- Unikalność aktywnego `ItemId` na publicznym API `Engine`.
- Zachowanie bufora w trakcie zatrzymania, E-Stop, Fault i przy powrocie do pracy — bez cichej utraty
  paczek.
- Powtarzalność: ten sam `Scenario` dwa razy → identyczny ślad.

Zastosuj zasadę z ćwiczenia Project Kickoff: każdy test powinien wykrywać konkretną błędną
implementację, a nie tylko przechodzić dla poprawnego kodu.

## Demonstracja przez Scenario

Poza testami jednostkowymi rozwiązanie musi zawierać **demonstrację przez `Scenario`** — pełny,
uruchamialny scenariusz (analogicznie do `recoveryDemoScenario`/`multiParcelDemoScenario` z modułu 9),
który pokazuje działanie bufora w praktyce. Scenariusz akceptacyjny z sekcji wyżej może pełnić tę
rolę.

## Uzasadnienie decyzji projektowych (krótkie, pisemne)

Przed rozpoczęciem implementacji napisz kilka zdań uzasadnienia dla dwóch decyzji opisanych niżej:

- **Gdzie mieszka bufor** (jaki byt jest jego właścicielem) — i dlaczego to miejsce, a nie inne.
  Rozważ oba warianty: czy jest to stan fizyczny, czy stan
  eksperymentu; czy bufor pasuje bardziej do stref, którymi już włada `Plant`, czy do komponentów
  „urządzeniowych”, którymi włada `Engine` (jak `Diverter`/`BeltMotor`); czy bufor istnieje także w
  trybie bezpośrednim (bez `Scenario`) w obu wariantach; jak każdy z wariantów wpływa na
  `Engine::step()` i testowalność. Porównaj warianty i uzasadnij wybór.
- **Sposób doboru pojemności** i mechanizm jej ustawienia.

To krótki dokument (kilkanaście zdań wystarczy), nie osobny raport — ale będzie punktem wyjścia do
indywidualnej obrony (patrz niżej).

## Decyzje projektowe — wybierasz i uzasadniasz

- **Miejsce własności bufora.** Bufor jako część `Plant` (obok istniejących stref) — albo jako osobny
  komponent, którym włada `Engine`, analogicznie do `Diverter`/`BeltMotor`, które `Engine` już dziś
  posiada. Obie opcje są równorzędne — żadna nie jest sugerowana jako „bardziej poprawna”;
  wybór i uzasadnienie należą do Ciebie (patrz „Uzasadnienie decyzji projektowych” wyżej). Jedyna
  wykluczona opcja to bufor jako prywatny stan wyłącznie wewnątrz `runScenario`/`Scenario`, czyli
  niewidoczny przy sterowaniu bezpośrednim.
- **Wartość pojemności powyżej minimum 2** i sposób jej ustawienia (stała czy pole
  konfigurowalne).
- **Nazwa nowego API przybyć** (`acceptArrival` jest sugerowane, ale wybór należy do Ciebie).
- **Wewnętrzna reprezentacja bufora** — o ile pozostaje prywatna i chroni FIFO/pojemność.
- **Nazewnictwo i dekompozycja** nowych funkcji pomocniczych.

## Rozszerzenia opcjonalne — poza wymaganiami obowiązkowymi

Poniższe **nie są wymagane** do pełnej podstawowej oceny i leżą poza budżetem obowiązkowej ścieżki.
Warto podjąć je tylko, jeśli starczy na to czasu i ochoty — nierówna trudność między nimi jest
zamierzona, nie warto robić wszystkich naraz:

- Wiele przybyć w tym samym ticku, z jawną, deterministyczną regułą rozstrzygania kolejności (nigdy
  opartą o przypadkową pozycję w `std::vector`).
- Bogatsza sygnalizacja lub konfigurowalna strategia przepełnienia.
- Generator `ItemId`.
- Zróżnicowane priorytety przybyć.
- Serializacja `Scenario`.
- Bogatszy ślad/statystyki (np. maksymalna głębokość bufora w przebiegu).

## Praca w Git i złożenie projektu

- Punktem startowym jest tag `final-project-start-v3` na osobistej gałęzi.
- Historia commitów powinna pokazywać realny postęp — kilka commitów odzwierciedlających kolejne
  kroki (np. szkic typu bufora, integracja z resztą linii, rozszerzenie śladu, własne testy, scenariusz
  akceptacyjny), nie jeden duży commit na końcu. Nie jest wymagany zaawansowany sposób pracy z
  Gitem (bez wymuszania rebase/squash) — wystarczy czytelna historia na własnej gałęzi.
  Bezpośrednie commity do gałęzi współdzielonej są niedozwolone.
- W połowie pracy warto zrobić sobie lekki, nieformalny przegląd własnej pracy: czy wybrany kierunek
  architektury rzeczywiście daje się uzasadnić. Na tym etapie łatwiej jeszcze zmienić kierunek.
- Finalne złożenie projektu: czysta gałąź/PR zawierająca implementację, własne testy, scenariusz
  demonstracyjny i krótkie uzasadnienie decyzji projektowych.

## Obrona

Zielone testy nie są dowodem zrozumienia. Po złożeniu projektu odbędzie się krótka, **indywidualna**
obrona (nawet jeśli projekt powstał w parze) — wyjaśnienie własnych decyzji, przewidywanie zachowania
systemu w nowych sytuacjach, i mały, wcześniej niewidziany problem do rozwiązania na podstawie
własnego kodu. Warto pisać kod, który da się wytłumaczyć, nie tylko taki, który przechodzi testy.
