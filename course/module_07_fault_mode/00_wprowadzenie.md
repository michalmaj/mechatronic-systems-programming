🇵🇱 Polski | [🇬🇧 English](00_wprowadzenie.en.md)

# 7.0 Wprowadzenie

`Mode::EStopped` od Modułu 5 ma jasny powód istnienia: przycisk awaryjny. `Mode::Fault` istnieje od
Modułu 4 — ale bez żadnego wyzwalacza. Ten moduł to zmienia: dywerter, który nie potrafi dotrzymać
terminu rutowania (bo jest fizycznie zablokowany), w końcu daje `Fault` konkretny powód, żeby się
pojawić.

`Diverter::isSettled()` — sprawdzenie, które pozwala odróżnić dywerter naprawdę zablokowany od
takiego, który po prostu jeszcze nie dostał polecenia ruchu — istnieje bez zmian od Modułu 2. Ten
moduł go nie przebudowuje; wykorzystuje go do czegoś nowego.

Największa mechaniczna nowość tego modułu: `Mode` jest teraz liczony w **dwóch jawnie osobnych
krokach**, nie jednym. Wcześniej `modeStep` odpowiadał na jedno pytanie: "co mówią wejścia
operatora/e-stopu/resetu?" Teraz to pytanie zostaje osobne od drugiego: "co właśnie zgłosił
`Plant::advance()`?" — bo te dwa pytania mają odpowiedzi dostępne w dwóch różnych momentach tego
samego ticku. Misja `tryb_awarii` tłumaczy dokładnie dlaczego to musiały być dwie funkcje, nie jedna
wywoływana dwukrotnie.

## Skąd startujesz

```bash
git fetch --tags
git switch -c <nazwa-twojego-brancha> module-07-start
```

Jak zawsze: `Engine::step()` zostaje dokładnie taki, jaki zostawił go Moduł 6, aż do ostatniej misji.

## Mapa modułu

1. **Zablokowany dywerter** — `Diverter::resolve` uczy się reagować na usterkę aktuatora, tym samym
   wzorcem co czujniki w Module 6, ale zastosowanym do nowego, osobnego typu (`DiverterFaultKind`) —
   nie do wspólnego, przeciążonego typu, który dopuszczałby bezsensowne kombinacje w rodzaju
   "zablokowany czujnik obecności".
2. **Termin rutowania** — `Plant` liczy, jak długo dywerter nie nadąża, i zgłasza to jako
   `SystemEventKind`. Największa porcja teorii w tym module: dlaczego termin musi mierzyć wyłącznie
   *ciągłą, aktywną* próbę rutowania.
3. **Tryb awarii** — `Mode` uczy się `Fault`, w dwóch osobnych krokach: `modeStep` (wejścia
   operatora) i nowa `reactToSystemEvent` (reakcja na zdarzenie z `Plant`). `Fault` jest zatrzaskowe:
   ani `stopRequested`, ani `startRequested` nie mogą z niego wyjść — tylko jawny reset.
4. **Silnik z wykrywaniem awarii** — spięcie wszystkiego: rozdzielone API usterek
   (`injectSensorFault`/`injectDiverterFault`), `step()` używający `modeForTick` do bramkowania
   aktuatorów i `reactToSystemEvent` do ustalenia ostatecznego `mode_`, oraz pełny scenariusz
   odzyskiwania w CLI.

## Zanim zaczniesz

- Testy Modułów 1–6 (`misja-1`–`misja-4`, `misja-6`–`misja-24`) są już obecne i przechodzą.
- Tak jak zawsze: nie edytujesz plików testowych ani `CMakeLists.txt`.

**Dalej:** [Misja 25: zablokowany dywerter](./01_zablokowany_dywerter.md).
