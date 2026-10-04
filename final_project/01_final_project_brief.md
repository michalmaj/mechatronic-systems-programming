🇵🇱 Polski | [🇬🇧 English](01_final_project_brief.en.md)

# Projekt końcowy: bufor wejściowy

## Kontekst

Paczki mogą docierać szybciej, niż linia jest w stanie je przyjąć. Dlatego przed strefą `Infeed`
potrzebny jest bufor wejściowy.

W `module-09-solution` każda paczka zapisana w `Scenario::arrivals` trafia do
`Engine::spawnItem()`. Jeżeli strefa `Infeed` jest zajęta, `runScenario()` zwraca `std::nullopt` i
przerywa scenariusz. W projekcie końcowym dodasz bufor, w którym paczka zaczeka na zwolnienie wejścia.

Projekt wykonujesz po ukończeniu modułów 0–9. Tag `final-project-start-v3` zawiera funkcjonalnie ten
sam symulator co `module-09-solution`. Rozszerz istniejący kod; nie twórz obok niego drugiej,
niezależnej implementacji.

Poniżej znajdziesz wymagane zachowanie i sposób jego sprawdzenia. Samodzielnie zaprojektujesz część
architektury. Potrzebna będzie kolejka FIFO, którą można oprzeć na `std::deque`. Do Ciebie należy
wybór jej właściciela oraz publicznego interfejsu.

## Wymagania obowiązkowe

Niezależnie od podjętych decyzji projektowych, poniższe musi być prawdziwe w rozwiązaniu:

- **Bufor jest trwałą częścią stanu systemu**, niezależną od `Scenario`. Musi działać tak samo przy
  sterowaniu bezpośrednim i przez `Scenario`. Nie może być prywatnym stanem funkcji `runScenario`.
- **Enkapsulacja.** Kolejność FIFO i pojemność muszą być chronione przez typ z prywatnym kontenerem.
  Publiczny `std::deque<Item>` pozwalałby dowolnemu fragmentowi programu ominąć te zasady, dlatego
  nie spełnia wymagania.
- **Dwa oddzielne interfejsy.** `Engine::spawnItem(id, mass)` zachowuje obecne działanie. Dodaje
  paczkę bezpośrednio do `Infeed`, a odrzuca ją, gdy strefa jest zajęta albo identyfikator się
  powtarza. Dodaj osobny interfejs do przyjmowania nowych paczek. Sugerowana nazwa to
  `acceptArrival`, ale możesz wybrać inną. Tylko ten nowy interfejs korzysta z bufora i to on ma
  zastąpić `spawnItem` podczas obsługi `Scenario::arrivals` w `runScenario`.
- **FIFO.** Żadna później przyjęta paczka nie może wejść do `Infeed` przed wcześniejszą paczką wciąż
  oczekującą w buforze.
- **Ograniczona pojemność, minimum 2.** Bufor musi mieć górną granicę. Próba jej przekroczenia musi
  zostać jawnie zgłoszona; paczka nie może zniknąć bez informacji o odrzuceniu.
- **Moment oceny przepełnienia.** Dostępność miejsca sprawdzaj podczas obsługi nowych paczek, na
  podstawie stanu bufora z końca poprzedniego ticku. Musi się to wydarzyć **przed** pozostałymi
  działaniami bieżącego ticku. Jeśli miejsce zwolni się dopiero później w tym samym ticku, nowa
  paczka nadal zostaje odrzucona.
- **Kolejność w ticku.** Przejście bufor→Infeed następuje **po** rozstrzygnięciu przejścia
  Infeed→PresenceCheck. Może więc wykorzystać miejsce zwolnione w `Infeed` w bieżącym ticku. Paczka
  przeniesiona właśnie do `Infeed` **nie porusza się dalej w tym samym ticku**.
- **Zależność od gotowości pasa.** Paczkę można dodać do bufora niezależnie od trybu pracy linii,
  także podczas postoju. Przejście z bufora do `Infeed` wymaga natomiast stanu
  `beltMotor_.actualState() == Running`, tak jak każdy inny ruch na linii. Obowiązuje również
  opóźnienie `RampingUp`: po uruchomieniu ze stanu `Stopped` pas potrzebuje jednego ticku, aby zacząć
  się poruszać. Paczka dodana do bufora w chwili uruchomienia linii może więc czekać o jeden tick
  dłużej. Uwzględnij ten przypadek we własnych testach.
- **Reguła „co najwyżej jedna nowa paczka na tick” w `isValidScenario` zostaje bez zmian.** Obsługa
  kilku nowych paczek w tym samym ticku jest rozszerzeniem opcjonalnym i nie należy do wspólnego
  rdzenia projektu.
- **Unikalność aktywnego `ItemId`.** Publiczny interfejs `Engine` (`spawnItem` oraz nowa funkcja)
  gwarantuje, że dwie aktywne paczki o tym samym `ItemId` nie mogą jednocześnie istnieć w buforze ani w
  żadnej strefie. Zasada obowiązuje zarówno dla wywołań pochodzących ze `Scenario`, jak i dla
  bezpośredniego sterowania przez kod.
