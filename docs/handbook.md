🇵🇱 Polski | [🇬🇧 English](handbook.en.md)

# Podręcznik kursu

← [README](../README.md) · [Roadmap](roadmap.md)

Spójny materiał dydaktyczny do Course Core 0–9 i wstęp do projektu końcowego. Źródłem prawdy dla
każdego rozdziału jest aktualny kod i testy odpowiadającego tagu (`module-XX-start` /
`module-XX-solution`) — jeśli coś tutaj wygląda inaczej niż kod, którym operujesz, kod wygrywa; zgłoś
to przez Issues.

**Spis treści:** [0](#0-jak-korzystać-z-podręcznika) · [1](#1-modelowanie-prostego-procesu) ·
[2](#2-polecenie-to-nie-stan-fizyczny) · [3](#3-orkiestracja-systemu) ·
[4](#4-aktuator-i-tryb-pracy) · [5](#5-niezależna-ścieżka-e-stop) ·
[6](#6-czujniki-i-jakość-danych) · [7](#7-usterki-i-zdarzenia-systemowe) ·
[8](#8-wiele-paczek-i-niezmienniki) · [9](#9-scenariusze-jako-dane) ·
[10](#10-jak-czytać-i-pisać-test) · [11](#11-projekt-końcowy--jak-podejść-do-nowego-wymagania) ·
[Dodatek A](#dodatek-a--słownik) · [Dodatek B](#dodatek-b--komendy) ·
[Dodatek C](#dodatek-c--mapa-typówapi)

---

## 0. Jak korzystać z podręcznika

Ten kurs uczy przez trzy powtarzające się kroki: **kod → eksperyment → test.** Piszesz kawałek
implementacji, uruchamiasz symulator (przez CLI albo test), patrzysz na wynik, i dopiero wtedy
sprawdzasz go dostarczonym testem. Nie odwrotnie — test istnieje po to, żeby potwierdzić to, co już
zaobserwowałeś/aś, nie żeby zgadywać, co ma robić kod.

Kilka rzeczy, które są prawdziwe przez cały kurs i warto je zrozumieć raz, na początku:

- **Symulacja działa w dyskretnych, deterministycznych krokach — tickach.** Każde wywołanie
  `Engine::step()` to jeden tick: przeczytaj bieżący stan, zdecyduj, co się zmienia, zwróć wynik
  (`TickResult`). Ten sam ciąg wejść zawsze daje ten sam ciąg wyników.
- **Testy są dostarczone przez kurs.** Nie piszesz ich sam/a aż do Project Kickoff (rozdział 10) —
  Twoim zadaniem jest je zrozumieć i uruchomić, nie zaprojektować od zera. Test jest ostatecznym,
  wykonywalnym kontraktem misji: jeśli jest zielony, misja jest zrobiona.
- **CMake/CTest to narzędzia, nie przedmiot nauki.** Konfigurujesz preset, budujesz, uruchamiasz testy
  — to wszystko, czego potrzebujesz umieć o samym CMake w tym kursie (patrz Dodatek B).
- **Pracujesz na tagach.** Każdy moduł zaczyna się od `git switch -c <twoja-gałąź> module-XX-start`.
  Commitujesz na własnej gałęzi; tagi kursu pozostają nietknięte i są Twoim punktem odniesienia.

---

## 1. Modelowanie prostego procesu

**Moduł 1 · `module-01-start` · misje 1–6**

Zaczynasz od najmniejszej możliwej wersji problemu: **jedna paczka, jedna strefa na raz.** Paczka
wjeżdża, zostaje sklasyfikowana, i „przesuwa się” po strefach — na razie bez żadnego aktuatora, bez
czasu reakcji, bez wielu paczek naraz.

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

To jest celowo najprostszy możliwy kształt — jeden `std::optional<Item>`, żadnego kontenera. Uczysz się
tu podstawowego słownika C++, którego kurs używa wszędzie dalej: `enum class` do zamkniętego zbioru
stanów, `struct` do prostych danych, `std::optional` do „może nie ma wartości” zamiast wskaźnika albo
wartowniczej stałej, oraz wolnych funkcji (`spawnItem`, `advance`, `classify`) jako podstawowej jednostki
logiki — jeszcze bez klas.

Pierwsza pętla sterowania w CLI (uproszczona) pokazuje już cały rytm kursu: ustal polecenie na
podstawie stanu, wykonaj krok, wypisz wynik. Ten rytm — *decyzja → krok → obserwacja* — wraca w każdym
kolejnym module, tylko z coraz bogatszym stanem.

---

## 2. Polecenie to nie stan fizyczny

**Moduł 2 · `module-02-start` · misje 7–9**

Kluczowa lekcja tego modułu: **wysłanie polecenia do urządzenia nie oznacza, że urządzenie już
wykonało to polecenie.** Dywerter, który ma się przesunąć w pozycję `Diverted`, potrzebuje czasu.
Zanim to zamodelujesz, kod, który zakłada natychmiastowość, jest po prostu błędny — nawet jeśli się
kompiluje i „wygląda dobrze”.

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

To Twoja pierwsza własna klasa w kursie. Uczysz się enkapsulacji nie jako abstrakcyjnej zasady, tylko
jako konkretnego narzędzia: `Diverter` chroni rozróżnienie między tym, co **poleciłeś** (`command`), a
tym, co **faktycznie się dzieje** (`actualPosition`) — reszta systemu nie powinna mieć możliwości
bezpośrednio nadpisać stanu faktycznego. Ten wzorzec *polecenie/żądane vs. rzeczywiste* wraca później
przy `BeltMotor` (Moduł 4) i jest jednym z najważniejszych nawyków projektowych całego kursu.

`Plant::advance()` musi teraz **czekać**, aż dywerter się ustabilizuje (`isSettled()`), zanim paczka
faktycznie odjedzie — to pierwszy raz, kiedy czas (liczba ticków) staje się częścią logiki, nie tylko
licznikiem pętli.

---

## 3. Orkiestracja systemu

**Moduł 3 · `module-03-start` · misje 10–12**

Do tej pory `main()` w CLI robiło całą orkiestrację ręcznie. W tym module ta odpowiedzialność
przenosi się do jednego miejsca: `Engine`.

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

Kluczowe pojęcie: **kompozycja, nie dziedziczenie.** `Engine` *posiada* `Plant` i `Diverter` jako pola
— nie dziedziczy po nich, nie jest żadnym z nich. To rozróżnienie („ma” vs. „jest”) będzie wracać w
kursie wielokrotnie i jest jednym z częstszych błędnych skrótów myślowych: `Engine` **komponuje**
obiekty domeny i **wywołuje** logikę sterowania (funkcje z `Controller`), ale logika sterowania sama w
sobie zostaje jako zbiór wolnych funkcji, nie metod `Engine`.

`TickResult` to nowy, ważny wzorzec: zamiast wielu osobnych getterów, `Engine::step()` zwraca **jedną,
kompletną wartość** opisującą wszystko, co wydarzyło się w danym ticku. Testy i CLI patrzą tylko na tę
wartość — nigdy nie muszą pytać `Engine` o stan w środku ticku.

Ten moduł formalizuje też **kolejność operacji w ramach jednego kroku** — coś, co w Module 1 było
niejawne, tu staje się jawnym, przetestowanym kontraktem.

---

## 4. Aktuator i tryb pracy

**Moduł 4 · `module-04-start` · misje 13–15**

Linia produkcyjna nie może po prostu „działać” — musi mieć **tryb pracy**, a jej główny napęd musi się
rozpędzać i hamować, nie przełączać się natychmiast.

```cpp
enum class BeltMotorCommand { Stop, Run };
enum class BeltMotorState { Stopped, RampingUp, Running, RampingDown };
enum class Mode { Idle, Running, EStopped, Fault };
```

`BeltMotor` powtarza wzorzec command/actual z Modułu 2, ale z ważną nowością: przejście między stanami
zajmuje więcej niż jeden tick (`Stopped → RampingUp → Running`). To nie jest szczegół implementacyjny
bez znaczenia — w dalszych modułach (a także w projekcie końcowym) ten jeden tick opóźnienia ma
bezpośredni wpływ na to, kiedy dokładnie coś może ruszyć się po linii.

`Mode` to pierwsza maszyna stanów, która **bramkuje** (guards) zachowanie reszty systemu: ruch fizyczny
(`advance()`) wykonuje się tylko, gdy pas faktycznie jest `Running`. To wprowadza kluczowy nawyk
architektoniczny kursu: zamiast rozrzucać warunki `if (tryb == coś)` po całym kodzie, jeden, dobrze
przetestowany punkt decyzyjny (bramka) chroni jedną, konkretną operację.

Od tego modułu w systemie współpracuje już kilka niezależnych maszyn stanów naraz (`Mode`,
`DiverterPosition`, `BeltMotorState`) — każda ma swój własny zakres odpowiedzialności, żadna nie zna
szczegółów pozostałych.

---

## 5. Niezależna ścieżka E-Stop

**Moduł 5 · `module-05-start` · misje 16–19**

> **Ważny disclaimer:** to, co budujesz w tym module, to uproszczony, dydaktyczny model wzorca
> „niezależna ścieżka bezpieczeństwa” w kodzie. **To nie jest projekt rzeczywistego systemu
> safety-rated** i nie zastępuje kursu functional safety. Uczysz się tu wzorca inżynierskiego, nie
> normy bezpieczeństwa.

Główna lekcja: bezpieczeństwo nie powinno być „jeszcze jednym `if`-em” wplecionym w logikę biznesową
— powinno być **osobną, priorytetową ścieżką**, która potrafi nadpisać wszystko inne, niezależnie od
tego, co w danym momencie robi reszta systemu.

```cpp
enum class EStopLatchState { Released, Engaged, Armed };
```

`EStopLatchState` to *latch* (zatrzask) — stan, który **nie cofa się sam z siebie**. Naciśnięcie
przycisku wprowadza go w `Engaged`; zwolnienie przycisku samo w sobie nie wystarcza, żeby wrócić do
`Released` — potrzebny jest jeszcze jawny `Reset`, i to dopiero z pośredniego stanu `Armed`. To
świadomy wybór projektowy: **fresh start po recovery** ma być zawsze wynikiem jawnej, dwuetapowej
decyzji operatora, nigdy przypadkowego zbiegu okoliczności.

`Mode::EStopped` ma wyższy priorytet niż wszystko inne — dokładnie to znaczy „niezależna ścieżka”:
reszta systemu nie musi wiedzieć o E-Stopie, żeby E-Stop mógł nad nią zapanować.

---

## 6. Czujniki i jakość danych

**Moduł 6 · `module-06-start` · misje 20–24**

Do tej pory system zawsze „wiedział” dokładnie, co się dzieje. W realnym systemie czujnik może się
zepsuć, dać nieaktualny odczyt, albo w ogóle nie odpowiedzieć.

```cpp
enum class ReadingStatus { Ok, Missing, Stale };

struct PresenceReading { ReadingStatus status; bool occupied; };
struct WeightReading { ReadingStatus status; Grams grams; };
```

Kluczowe rozróżnienie tego modułu: **ground truth** (co naprawdę leży na wadze — coś, co w symulatorze
zawsze znamy, bo to my go budujemy) kontra **observation** (co czujnik faktycznie zgłosił, ze swoim
`ReadingStatus`). Logika sterowania nigdy nie powinna sięgać po ground truth bezpośrednio — działa
wyłącznie na obserwacjach, dokładnie tak, jak działałby prawdziwy sterownik z prawdziwymi czujnikami.

Gdy odczyt jest `Missing` albo `Stale`, system stosuje wzorzec **last-known-good**: trzyma ostatnią
zaufaną wartość zamiast zgadywać albo zamierać. To prowadzi do „klasyfikacji odpornej na awarie” —
decyzji, która degraduje się w sposób przewidywalny, zamiast psuć się w sposób przypadkowy.

Ten moduł wprowadza też pierwszą realną potrzebę **korelacji** — skąd wiadomo, którego `ItemId`
dotyczy dany odczyt, skoro odczyty i ruch paczek dzieją się asynchronicznie względem siebie w ramach
jednego ticku.

---

## 7. Usterki i zdarzenia systemowe

**Moduł 7 · `module-07-start` · misje 25–28**

Ten moduł uczy rozróżnienia, które wraca później w każdym module: **zewnętrzne wejście** kontra
**zdarzenie emergentne**.

```cpp
enum class DiverterFaultKind { Blocked };
enum class SystemEventKind { DiverterNotReady, RoutingDeadlineMissed };
```

`DiverterFaultKind::Blocked` to zewnętrzne wejście — coś, co *wstrzykujesz* do systemu z zewnątrz (np.
w teście albo w scenariuszu), żeby zasymulować awarię mechaniczną. `SystemEventKind` to coś zupełnie
innego: **zdarzenie, które system sam wywnioskował** z tego, co zaobserwował — np. że dywerter nie
osiągnął pozycji w rozsądnym czasie (`DiverterNotReady`), a jeśli to trwa zbyt długo,
`RoutingDeadlineMissed`.

To rozróżnienie ma znaczenie architektoniczne: zewnętrzne wejście jest czymś, co *dajesz* systemowi;
zdarzenie systemowe jest czymś, co system *produkuje* jako wynik własnej logiki. Mylenie tych dwóch
kategorii to częsty błąd projektowy — ten moduł uczy trzymać je rozdzielone w typach, nie tylko w
nazwach.

`Mode::Fault` reaguje na `RoutingDeadlineMissed` i blokuje dalszy ruch, dopóki operator nie zresetuje
systemu — podobny wzorzec recovery co w Module 5, ale wywołany zupełnie inną przyczyną.

---

## 8. Wiele paczek i niezmienniki

**Moduł 8 · `module-08-start` · misje 29–32**

Największy pojedynczy skok architektoniczny w Course Core: system musi obsłużyć **więcej niż jedną
paczkę jednocześnie**.

> **Ważne rozróżnienie:** „wiele paczek naraz” **nie oznacza programowania współbieżnego.** Cały
> rdzeń symulatora pozostaje w pełni sekwencyjny i deterministyczny — jeden wątek, jeden
> `Engine::step()` na raz. Zmienia się tylko to, ile danych mieści `Plant` jednocześnie, nie model
> wykonania.

`Plant` przestaje trzymać jeden `std::optional<Item>` i zamiast tego dostaje **cztery nazwane sloty**:

```cpp
struct Plant {
    std::optional<Item> infeed;
    std::optional<Item> presenceCheck;
    std::optional<Item> weighing;
    std::optional<Item> diverting;
};
```

To celowa decyzja: **fixed slots, nie kontener.** Każda fizyczna strefa linii mieści dokładnie jedną
paczkę naraz — to fizyczne ograniczenie, więc typ (`std::optional`, nie `std::vector`) je od razu
wymusza, zamiast tylko go opisywać w komentarzu.

`advance()` przetwarza przejścia w jednym, ustalonym kierunku: **downstream → upstream** (najpierw
strefa najbliżej wyjścia, na końcu Infeed) — dzięki temu paczka zwolniona z jednej strefy może od razu
skorzystać z miejsca w tym samym ticku („przesunięcie łańcuchowe”), a jednocześnie zachowany jest
kluczowy niezmiennik: **żadna paczka nie może poruszyć się dwa razy w tym samym ticku.** To nie jest
przypadek — to bezpośrednia konsekwencja tego, że każde przejście jest ewaluowane dokładnie raz, w
ustalonej kolejności.

`ItemId` staje się teraz kluczem korelacji per-paczka wszędzie tam, gdzie wcześniej wystarczał sam fakt
„czy coś tu jest” — ślad (`TickResult`) musi jednoznacznie mówić, *której* paczki dotyczy dana obserwacja
czy zdarzenie, nawet gdy na linii jest ich kilka naraz.

---

## 9. Scenariusze jako dane

**Moduł 9 · `module-09-start` · misje 33–35**

Do tej pory każdy eksperyment wymagał ręcznego wywoływania metod `Engine` krok po kroku. Ten moduł
wprowadza **deklaratywny** sposób opisywania całego eksperymentu jako jednej wartości danych:

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

Kluczowa własność: `Scenario` to **czysta warstwa dodana nad `Engine`**, nie nowy mechanizm sterowania
— `runScenario` wewnątrz po prostu wywołuje te same publiczne metody `Engine`, które znałeś/aś od
Modułu 3, w kolejności i momentach zapisanych w danych.

Przedziały czasowe usterek (`from`, `until`) są **half-open**: `[from, until)` — usterka jest aktywna
w ticku `from`, ale już nie w ticku `until`. To standardowy, przewidywalny sposób zapisu przedziałów w
całym kursie, nie przypadkowa konwencja tego jednego typu.

Ważne rozróżnienie dwóch rodzajów błędu: **błąd statyczny** (`isValidScenario` zwraca `false` —
scenariusz w ogóle się nie uruchamia, bo jego opis jest wewnętrznie sprzeczny) kontra **błąd
dynamiczny** (scenariusz jest poprawny statycznie, ale coś nie da się wykonać w trakcie — `runScenario`
zwraca `std::nullopt` już po rozpoczęciu). To rozróżnienie — *czy błąd jest widoczny bez uruchamiania
czegokolwiek, czy dopiero w trakcie* — wraca w projekcie końcowym.

`runScenario` buduje **świeży `Engine`** przy każdym wywołaniu — to gwarantuje **powtarzalność**: ten
sam poprawny `Scenario` uruchomiony dwa razy zawsze daje identyczny ślad.

---

## 10. Jak czytać i pisać test

Krótkie przygotowanie do Project Kickoff — pierwszego ćwiczenia, w którym sam/a piszesz test, nie
tylko go uruchamiasz.

Każdy test w tym kursie ma tę samą, prostą strukturę — nieformalny **arrange / act / assert**:

```cpp
// arrange — przygotuj stan
psm::WeightReading reading{psm::ReadingStatus::Ok, 750};

// act — wywołaj to, co testujesz
auto result = psm::decideClassification(reading);

// assert — sprawdź wynik
psmCheck(result == psm::WeightClass::Heavy, "750g przy poprawnym odczycie klasyfikuje się jako Heavy");
```

Test to **wykonywalny kontrakt** — zamiast opisywać słownie, jak funkcja ma się zachować, test to
*pokazuje*, w sposób, który można uruchomić i który albo przejdzie, albo nie. To dlatego kurs traktuje
zielony `ctest -L misja-N` jako ostateczne kryterium ukończenia misji: kontrakt jest wykonywalny, nie
tylko opisany.

Najważniejsze pytanie, jakie warto zadać sobie o własnym teście: **czy on faktycznie coś wykrywa, czy
tylko przechodzi?** Test, który przejdzie niezależnie od tego, czy kod jest poprawny, niczego nie
dowodzi. Prosty sposób, żeby to sprawdzić: świadomie zepsuj testowany kod i upewnij się, że Twój test
wtedy **faktycznie** się zaczerwieni. Dokładne ćwiczenie tego kroku znajdziesz w
`final_project/00_project_kickoff.md`.

To nie jest rozdział o frameworkach testowych — kurs celowo używa jednej, minimalnej funkcji
(`psmCheck`) przez cały czas, żeby uwaga została na *co* i *dlaczego* testujesz, nie na narzędziu.

---

## 11. Projekt końcowy — jak podejść do nowego wymagania

Final Project stawia Cię po raz pierwszy w sytuacji, w której **nikt nie napisał za Ciebie
architektury**. Dostajesz wymaganie domenowe (paczki mogą przybywać szybciej, niż linia może je
przyjąć) i wspólny kontrakt do spełnienia (`final_project/01_final_project_brief.md`) — resztę
projektujesz sam/a.

Ten rozdział **nie mówi, jak rozwiązać** zadanie projektu — to metodyka podejścia do nowego wymagania
w istniejącym, ugruntowanym systemie, przydatna dużo szerzej niż tylko tutaj:

1. **Zacznij od kontraktu domenowego, nie od kodu.** Zanim napiszesz linijkę C++, zapisz słownie, co
   ma być prawdą, zanim i po każdej operacji, którą dodajesz.
2. **Wypisz niezmienniki jawnie.** Które z nich są zupełnie nowe? Które istniejące niezmienniki
   systemu muszą pozostać prawdziwe bez zmian? Które trzeba świadomie rozszerzyć?
3. **Zdecyduj o własności (ownership) nowego stanu**, zanim zaczniesz go implementować. Kto go trzyma?
   Czy ma sens tylko w jednym trybie sterowania systemem, czy w każdym?
4. **Chroń niezmienniki przez enkapsulację**, nie przez konwencję. Jeśli coś musi zostać prawdziwe
   zawsze, powinno być niemożliwe do złamania przez publiczne API, nie tylko „niezalecane”.
5. **Zachowuj istniejące kontrakty, jeśli nie musisz ich świadomie zmienić.** Rozszerzanie systemu nie
   oznacza przepisywania go od nowa ani obchodzenia tego, co już działa równoległą implementacją.
6. **Napisz własne testy** — kategoriami zachowań, nie przypadkowymi przykładami. Sprawdź, czy każdy
   test faktycznie coś wykrywa (rozdział 10).
7. **Zbuduj `Scenario`**, który demonstruje działanie Twojego rozwiązania od początku do końca —
   dokładnie tak, jak `Scenario` demonstrowały gotowe zachowanie w Module 9.
8. **Uzasadnij swoje decyzje projektowe** — krótko, pisemnie. To będzie punktem wyjścia do obrony.

Pełne wymagania, kryteria akceptacji i lista decyzji pozostawionych Tobie:
[`final_project/01_final_project_brief.md`](https://github.com/michalmaj/mechatronic-systems-programming/blob/final-project-start/final_project/01_final_project_brief.md)
(dostępne od tagu `final-project-start`).

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
| zatrzask (E-Stop) | latch | `EStopLatchState` |
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

Skrócona mapa najważniejszych typów Core (nie pełna dokumentacja API — po szczegóły sięgaj do
nagłówków w `include/psm/` i materiału danego modułu):

| Typ | Gdzie | Rola |
|---|---|---|
| `Plant` | `plant.hpp` | fizyczny stan linii — sloty na paczki |
| `Item` / `ItemId` | `item.hpp` | paczka i jej tożsamość |
| `Diverter` | `diverter.hpp` | aktuator kierujący paczki na wyjście |
| `BeltMotor` | `belt_motor.hpp` | napęd taśmy, z rampowaniem |
| `Mode` | `mode.hpp` | tryb pracy całego systemu |
| `EStopLatchState` | `estop_latch.hpp` | zatrzask bezpieczeństwa |
| `PresenceSensor` / `WeightSensor` | `presence_sensor.hpp`, `weight_sensor.hpp` | czujniki z niepewnością odczytu |
| `Engine` | `engine.hpp` | orkiestrator jednego ticku, publiczne API systemu |
| `TickResult` | `tick_result.hpp` | pełny, jednoznaczny wynik jednego ticku |
| `Scenario` / `runScenario` | `scenario.hpp` | deklaratywny opis i odtwarzanie eksperymentu |
