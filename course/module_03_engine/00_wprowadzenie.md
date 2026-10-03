🇵🇱 Polski | [🇬🇧 English](00_wprowadzenie.en.md)

# 3.0 Wprowadzenie

W misji 9 ustaliliśmy kolejność jednego ticka: decyzja Controllera → `setCommand` → `resolve` →
`advance` → obserwacja. Tę samą sekwencję zapisaliśmy osobno w `runTicks` i w `main()`, więc każda
późniejsza zmiana musiałaby trafić w dwa miejsca.

Nowa klasa `Engine` przejmie `Plant` i `Diverter` oraz skupi kolejność operacji w jednym miejscu.

## Skąd startujesz

```bash
git fetch --tags
git switch -c <nazwa-twojego-brancha> module-03-start
```

`runTicks` oraz dotychczasowy `main()` nadal działają tak jak po module 2. Obok nich znajduje się
pusty `Engine`, który ma je zastąpić. Dzięki temu przed usunięciem duplikacji możesz porównać oba
rozwiązania.

Dopiero ostatnia misja tego modułu przepina `main()` na `Engine`.

## Mapa modułu

1. **Tick i wynik** — jeden, niezmienny opis tego, co wydarzyło się w danym ticku.
2. **Silnik formalizuje kolejność** — druga klasa w kursie: `Engine`, który *posiada* `Plant` i
   `Diverter`.
3. **Przepięcie na silnik** — moment, w którym `main()` zaczyna faktycznie korzystać z `Engine`.

## Zanim zaczniesz

- Testy modułów 1 i 2 (`misja-1`–`misja-4`, `misja-6`–`misja-9`) są już obecne i przechodzą —
  odziedziczona, gotowa podstawa.
- Ostatnia misja tego modułu (Misja 12) nie ma osobnego automatycznego testu — jej prawdziwym
  sprawdzianem jest zbudowanie i uruchomienie programu, tak jak w misji 6.

**Dalej:** [Misja 10: tick i wynik](./01_tick_i_wynik.md).