- **Dane wymagane w śladzie.** `TickResult` musi udostępniać dwa nowe pola: `bufferedCount` (liczba
  paczek czekających w buforze) oraz
  `bufferHeadItemId` (`std::optional<ItemId>`, czyli identyfikator pierwszej paczki w kolejce albo
  `std::nullopt`, gdy bufor jest pusty). Te informacje wystarczą do sprawdzenia kolejności FIFO bez
  niepotrzebnego rozbudowywania `TickResult`.
- **Powtarzalność.** Dwukrotne uruchomienie tego samego poprawnego `Scenario` musi dać te same
  wartości i ten sam przebieg zdarzeń w śladzie.
- **Bez zmian w zasadach bezpieczeństwa.** Dodawanie paczek do bufora podczas `EStopped` lub `Fault`
  jest uproszczeniem przyjętym w tym zadaniu. Nie należy traktować go jako projektu rzeczywistego
  układu spełniającego wymagania bezpieczeństwa przemysłowego. Poza zasadami opisanymi wyżej
  działanie `EStopped` i `Fault` się nie zmienia.

## Niezmienniki

1. Każdy `ItemId` oznacza jedną paczkę w całym aktywnym systemie: buforze i czterech strefach razem.
2. Bufor zachowuje kolejność FIFO opisaną wyżej.
3. Żadna paczka nie ginie bez widocznego wyniku. Takim wynikiem może być przeniesienie do `Infeed`,
   wyjazd z linii albo `std::nullopt` zwrócone przez `runScenario` przy przepełnieniu.
4. Każda paczka wykonuje co najwyżej jeden ruch na linii w jednym ticku. Reguła obejmuje teraz pięć
   punktów: Diverting, Weighing→Diverting, PresenceCheck→Weighing, Infeed→PresenceCheck oraz
   Bufor→Infeed. Dodanie nowej paczki do bufora **nie jest** ruchem na linii. Liczenie zaczyna się
   dopiero od przejścia Bufor→Infeed.
5. Dotychczasowe zasady przypisywania odczytów czujników do właściwego `ItemId`
   (`presenceObservedItemId` / `weightObservedItemId`) pozostają bez zmian. Bufor znajduje się przed
   PresenceCheck.
6. Zasady działania `EStopped`, `Fault` i `RampingUp` pozostają zachowane.
7. Każde uruchomienie z tymi samymi danymi daje ten sam wynik.
8. Kolejność elementów w `Scenario::arrivals` nadal nie wpływa na wynik.
9. Publiczny interfejs `Engine` nie pozwala dodać dwóch aktywnych paczek o tym samym `ItemId`.

## Kryteria akceptacji

Rozwiązanie musi przejść poniższy scenariusz odbiorczy. Jest to opis zachowania, a nie gotowy kod.
Scenariusz zapisujesz samodzielnie jako `Scenario`.

1. Linia startuje, po niej niemal natychmiast E-Stop.
2. Dwie paczki docierają w trakcie zatrzymania i trafiają do bufora.
3. Po zwolnieniu E-Stop, resecie i ponownym uruchomieniu system zaczyna opróżniać bufor.
4. Trzecia paczka dociera już po wznowieniu pracy.
5. Wszystkie trzy paczki docierają ostatecznie do poprawnych wyjść (zgodnie z ich masą/klasyfikacją).
6. Ślad pokazuje, że paczki wchodzą na linię w kolejności, w której dotarły (FIFO). Żadna nie ginie
   ani nie zostaje pominięta podczas zatrzymania.

**Scenariusz musi działać przy minimalnej dozwolonej pojemności bufora, czyli 2.** Nie uzależniaj go
ani implementacji od większej, wybranej przez siebie pojemności.

Dwukrotne uruchomienie tego samego scenariusza musi dać taki sam ślad, zgodnie z zasadą
powtarzalności obowiązującą od modułu 9.

## Własne testy

Napisz **co najmniej 3 małe testy, które obejmują co najmniej 3 różne kategorie** z poniższej listy.
Trzy warianty tego samego przypadku nie spełniają tego wymagania.

- Kolejność FIFO przy kilku paczkach czekających w buforze.
- Dodanie paczki przy zajętym, ale niepełnym buforze. Operacja ma się powieść, choć bez nowego bufora
  zakończyłaby się niepowodzeniem.
- Przepełnienie bufora, z uwzględnieniem momentu oceny przed zmianami danego ticku. Miejsce zwolnione
  dopiero w tym samym ticku nie zapobiega odrzuceniu nowej paczki.
- Unikalność aktywnego `ItemId` w publicznym interfejsie `Engine`.
- Zachowanie bufora w trakcie zatrzymania, E-Stop, Fault i przy powrocie do pracy, bez cichej utraty
  paczek.
- Powtarzalność: ten sam `Scenario` dwa razy → identyczny ślad.

