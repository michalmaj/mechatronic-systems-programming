🇵🇱 Polski | [🇬🇧 English](handbook.en.md)

# Podręcznik kursu

← [README](../README.md) · [Plan pracy](roadmap.md)

Spójny materiał do rdzenia kursu (moduły 0–9) i wstęp do projektu końcowego. Źródłem prawdy dla
każdego rozdziału jest aktualny kod i testy odpowiadającego tagu (`module-XX-start` /
`module-XX-solution`) — jeśli coś tu wygląda inaczej niż kod, którym operujesz, kod wygrywa. Zgłoś
to przez Issues.

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

Ten kurs uczy przez trzy powtarzające się kroki: kod, eksperyment, test. Piszesz kawałek
implementacji, uruchamiasz symulator (przez CLI albo test), patrzysz na wynik i dopiero wtedy
sprawdzasz go dostarczonym testem — nie odwrotnie. Test ma potwierdzić to, co już zaobserwowałeś, a
nie podpowiedzieć, co ma robić kod.

Kilka rzeczy jest prawdziwych przez cały kurs i warto je zrozumieć raz, na początku:

- Symulacja działa w dyskretnych, deterministycznych krokach, tickach. Każde wywołanie
  `Engine::step()` to jeden tick: przeczytaj bieżący stan, zdecyduj, co się zmienia, zwróć wynik
  (`TickResult`). Ten sam ciąg wejść zawsze daje ten sam ciąg wyników.
- Testy dostarcza kurs. Nie piszesz ich sam aż do ćwiczenia przed projektem końcowym (rozdział 10) —
  masz je zrozumieć i uruchomić, nie zaprojektować od zera. Test jest wykonywalnym kontraktem misji:
  zielony wynik potwierdza wymagane zachowanie i kończy misję, ale sam w sobie nie dowodzi
  zrozumienia. Tym zajmują się checkpointy po Modułach 3, 7 i 9.
- CMake i CTest to narzędzia, nie przedmiot nauki. Konfigurujesz preset, budujesz, uruchamiasz testy
  — to wszystko, co musisz wiedzieć o samym CMake w tym kursie (Dodatek B).
- Pracujesz na tagach. Każdy moduł zaczynasz od `git switch -c <twoja-gałąź> module-XX-start`.
  Commitujesz na własnej gałęzi; tagi kursu zostają nietknięte jako punkt odniesienia.

---

## 1. Modelowanie prostego procesu

**Moduł 1 · `module-01-start` · misje 1–6**

Zaczynasz od najmniejszej możliwej wersji problemu: jedna paczka, jedna strefa na raz. Paczka
wjeżdża, zostaje sklasyfikowana i „przesuwa się” po strefach — na razie bez aktuatora, bez czasu
reakcji, bez wielu paczek naraz.

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

To najprostszy możliwy kształt — jeden `std::optional<Item>`, żadnego kontenera. Poznajesz tu
podstawowy słownik C++, którego kurs używa dalej wszędzie: `enum class` na zamknięty zbiór stanów,
`struct` na proste dane, `std::optional` na „może nie ma wartości” zamiast wskaźnika czy wartowniczej
stałej, oraz wolne funkcje (`spawnItem`, `advance`, `classify`) jako podstawową jednostkę logiki —
jeszcze bez klas.

Pierwsza pętla sterowania w CLI, w uproszczonej formie, pokazuje już cały rytm kursu: ustal polecenie
na podstawie stanu, wykonaj krok, wypisz wynik. Ten rytm — decyzja, krok, obserwacja — wraca w każdym
kolejnym module, tylko z coraz bogatszym stanem.

---

## 2. Polecenie to nie stan fizyczny

**Moduł 2 · `module-02-start` · misje 7–9**

Lekcja tego modułu: wysłanie polecenia do urządzenia nie znaczy, że urządzenie już je wykonało.
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

To Twoja pierwsza własna klasa w kursie. Enkapsulacja przestaje być tu abstrakcyjną zasadą, a staje
się konkretnym narzędziem: `Diverter` chroni różnicę między tym, co poleciłeś (`command`), a tym, co
się faktycznie dzieje (`actualPosition`) — reszta systemu nie ma jak bezpośrednio nadpisać stanu
faktycznego. Ten wzorzec, polecenie kontra rzeczywistość, wraca później przy `BeltMotor` (Moduł 4) i
jest jednym z najważniejszych nawyków projektowych całego kursu.

`Plant::advance()` musi teraz poczekać, aż dywerter się ustabilizuje (`isSettled()`), zanim paczka
faktycznie odjedzie. To pierwszy moment, w którym czas — liczba ticków — staje się częścią logiki, a
nie tylko licznikiem pętli.

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

