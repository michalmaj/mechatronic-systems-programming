🇵🇱 Polski | [🇬🇧 English](00_wprowadzenie.en.md)

# 7.0 Wprowadzenie

W module 5 tryb `Mode::EStopped` został powiązany z przyciskiem awaryjnym. Teraz podobnie zajmiemy
się trybem `Mode::Fault`. Program przejdzie do niego, gdy zablokowany dywerter nie ustawi się w
wyznaczonym czasie.

Do wykrycia takiej sytuacji wykorzystasz `Diverter::isSettled()`. Ta metoda istnieje od modułu 2 i
sprawdza, czy dywerter osiągnął położenie wynikające z ostatniego polecenia. Nie będziesz jej
zmieniać, lecz użyjesz jej w nowym miejscu.

Zmieni się za to sposób wyznaczania trybu pracy. Od tej pory będzie się to odbywać w dwóch krokach.
Na początku ticku program uwzględni polecenia operatora, stan przycisku awaryjnego i żądanie resetu.
Po wywołaniu `Plant::advance()` zareaguje na zdarzenie zgłoszone przez symulowany układ. Te
informacje są dostępne w różnych momentach, dlatego każda z nich będzie obsługiwana przez osobną
funkcję.

## Skąd startujesz

```bash
git fetch --tags
git switch -c <nazwa-twojego-brancha> module-07-start
```

Do ostatniej misji pozostaw `Engine::step()` w wersji z modułu 6.

## Mapa modułu

1. **Zablokowany dywerter**: `Diverter::resolve()` zacznie uwzględniać usterkę elementu wykonawczego.
   Zdefiniujesz ją za pomocą osobnego typu `DiverterFaultKind`, aby nie można było pomylić jej z
   usterką czujnika.
2. **Limit czasu na ustawienie dywertera**: `Plant` policzy, jak długo dywerter nie może osiągnąć
   wymaganego położenia, a następnie zgłosi odpowiedni `SystemEventKind`. Dowiesz się też, dlaczego
   licznik powinien obejmować tylko kolejne aktywne próby wyboru trasy.
3. **Tryb awarii**: obsługę `Mode` podzielisz między `modeStep()`, które przetwarza wejścia
   operatora, a `reactToSystemEvent()`, które reaguje na zdarzenie z `Plant`. Tryb `Fault` pozostanie
   aktywny aż do jawnego resetu.
4. **`Engine` z wykrywaniem awarii**: połączysz wszystkie elementy w `Engine::step()`, dodasz osobne
   metody do obsługi usterek czujników i dywertera oraz przygotujesz w programie terminalowym pełny
   scenariusz usunięcia awarii i wznowienia pracy.

## Zanim zaczniesz

- Testy modułów 1–6 (`misja-1`–`misja-4`, `misja-6`–`misja-24`) są już obecne i przechodzą.
- Tak jak wcześniej, nie edytuj plików testowych ani `CMakeLists.txt`.

**Dalej:** [Misja 25: zablokowany dywerter](./01_zablokowany_dywerter.md).
