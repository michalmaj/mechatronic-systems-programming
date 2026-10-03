🇵🇱 Polski | [🇬🇧 English](handbook.en.md)

# Podręcznik kursu

← [README](../README.md) · [Plan pracy](roadmap.md)

Podręcznik obejmuje moduły 0–9 oraz przygotowanie do projektu końcowego. Punktem odniesienia dla
każdego rozdziału są kod i testy z odpowiedniego tagu (`module-XX-start` albo
`module-XX-solution`). Jeśli opis nie zgadza się z kodem, kieruj się kodem i zgłoś problem przez
Issues.

**Spis treści:** [0](#0-jak-korzystać-z-podręcznika) · [1](#1-modelowanie-prostego-procesu) ·
[2](#2-polecenie-to-nie-stan-fizyczny) · [3](#3-orkiestracja-systemu) ·
[4](#4-aktuator-i-tryb-pracy) · [5](#5-niezależna-ścieżka-awaryjnego-stopu) ·
[6](#6-czujniki-i-jakość-danych) · [7](#7-usterki-i-zdarzenia-systemowe) ·
[8](#8-wiele-paczek-i-niezmienniki) · [9](#9-scenariusze-jako-dane) ·
[10](#10-jak-czytać-i-pisać-test) · [11](#11-projekt-końcowy--jak-podejść-do-nowego-wymagania) ·
[Dodatek A](#dodatek-a--słownik) · [Dodatek B](#dodatek-b--komendy) ·
[Dodatek C](#dodatek-c--mapa-typówapi)

---

## 0. Jak korzystać z podręcznika

Praca nad każdą misją wygląda podobnie: piszesz fragment programu, uruchamiasz go, oglądasz wynik, a
na końcu sprawdzasz rozwiązanie dostarczonym testem. Najpierw spróbuj przewidzieć i zobaczyć działanie
programu. Test służy do potwierdzenia wyniku, a nie do zgadywania wymagań metodą prób i błędów.

Na początku warto zapamiętać cztery zasady:

- Symulacja działa w dyskretnych, deterministycznych krokach, tickach. Każde wywołanie
  `Engine::step()` wykonuje jeden tick: odczytuje bieżący stan, wyznacza zmiany i zwraca
  `TickResult`. Ten sam ciąg wejść zawsze prowadzi do tego samego ciągu wyników.
- Testy dostarcza kurs. Nie piszesz ich sam aż do ćwiczenia przed projektem końcowym (rozdział 10) —
  masz je przeczytać, zrozumieć i uruchomić. Test zapisuje wymagania misji w postaci kodu:
  zielony wynik potwierdza wymagane zachowanie i kończy misję, ale sam w sobie nie dowodzi
  zrozumienia. Temu służą krótkie rozmowy sprawdzające po modułach 3, 7 i 9.
- CMake i CTest to narzędzia, nie przedmiot nauki. Konfigurujesz preset, budujesz, uruchamiasz testy
  — to wszystko, co musisz wiedzieć o samym CMake w tym kursie (Dodatek B).
- Pracujesz na tagach. Każdy moduł zaczynasz od `git switch -c <twoja-gałąź> module-XX-start`.
  Commitujesz na własnej gałęzi; tagi kursu zostają nietknięte jako punkt odniesienia.

---

## 1. Modelowanie prostego procesu

**Moduł 1 · `module-01-start` · misje 1–6**

Zaczynasz od prostej wersji sortowni, w której na linii znajduje się najwyżej jedna paczka. Paczka
wjeżdża, zostaje sklasyfikowana i przechodzi przez kolejne strefy. Nie ma jeszcze aktuatorów ani
opóźnień wynikających z ich ruchu.

Pierwszy model domeny:

```cpp
enum class Zone { Infeed, PresenceCheck, Weighing, Diverting, OutputLight, OutputHeavy };

struct Item {
    ItemId id;
    Zone zone;
    Grams mass;
};

struct Plant {
    std::optional<Item> item;
};
```

Model zawiera jeden `std::optional<Item>` i nie potrzebuje jeszcze kontenera. Przy tej okazji
poznajesz podstawowe elementy C++ używane w dalszej części kursu: `enum class` opisuje zamknięty zbiór
stanów, `struct` grupuje proste dane, a `std::optional` mówi wprost, że wartości może nie być. Logikę
tworzą na razie wolne funkcje: `spawnItem`, `advance` i `classify`.

Pierwsza pętla sterowania w CLI pokazuje schemat, który będzie wracał przez cały kurs: na podstawie
stanu wybierz polecenie, wykonaj krok i wypisz wynik. W kolejnych modułach stan stanie się bogatszy,
ale ta kolejność pozostanie bez zmian.

---

## 2. Polecenie to nie stan fizyczny

**Moduł 2 · `module-02-start` · misje 7–9**

Wysłanie polecenia do urządzenia nie oznacza, że urządzenie zdążyło już je wykonać.
Dywerter, który ma się przesunąć w pozycję `Diverted`, potrzebuje na to czasu. Kod, który zakłada
natychmiastowość, jest po prostu błędny, nawet jeśli się kompiluje i wygląda dobrze.

```cpp
enum class DiverterCommand { HoldStraight, Divert };
enum class DiverterPosition { Straight, Moving, Diverted };

class Diverter {
public:
    void setCommand(DiverterCommand command);
    void resolve();
    DiverterPosition actualPosition() const;
    bool isSettled() const;
};
```

To pierwsza klasa, którą zbudujesz w tym kursie. `Diverter` przechowuje zarówno wydane polecenie
(`command`), jak i rzeczywiste położenie (`actualPosition`). Pola prywatne chronią stan aktuatora
przed bezpośrednią zmianą z zewnątrz. Ten sam podział między poleceniem a wykonaniem wróci przy
`BeltMotor` w module 4.

`Plant::advance()` musi teraz poczekać, aż dywerter się ustabilizuje (`isSettled()`), zanim paczka
faktycznie odjedzie. To pierwszy moment, w którym czas — liczba ticków — staje się częścią logiki, a
nie tylko licznikiem pętli.

---

## 3. Orkiestracja systemu

**Moduł 3 · `module-03-start` · misje 10–12**

Do tej pory funkcja `main()` w CLI samodzielnie ustalała kolejność wszystkich operacji. W tym module
odpowiedzialność przejmuje `Engine`.

```cpp
struct TickResult { /* pełny, jednoznaczny opis jednego ticku */ };

class Engine {
public:
    bool spawnItem(ItemId id, Grams mass);
    TickResult step();
private:
    Plant plant_;
    Diverter diverter_;
};
```

`Engine` przechowuje `Plant` i `Diverter` jako pola. Nie dziedziczy po nich: składa kilka obiektów w
większą całość. To przykład kompozycji. `Engine` wywołuje funkcje sterujące z `Controller`, ale nie
wchłania ich jako własnych metod.

Zamiast udostępniać wiele osobnych getterów, `Engine::step()` zwraca jeden `TickResult` z pełnym
wynikiem ticku. Testy i CLI korzystają z tej wartości; nie zaglądają do stanu `Engine` w trakcie
wykonywania kroku.

Moduł ustala również kolejność operacji wewnątrz jednego kroku. W module 1 wynikała ona jedynie z
kodu w `main()`, teraz pilnuje jej `Engine`, a poprawność sprawdzają testy.

---

## 4. Aktuator i tryb pracy

**Moduł 4 · `module-04-start` · misje 13–15**

Linia potrzebuje jawnego trybu pracy. Jej napęd nie osiąga pełnej prędkości natychmiast: najpierw się
rozpędza, a podczas zatrzymywania zwalnia.

```cpp
enum class BeltMotorCommand { Stop, Run };
enum class BeltMotorState { Stopped, RampingUp, Running, RampingDown };
enum class Mode { Idle, Running };
```

(`Mode` rośnie w kolejnych modułach — `EStopped` dochodzi w module 5, `Fault` w module 7. Każdy stan
pojawia się dopiero razem z mechanizmem, który go potrzebuje.)

`BeltMotor` powtarza wzorzec polecenie/stan rzeczywisty z modułu 2, z jedną istotną nowością:
przejście między stanami zajmuje więcej niż jeden tick (`Stopped → RampingUp → Running`). To ma
realne konsekwencje w dalszych modułach i w projekcie końcowym — ten jeden tick opóźnienia decyduje o
tym, kiedy dokładnie coś może ruszyć się po linii.

`Mode` decyduje, czy dana część systemu może działać. Na przykład `advance()` przesuwa paczki dopiero
wtedy, gdy pas rzeczywiście znajduje się w stanie `Running`. Warunki zależne od trybu warto skupiać
w kilku jasno nazwanych funkcjach, zamiast powtarzać podobne instrukcje `if` w wielu miejscach.

Od tego modułu współpracuje już kilka niezależnych maszyn stanów naraz (`Mode`, `DiverterPosition`,
`BeltMotorState`). Każda ma własny zakres odpowiedzialności i żadna nie zna szczegółów pozostałych.

---

## 5. Niezależna ścieżka awaryjnego stopu

**Moduł 5 · `module-05-start` · misje 16–19**

> To, co budujesz w tym module, to uproszczony, dydaktyczny model wzorca „niezależna ścieżka
> bezpieczeństwa” w kodzie. Nie jest to projekt rzeczywistego systemu safety-rated i nie zastępuje
> kursu functional safety — uczysz się tu wzorca inżynierskiego, nie normy bezpieczeństwa.

Obsługa awaryjnego stopu tworzy osobną ścieżkę decyzji o wyższym priorytecie niż zwykłe sterowanie.
Dzięki temu może zatrzymać napęd bez względu na bieżący tryb pracy i pozostałe polecenia.

```cpp
enum class EStopLatchState { Released, Engaged, Armed };
```

`EStopLatchState` to zatrzask — stan, który nie cofa się sam z siebie. Naciśnięcie przycisku
wprowadza go w `Engaged`; samo zwolnienie przycisku nie wystarcza, żeby wrócić do `Released` —
potrzebny jest jeszcze jawny `Reset`, i to dopiero z pośredniego stanu `Armed`.

Tyle robi sam zatrzask; ponowne uruchomienie linii jest osobną operacją. Zwolnienie i reset odblokowują system i
sprowadzają `Mode` z powrotem do `Idle` — to jeszcze nie `Running`. Żeby linia faktycznie ruszyła,
potrzeba osobnego, kolejnego `StartRequested`, na późniejszym ticku. `Reset` i `StartRequested`
wysłane na tym samym ticku nie restartują systemu od razu: `modeStep` w tym ticku i tak sprowadza
`Mode` tylko do `Idle`, a `Running` wymaga oddzielnego wywołania startu, gdy `Mode` jest już `Idle`.
W ten sposób linia rusza dopiero po osobnym poleceniu operatora, a nie przy okazji skasowania stanu
awaryjnego.

`Mode::EStopped` ma wyższy priorytet niż pozostałe przejścia trybu. Kod zwykłego sterowania nie musi
samodzielnie obsługiwać każdego przypadku związanego z awaryjnym stopem.

---

## 6. Czujniki i jakość danych

**Moduł 6 · `module-06-start` · misje 20–24**

Do tej pory system zawsze znał poprawny stan linii. W rzeczywistej instalacji czujnik może się
zepsuć, dać nieaktualny odczyt albo w ogóle nie odpowiedzieć.

```cpp
enum class ReadingStatus { Ok, Missing, Stale };

struct PresenceReading { ReadingStatus status; bool occupied; };
struct WeightReading { ReadingStatus status; Grams grams; };
```

W symulatorze znamy zarówno rzeczywisty stan linii, jak i odczyt zgłoszony przez czujnik. Sterownik
korzysta wyłącznie z odczytu oraz jego `ReadingStatus`. Dzięki temu podlega takim samym ograniczeniom
jak program współpracujący z fizycznymi czujnikami.

Gdy odczyt jest `Stale`, czujnik podstawia ostatnią zaufaną wartość — ale tylko wtedy, gdy taka
wartość już istnieje. W przeciwnym razie zachowuje się tak samo jak `Missing`. Status `Missing`
oznacza brak odczytu i nie korzysta z poprzedniej wartości. Odróżniamy więc „brak danych” od
„ostatnia znana wartość może być nieaktualna”. Ta sama zasada obowiązuje dla czujnika obecności i
wagi, dzięki czemu zachowanie klasyfikacji podczas awarii jest przewidywalne.

Klasyfikacja korzysta z tych odczytów, ale musi też pamiętać coś własnego między tickami:
potwierdzenie obecności (`PresenceCheck`) i odczyt wagi (`Weighing`) to dwie osobne, sekwencyjne
strefy — paczka mija je jedna po drugiej, na różnych tickach. Żeby zaufać wadze później, system musi
pamiętać, że obecność była już wcześniej potwierdzona. Stąd `ControllerState` — mały rekord
(`presenceConfirmed`, `classification`) trzymany między tickami dla aktualnie przetwarzanej paczki,
aktualizowany co tick w miarę tego, jak paczka przechodzi przez kolejne strefy.

---

## 7. Usterki i zdarzenia systemowe

**Moduł 7 · `module-07-start` · misje 25–28**

W tym module oddzielisz usterkę podaną z zewnątrz od zdarzenia wykrytego przez sam system.

```cpp
enum class DiverterFaultKind { Blocked };
enum class SystemEventKind { DiverterNotReady, RoutingDeadlineMissed };
```

`DiverterFaultKind::Blocked` opisuje usterkę zadaną przez test albo scenariusz. W ten sposób
symulujemy mechaniczną blokadę. `SystemEventKind` opisuje natomiast wniosek programu: dywerter nie
osiągnął pozycji na czas (`DiverterNotReady`), a po przekroczeniu terminu pojawia się
`RoutingDeadlineMissed`.

Usterka jest wejściem symulacji, a zdarzenie jej wynikiem. Osobne typy pozwalają zachować tę granicę
również w kodzie.

`Mode::Fault` reaguje na `RoutingDeadlineMissed` i blokuje dalszy ruch, dopóki operator nie
zresetuje systemu. Przebieg wznowienia pracy przypomina moduł 5, choć tym razem zatrzymanie ma inną
przyczyną.

---

## 8. Wiele paczek i niezmienniki

**Moduł 8 · `module-08-start` · misje 29–32**

To największa przebudowa w rdzeniu kursu: system zaczyna obsługiwać kilka paczek jednocześnie.

> Wiele paczek naraz nie oznacza programowania współbieżnego. Rdzeń symulatora zostaje w pełni
> sekwencyjny i deterministyczny — jeden wątek, jeden `Engine::step()` na raz. Zmienia się tylko to,
> ile danych mieści `Plant` jednocześnie, nie model wykonania.

`Plant` przestaje trzymać jeden `std::optional<Item>` i dostaje cztery nazwane pola odpowiadające
strefom:

```cpp
struct Plant {
    std::optional<Item> infeed;
    std::optional<Item> presenceCheck;
    std::optional<Item> weighing;
    std::optional<Item> diverting;
};
```

Każde pole odpowiada jednej fizycznej strefie, która mieści najwyżej jedną paczkę. Typ
`std::optional` wymusza to ograniczenie bez dodatkowego komentarza i bez sprawdzania rozmiaru
kontenera.

`advance()` przetwarza przejścia w jednym, ustalonym kierunku: od wyjścia do wejścia — najpierw
strefa najbliżej wyjścia, na końcu Infeed. Dzięki temu paczka, która zwalnia jedną strefę, może od
razu skorzystać z miejsca w tym samym ticku (przesunięcie łańcuchowe), a jednocześnie zachowany jest
niezmiennik: żadna paczka nie może poruszyć się dwa razy w tym samym ticku. Wynika to z faktu, że
każde przejście rozpatrujemy raz, zawsze w tej samej kolejności.

`ItemId` staje się teraz kluczem korelacji dla każdej paczki z osobna, wszędzie tam, gdzie wcześniej
wystarczał sam fakt „czy coś tu jest”. Ślad (`TickResult`) musi jednoznacznie mówić, której paczki
dotyczy dana obserwacja czy zdarzenie, nawet gdy na linii jest ich kilka naraz.

---

## 9. Scenariusze jako dane

**Moduł 9 · `module-09-start` · misje 33–35**

Do tej pory każdy eksperyment wymagał ręcznego wywoływania metod `Engine`. Teraz cały przebieg można
zapisać w jednej strukturze danych:

```cpp
struct Scenario {
    std::vector<ScenarioInput> operatorInputs;
    std::vector<ScriptedItemArrival> arrivals;
    std::vector<ScriptedSensorFault> sensorFaults;
    std::vector<ScriptedDiverterFault> diverterFaults;
    Tick duration;
};

bool isValidScenario(const Scenario& scenario);
std::optional<std::vector<TickResult>> runScenario(const Scenario& scenario);
```

`Scenario` nie zastępuje mechanizmu sterowania. Funkcja `runScenario` wywołuje publiczne metody
`Engine` w chwilach i kolejności zapisanych w danych.

Przedział usterki zapisujemy jako `[from, until)`: usterka jest aktywna w ticku `from`, lecz już nie
w ticku `until`. Tę samą konwencję stosujemy w całym kursie.

Warto rozróżnić dwa rodzaje błędu: błąd statyczny, gdy `isValidScenario` zwraca `false` i scenariusz
w ogóle się nie uruchamia, bo jego opis jest sprzeczny sam w sobie; oraz błąd dynamiczny, gdy
scenariusz jest poprawny statycznie, ale czegoś nie da się wykonać w trakcie — `runScenario` zwraca
`std::nullopt` już po rozpoczęciu. To rozróżnienie, czy błąd widać bez uruchamiania czegokolwiek, czy
dopiero w trakcie, wraca w projekcie końcowym.

`runScenario` buduje świeży `Engine` przy każdym wywołaniu. To gwarantuje powtarzalność: ten sam,
poprawny `Scenario` uruchomiony dwa razy zawsze daje identyczny ślad.

---

## 10. Jak czytać i pisać test

Krótkie przygotowanie do pierwszego ćwiczenia, w którym sam piszesz test, a nie tylko go uruchamiasz.

Każdy test w tym kursie ma tę samą, prostą strukturę — nieformalne przygotuj, wywołaj, sprawdź:

```cpp
// przygotuj stan
psm::WeightReading reading{psm::ReadingStatus::Ok, 750};

// wywołaj to, co testujesz
auto result = psm::decideClassification(reading);

// sprawdź wynik
psmCheck(result == psm::WeightClass::Heavy, "750g przy poprawnym odczycie klasyfikuje się jako Heavy");
```

Test zapisuje oczekiwane zachowanie funkcji w kodzie, który można uruchomić. Dlatego poprawny wynik
`ctest -L misja-N` jest jednym z kryteriów ukończenia misji.

Własny test powinien wykrywać błąd, a nie tylko przechodzić dla obecnej implementacji. Możesz to
łatwo sprawdzić: świadomie zepsuj testowany kod i upewnij się, że test wtedy nie przechodzi. Takie
ćwiczenie znajdziesz w
[`final_project/00_project_kickoff.md`](../final_project/00_project_kickoff.md).

To nie jest rozdział o frameworkach testowych — kurs celowo używa jednej, minimalnej funkcji
(`psmCheck`) przez cały czas, żeby uwaga została przy tym, co i dlaczego testujesz, a nie przy
narzędziu.

---

## 11. Projekt końcowy — jak podejść do nowego wymagania

W projekcie końcowym samodzielnie zaprojektujesz większą zmianę. Wymaganie jest następujące: paczki
mogą przybywać szybciej, niż linia jest w stanie je przyjąć. Otrzymujesz też wspólny zestaw kryteriów
([`final_project/01_final_project_brief.md`](../final_project/01_final_project_brief.md)); resztę
projektujesz sam.

Poniższe wskazówki nie podają rozwiązania. Pomagają uporządkować pracę nad nowym wymaganiem w
istniejącym systemie:

1. Zacznij od kontraktu domenowego, nie od kodu. Zanim napiszesz linijkę C++, zapisz słownie, co ma
   być prawdą przed każdą operacją, którą dodajesz, i po niej.
2. Wypisz niezmienniki jawnie. Które z nich są zupełnie nowe? Które istniejące niezmienniki systemu
   muszą pozostać prawdziwe bez zmian? Które trzeba świadomie rozszerzyć?
3. Zdecyduj, kto jest właścicielem nowego stanu, zanim zaczniesz go implementować. Kto go trzyma? Czy
   ma sens tylko w jednym trybie sterowania systemem, czy w każdym?
4. Chroń niezmienniki enkapsulacją, nie umową dżentelmeńską. Jeśli coś musi zostać prawdziwe zawsze,
   powinno być niemożliwe do złamania przez publiczne API, a nie tylko „niezalecane”.
5. Zachowuj istniejące kontrakty, jeśli nie musisz ich świadomie zmienić. Rozszerzanie systemu to nie
   przepisywanie go od nowa ani obchodzenie tego, co już działa, równoległą implementacją.
6. Napisz własne testy — kategoriami zachowań, nie przypadkowymi przykładami. Sprawdź, czy każdy z
   nich faktycznie coś wykrywa (rozdział 10).
7. Zbuduj `Scenario`, które demonstruje działanie Twojego rozwiązania od początku do końca, dokładnie
   tak, jak `Scenario` demonstrowały gotowe zachowanie w module 9.
8. Uzasadnij swoje decyzje projektowe — krótko, pisemnie. To będzie punktem wyjścia do obrony.

Pełne wymagania, kryteria akceptacji i lista decyzji pozostawionych Tobie:
[`final_project/01_final_project_brief.md`](../final_project/01_final_project_brief.md). Materiał
czytasz na `main`; kod startowy — z tagu `final-project-start-v3`.

---

## Dodatek A — słownik

| PL | EN | Z kodu |
|---|---|---|
| tick | tick | `Tick` |
| krok symulacji | simulation step | `Engine::step()` |
| wynik ticku / ślad | tick result / trace | `TickResult`, `std::vector<TickResult>` |
| strefa | zone | `Zone` |
| paczka | item / parcel | `Item`, `ItemId` |
| polecenie | command | `DiverterCommand`, `BeltMotorCommand` |
| stan rzeczywisty | actual state | `DiverterPosition`, `BeltMotorState` |
| tryb pracy | operating mode | `Mode` |
| zatrzask (awaryjny stop) | latch | `EStopLatchState` |
| odczyt czujnika | sensor reading | `PresenceReading`, `WeightReading` |
| status odczytu | reading status | `ReadingStatus` |
| usterka (wstrzyknięta) | (injected) fault | `SensorFaultKind`, `DiverterFaultKind` |
| zdarzenie systemowe | system event | `SystemEventKind` |
| scenariusz | scenario | `Scenario` |
| odtwarzacz scenariusza | scenario replayer | `runScenario` |
| błąd statyczny | static failure | `isValidScenario(...) == false` |
| błąd dynamiczny | dynamic failure | `runScenario(...) == std::nullopt` |
| misja | mission | `ctest -L misja-N` |
| punkt startowy modułu | module starting point | tag `module-XX-start` |
| rozwiązanie referencyjne | reference solution | tag `module-XX-solution` |

## Dodatek B — komendy

**CMake / CTest**

```bash
cmake --preset dev                 # konfiguracja (raz, albo po zmianie CMakeLists.txt)
cmake --build --preset dev         # budowanie
ctest --preset test                # wszystkie testy
ctest --preset test -L misja-5     # tylko test danej misji
ctest --preset test -R nazwa_testu # test po nazwie (dopasowanie regex)
ctest --preset test --output-on-failure   # pokaż output nieudanych testów
```

**Git**

```bash
git fetch --tags                          # pobierz najnowsze tagi kursu
git switch -c my-work module-01-start     # własna gałąź z punktu startowego modułu
git add <plik>
git commit -m "krótki, konkretny opis"
git log --oneline                         # historia Twoich commitów
```

## Dodatek C — mapa typów/API

Skrócona mapa najważniejszych typów rdzenia kursu — nie pełna dokumentacja API. Po szczegóły sięgaj
do nagłówków w `include/psm/` i materiału danego modułu.

| Typ | Gdzie | Rola |
|---|---|---|
| `Plant` | `plant.hpp` | fizyczny stan linii — pola odpowiadające strefom |
| `Item` / `ItemId` | `item.hpp` | paczka i jej tożsamość |
| `Diverter` | `diverter.hpp` | aktuator kierujący paczki na wyjście |
| `BeltMotor` | `belt_motor.hpp` | napęd taśmy, z rampowaniem |
| `Mode` | `mode.hpp` | tryb pracy całego systemu |
| `EStopLatchState` | `estop_latch.hpp` | zatrzask bezpieczeństwa |
| `PresenceSensor` / `WeightSensor` | `presence_sensor.hpp`, `weight_sensor.hpp` | czujniki z niepewnością odczytu |
| `Engine` | `engine.hpp` | orkiestrator jednego ticku, publiczne API systemu |
| `TickResult` | `tick_result.hpp` | pełny, jednoznaczny wynik jednego ticku |
| `Scenario` / `runScenario` | `scenario.hpp` | deklaratywny opis i odtwarzanie eksperymentu |