`Engine` *ma* `Plant` i `Diverter` jako pola — nie dziedziczy po nich i nie jest żadnym z nich. To
rozróżnienie, „ma” kontra „jest”, wraca w kursie wielokrotnie; mylenie kompozycji z dziedziczeniem to
jeden z częstszych błędnych skrótów myślowych. `Engine` składa obiekty domeny w całość i wywołuje
logikę sterowania (funkcje z `Controller`), ale sama logika zostaje zbiorem wolnych funkcji, nie
metod `Engine`.

`TickResult` wprowadza kolejny wzorzec: zamiast wielu osobnych getterów, `Engine::step()` zwraca
jedną, kompletną wartość opisującą wszystko, co wydarzyło się w danym ticku. Testy i CLI patrzą tylko
na tę wartość — nigdy nie muszą pytać `Engine` o stan w środku ticku.

Ten moduł formalizuje też kolejność operacji w ramach jednego kroku — w Module 1 była ona niejawna,
tu staje się jawnym, przetestowanym kontraktem.

---

## 4. Aktuator i tryb pracy

**Moduł 4 · `module-04-start` · misje 13–15**

Linia produkcyjna nie może po prostu „działać” — musi mieć tryb pracy, a jej główny napęd musi się
rozpędzać i hamować, a nie przełączać się natychmiast.

```cpp
enum class BeltMotorCommand { Stop, Run };
enum class BeltMotorState { Stopped, RampingUp, Running, RampingDown };
enum class Mode { Idle, Running };
```

(`Mode` rośnie w kolejnych modułach — `EStopped` dochodzi w Module 5, `Fault` w Module 7. Każdy stan
pojawia się dopiero razem z mechanizmem, który go potrzebuje.)

`BeltMotor` powtarza wzorzec polecenie/stan rzeczywisty z Modułu 2, z jedną istotną nowością:
przejście między stanami zajmuje więcej niż jeden tick (`Stopped → RampingUp → Running`). To ma
realne konsekwencje w dalszych modułach i w projekcie końcowym — ten jeden tick opóźnienia decyduje o
tym, kiedy dokładnie coś może ruszyć się po linii.

`Mode` to pierwsza maszyna stanów, która bramkuje zachowanie reszty systemu: ruch fizyczny
(`advance()`) wykonuje się tylko wtedy, gdy pas faktycznie jest `Running`. Stąd nawyk, który wraca w
całym kursie — zamiast rozrzucać warunki `if (tryb == coś)` po całym kodzie, jeden, dobrze
przetestowany punkt decyzyjny (bramka) pilnuje jednej, konkretnej operacji.

Od tego modułu współpracuje już kilka niezależnych maszyn stanów naraz (`Mode`, `DiverterPosition`,
`BeltMotorState`). Każda ma własny zakres odpowiedzialności i żadna nie zna szczegółów pozostałych.

---

## 5. Niezależna ścieżka awaryjnego stopu

**Moduł 5 · `module-05-start` · misje 16–19**

> To, co budujesz w tym module, to uproszczony, dydaktyczny model wzorca „niezależna ścieżka
> bezpieczeństwa” w kodzie. Nie jest to projekt rzeczywistego systemu safety-rated i nie zastępuje
> kursu functional safety — uczysz się tu wzorca inżynierskiego, nie normy bezpieczeństwa.

Bezpieczeństwo nie powinno być kolejnym `if`-em wplecionym w logikę biznesową — powinno być osobną,
priorytetową ścieżką, zdolną nadpisać wszystko inne, niezależnie od tego, co akurat robi reszta
systemu.

```cpp
enum class EStopLatchState { Released, Engaged, Armed };
```

`EStopLatchState` to zatrzask — stan, który nie cofa się sam z siebie. Naciśnięcie przycisku
wprowadza go w `Engaged`; samo zwolnienie przycisku nie wystarcza, żeby wrócić do `Released` —
potrzebny jest jeszcze jawny `Reset`, i to dopiero z pośredniego stanu `Armed`.

To załatwia sam zatrzask, ale nie ponowne uruchomienie linii. Zwolnienie i reset odblokowują system i
sprowadzają `Mode` z powrotem do `Idle` — to jeszcze nie `Running`. Żeby linia faktycznie ruszyła,
potrzeba osobnego, kolejnego `StartRequested`, na późniejszym ticku. `Reset` i `StartRequested`
wysłane na tym samym ticku nie restartują systemu od razu: `modeStep` w tym ticku i tak sprowadza
`Mode` tylko do `Idle`, a `Running` wymaga oddzielnego wywołania startu, gdy `Mode` jest już `Idle`.
To świadoma decyzja projektowa — ponowny ruch linii ma być zawsze wynikiem osobnej, jawnej decyzji
operatora, nigdy efektem ubocznym samego odblokowania.