Zastosuj zasadę z ćwiczenia przygotowującego: każdy test powinien wykrywać konkretną błędną
implementację, a nie tylko przechodzić dla poprawnego kodu.

## Scenariusz demonstracyjny

Poza testami jednostkowymi rozwiązanie musi zawierać kompletny, uruchamialny `Scenario`, który
pokazuje działanie bufora. Wzoruj się na `recoveryDemoScenario` i `multiParcelDemoScenario` z modułu
9. Rolę demonstracji może pełnić scenariusz odbiorczy opisany wyżej.

## Uzasadnienie decyzji projektowych (krótkie, pisemne)

Przed rozpoczęciem implementacji napisz kilka zdań uzasadnienia dla dwóch decyzji opisanych niżej:

- **Gdzie będzie przechowywany bufor** i dlaczego wybierasz właśnie to miejsce. Rozważ oba warianty.
  Zastanów się, czy bufor jest stanem fizycznym, czy częścią przebiegu eksperymentu. Porównaj go ze
  strefami należącymi do `Plant` oraz urządzeniami należącymi do `Engine`, takimi jak `Diverter` i
  `BeltMotor`. Sprawdź też, czy w obu wariantach bufor istnieje podczas sterowania bez `Scenario`
  oraz jaki wpływ ma wybór na `Engine::step()` i łatwość testowania.
- **Sposób doboru pojemności** i mechanizm jej ustawienia.

To krótki dokument, nie osobny raport. Wystarczy kilkanaście zdań. Będzie punktem wyjścia do rozmowy
podsumowującej projekt.

## Decyzje projektowe, które trzeba uzasadnić

- **Właściciel bufora.** Bufor może być częścią `Plant`, obok istniejących stref, albo osobnym
  komponentem, którym włada `Engine`, analogicznie do `Diverter` i `BeltMotor`, które `Engine` już
  posiada. Obie możliwości są równorzędne. Wybierz jedną i uzasadnij decyzję. Bufor nie może być
  wyłącznie prywatnym stanem `runScenario` lub `Scenario`, niewidocznym przy sterowaniu
  bezpośrednim.
- **Wartość pojemności powyżej minimum 2** i sposób jej ustawienia (stała czy pole
  konfigurowalne).
- **Nazwa nowej funkcji do przyjmowania paczek.** Sugerowana nazwa to `acceptArrival`, ale wybór
  należy do Ciebie.
- **Wewnętrzna reprezentacja bufora**, o ile pozostaje prywatna i chroni kolejność FIFO oraz
  pojemność.
- **Nazwy i podział odpowiedzialności** nowych funkcji pomocniczych.

## Rozszerzenia opcjonalne

Poniższe rozszerzenia **nie są wymagane** do uzyskania pełnej oceny za podstawową wersję projektu.
Wybierz któreś z nich tylko wtedy, gdy masz na to czas i ochotę. Różnią się poziomem trudności i nie
ma potrzeby realizowania wszystkich.

- Wiele nowych paczek w tym samym ticku, z jednoznaczną i powtarzalną regułą ustalania kolejności.
  Reguła nie może zależeć od przypadkowej pozycji w `std::vector`.
- Bogatsza sygnalizacja lub konfigurowalna strategia przepełnienia.
- Generator `ItemId`.
- Różne priorytety przyjmowanych paczek.
- Serializacja `Scenario`.
- Bogatszy ślad/statystyki (np. maksymalna głębokość bufora w przebiegu).

## Praca w Git i oddanie projektu

- Punktem startowym jest tag `final-project-start-v3` na osobistej gałęzi.
- Historia commitów powinna pokazywać kolejne etapy pracy, na przykład szkic typu bufora, połączenie
  go z resztą linii, rozszerzenie śladu, własne testy i scenariusz odbiorczy. Nie odkładaj wszystkich
  zmian do jednego dużego commita na końcu. Nie wymagamy operacji takich jak rebase czy squash.
  Wystarczy czytelna historia na własnej gałęzi.
  Bezpośrednie commity do gałęzi współdzielonej są niedozwolone.
- Mniej więcej w połowie pracy zatrzymaj się i sprawdź, czy nadal potrafisz uzasadnić wybraną
  architekturę. Na tym etapie można jeszcze stosunkowo łatwo zmienić kierunek.
- Do oddania przygotuj czystą gałąź i pull request zawierający implementację, własne testy, scenariusz
  demonstracyjny oraz krótkie uzasadnienie decyzji projektowych.

## Rozmowa podsumowująca

Przechodzące testy nie wystarczą do sprawdzenia, czy autor rozumie własne rozwiązanie. Po oddaniu
projektu odbędzie się krótka, **indywidualna rozmowa**, również wtedy, gdy projekt powstał w parze.
Trzeba będzie wyjaśnić podjęte decyzje, przewidzieć zachowanie systemu w nowej sytuacji i rozwiązać
niewielki problem na podstawie własnego kodu. Dlatego warto pisać kod, który potrafisz wyjaśnić, a
nie tylko taki, który przechodzi testy.
