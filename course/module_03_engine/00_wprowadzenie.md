🇵🇱 Polski | [🇬🇧 English](00_wprowadzenie.en.md)

# 3.0 Wprowadzenie

W misji 9 ustaliliśmy kolejność operacji w jednym ticku: decyzja sterownika → `setCommand` →
`resolve` → `advance` → odczyt wyniku. Tę samą sekwencję zapisaliśmy osobno w `runTicks` i w
`main()`. Każdą późniejszą zmianę trzeba byłoby więc wprowadzać w dwóch miejscach.

Nowa klasa `Engine` będzie zawierać `Plant` i `Diverter`. Stanie się też jedynym miejscem, które
określa kolejność operacji w ticku.

## Skąd startujesz

```bash
git fetch --tags
git switch -c <nazwa-twojego-brancha> module-03-start
```

`runTicks` oraz dotychczasowy `main()` nadal działają tak jak po module 2. W projekcie znajduje się
też pusty szkielet klasy `Engine`, która ma przejąć ich zadania. Zanim usuniesz powtórzony kod,
możesz porównać oba rozwiązania.

Dopiero w ostatniej misji tego modułu zmienisz `main()` tak, aby korzystał z `Engine`.

## Mapa modułu

1. **Tick i wynik:** jeden, spójny opis tego, co wydarzyło się w danym ticku.
2. **Silnik porządkuje kolejność:** druga klasa w kursie, czyli `Engine`, który *zawiera* `Plant` i
   `Diverter`.
3. **Przejście na silnik:** zmiana `main()` tak, aby korzystał z `Engine`.

## Zanim zaczniesz

- Testy modułów 1 i 2 (`misja-1`–`misja-4`, `misja-6`–`misja-9`) są już obecne i powinny
  przechodzić od początku pracy nad tym modułem.
- Ostatnia misja tego modułu (misja 12) nie ma osobnego testu automatycznego. Sprawdzisz ją,
  budując i uruchamiając program, tak jak w misji 6.

**Dalej:** [Misja 10: tick i wynik](./01_tick_i_wynik.md).