`Mode::EStopped` ma wyższy priorytet niż wszystko inne. Dokładnie to znaczy niezależna ścieżka:
reszta systemu nie musi wiedzieć o awaryjnym stopie, żeby ten mógł nad nią zapanować.

---

## 6. Czujniki i jakość danych

**Moduł 6 · `module-06-start` · misje 20–24**

Do tej pory system zawsze „wiedział” dokładnie, co się dzieje. W realnym systemie czujnik może się
zepsuć, dać nieaktualny odczyt albo w ogóle nie odpowiedzieć.

```cpp
enum class ReadingStatus { Ok, Missing, Stale };

struct PresenceReading { ReadingStatus status; bool occupied; };
struct WeightReading { ReadingStatus status; Grams grams; };
```

Rozróżnienie tego modułu: to, co naprawdę leży na wadze — a w symulatorze zawsze to wiemy, bo sami go
budujemy — kontra to, co czujnik faktycznie zgłosił, wraz ze swoim `ReadingStatus`. Logika sterowania
nigdy nie powinna sięgać po rzeczywisty stan bezpośrednio; działa wyłącznie na tym, co zgłaszają
czujniki, dokładnie tak, jak działałby prawdziwy sterownik z prawdziwymi czujnikami.

Gdy odczyt jest `Stale`, czujnik podstawia ostatnią zaufaną wartość — ale tylko wtedy, gdy taka
wartość w ogóle już istnieje; jeśli nie, `Stale` degraduje się do tego samego zachowania co
`Missing`. `Missing` z kolei nigdy nie korzysta z poprzedniej wartości — to jawny sygnał „nic nie
wiadomo”, inny niż „wiadomo, ale nieaktualnie”. Ten sam mechanizm działa identycznie dla odczytu
obecności i wagi. Stąd klasyfikacja odporna na awarie: decyzja, która degraduje się przewidywalnie,
zamiast psuć się przypadkowo.

Klasyfikacja korzysta z tych odczytów, ale musi też pamiętać coś własnego między tickami:
potwierdzenie obecności (`PresenceCheck`) i odczyt wagi (`Weighing`) to dwie osobne, sekwencyjne
strefy — paczka mija je jedna po drugiej, na różnych tickach. Żeby zaufać wadze później, system musi
pamiętać, że obecność była już wcześniej potwierdzona. Stąd `ControllerState` — mały rekord
(`presenceConfirmed`, `classification`) trzymany między tickami dla aktualnie przetwarzanej paczki,
aktualizowany co tick w miarę tego, jak paczka przechodzi przez kolejne strefy.

---

## 7. Usterki i zdarzenia systemowe

**Moduł 7 · `module-07-start` · misje 25–28**

Ten moduł uczy rozróżnienia, które wraca później w każdym module: zewnętrzne wejście kontra
zdarzenie, które system sam wywnioskował.

```cpp
enum class DiverterFaultKind { Blocked };
enum class SystemEventKind { DiverterNotReady, RoutingDeadlineMissed };
```

`DiverterFaultKind::Blocked` to zewnętrzne wejście — coś, co wstrzykujesz do systemu z zewnątrz, na
przykład w teście albo w scenariuszu, żeby zasymulować awarię mechaniczną. `SystemEventKind` to coś
innego: zdarzenie, które system sam wywnioskował z tego, co zaobserwował. Na przykład, że dywerter
nie osiągnął pozycji w rozsądnym czasie (`DiverterNotReady`), a jeśli trwa to zbyt długo,
`RoutingDeadlineMissed`.

To rozróżnienie ma znaczenie architektoniczne: zewnętrzne wejście dajesz systemowi, zdarzenie
systemowe system produkuje sam, jako wynik własnej logiki. Mylenie tych dwóch kategorii to częsty
błąd projektowy — ten moduł uczy trzymać je rozdzielone w typach, nie tylko w nazwach.

`Mode::Fault` reaguje na `RoutingDeadlineMissed` i blokuje dalszy ruch, dopóki operator nie
zresetuje systemu. Podobny wzorzec wznowienia pracy co w Module 5, ale wywołany zupełnie inną
przyczyną.

---

## 8. Wiele paczek i niezmienniki

**Moduł 8 · `module-08-start` · misje 29–32**

Największy pojedynczy skok architektoniczny w rdzeniu kursu: system musi obsłużyć więcej niż jedną
paczkę naraz.

> Wiele paczek naraz nie oznacza programowania współbieżnego. Rdzeń symulatora zostaje w pełni
> sekwencyjny i deterministyczny — jeden wątek, jeden `Engine::step()` na raz. Zmienia się tylko to,
> ile danych mieści `Plant` jednocześnie, nie model wykonania.

`Plant` przestaje trzymać jeden `std::optional<Item>` i dostaje cztery nazwane sloty:

```cpp
struct Plant {
    std::optional<Item> infeed;
    std::optional<Item> presenceCheck;
    std::optional<Item> weighing;
    std::optional<Item> diverting;
};
```

To celowa decyzja — nazwane sloty, nie kontener. Każda fizyczna strefa linii mieści dokładnie jedną
paczkę naraz. To ograniczenie fizyczne, więc typ (`std::optional`, nie `std::vector`) je od razu
wymusza, zamiast tylko opisywać w komentarzu.

`advance()` przetwarza przejścia w jednym, ustalonym kierunku: od wyjścia do wejścia — najpierw
strefa najbliżej wyjścia, na końcu Infeed. Dzięki temu paczka, która zwalnia jedną strefę, może od
razu skorzystać z miejsca w tym samym ticku (przesunięcie łańcuchowe), a jednocześnie zachowany jest
niezmiennik: żadna paczka nie może poruszyć się dwa razy w tym samym ticku. To nie przypadek — to
bezpośrednia konsekwencja tego, że każde przejście jest ewaluowane dokładnie raz, w ustalonej
kolejności.

`ItemId` staje się teraz kluczem korelacji dla każdej paczki z osobna, wszędzie tam, gdzie wcześniej
wystarczał sam fakt „czy coś tu jest”. Ślad (`TickResult`) musi jednoznacznie mówić, której paczki
dotyczy dana obserwacja czy zdarzenie, nawet gdy na linii jest ich kilka naraz.

---

## 9. Scenariusze jako dane

**Moduł 9 · `module-09-start` · misje 33–35**

Do tej pory każdy eksperyment wymagał ręcznego wywoływania metod `Engine` krok po kroku. Ten moduł
wprowadza deklaratywny sposób opisywania całego eksperymentu jako jednej wartości danych:

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

`Scenario` to czysta warstwa dodana nad `Engine`, nie nowy mechanizm sterowania — `runScenario`
wewnątrz po prostu wywołuje te same publiczne metody `Engine`, które znasz od Modułu 3, w kolejności
i momentach zapisanych w danych.

Przedziały czasowe usterek (`from`, `until`) są otwarte z jednej strony: `[from, until)` — usterka
jest aktywna w ticku `from`, ale już nie w ticku `until`. To standardowy, przewidywalny sposób
zapisu przedziałów w całym kursie, nie przypadkowa konwencja jednego typu.

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

Test to wykonywalny kontrakt. Zamiast opisywać słownie, jak funkcja ma się zachować, test to
pokazuje, w sposób, który można uruchomić i który albo przejdzie, albo nie. Dlatego kurs traktuje
zielony `ctest -L misja-N` jako ostateczne kryterium ukończenia misji: kontrakt jest wykonywalny, nie
tylko opisany.

Najważniejsze pytanie, jakie warto zadać sobie o własnym teście: czy on faktycznie coś wykrywa, czy
tylko przechodzi? Test, który przejdzie niezależnie od tego, czy kod jest poprawny, niczego nie
dowodzi. Prosty sposób, żeby to sprawdzić: świadomie zepsuj testowany kod i sprawdź, czy Twój test
naprawdę się wtedy zaczerwieni. Dokładne ćwiczenie tego kroku znajdziesz w
[`final_project/00_project_kickoff.md`](../final_project/00_project_kickoff.md).

To nie jest rozdział o frameworkach testowych — kurs celowo używa jednej, minimalnej funkcji
(`psmCheck`) przez cały czas, żeby uwaga została przy tym, co i dlaczego testujesz, a nie przy
narzędziu.

---

## 11. Projekt końcowy — jak podejść do nowego wymagania

Projekt końcowy stawia Cię po raz pierwszy w sytuacji, w której nikt nie napisał za Ciebie
architektury. Dostajesz wymaganie domenowe — paczki mogą przybywać szybciej, niż linia jest w stanie
je przyjąć — i wspólny kontrakt do spełnienia
([`final_project/01_final_project_brief.md`](../final_project/01_final_project_brief.md)). Resztę
projektujesz sam.

Ten rozdział nie mówi, jak rozwiązać zadanie projektu. To metodyka podejścia do nowego wymagania w
istniejącym, ugruntowanym systemie, przydatna dużo szerzej niż tylko tutaj:

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
   tak, jak `Scenario` demonstrowały gotowe zachowanie w Module 9.
8. Uzasadnij swoje decyzje projektowe — krótko, pisemnie. To będzie punktem wyjścia do obrony.

Pełne wymagania, kryteria akceptacji i lista decyzji pozostawionych Tobie:
[`final_project/01_final_project_brief.md`](../final_project/01_final_project_brief.md). Materiał
czytasz na `main`; kod startowy — z tagu `final-project-start-v2`.

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
