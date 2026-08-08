# 3.0 Wprowadzenie

Moduł 2 (misja 9) wprowadził ustaloną kolejność jednego ticka: decyzja Controllera → `setCommand` →
`resolve` → `advance` → obserwacja. Ale ta kolejność żyła wyłącznie z konwencji — była ręcznie
powtórzona i w `runTicks`, i w `main()`. Nic nie gwarantowało, że te dwie kopie się nie rozjadą.

Ten moduł kończy tę sprawę: nowa klasa `Engine` przejmuje na własność `Plant` i `Diverter` i staje
się **jedynym** miejscem, w którym ta kolejność w ogóle istnieje w kodzie.

## Skąd startujesz

```bash
git fetch --tags
git switch -c <nazwa-twojego-brancha> module-03-start
```

Ważna różnica względem poprzednich modułów: **`runTicks` i dotychczasowy `main()` wciąż tu są i wciąż
działają** — dokładnie tak, jak zostawił je Moduł 2. Nic nie zostało jeszcze przepięte. Zobaczysz obok
siebie: starą, ręczną orkiestrację (`runTicks`) i nowy, pusty jeszcze `Engine`, który ma ją zastąpić.
To celowe — zanim usuniesz duplikację, warto ją najpierw zobaczyć na własne oczy.

Dopiero ostatnia misja tego modułu przepina `main()` na `Engine`.

## Mapa modułu

1. **Tick i wynik** — jeden, niezmienny opis tego, co wydarzyło się w danym ticku.
2. **Silnik formalizuje kolejność** — druga klasa w kursie: `Engine`, który *posiada* `Plant` i
   `Diverter`.
3. **Przepięcie na silnik** — moment, w którym `main()` zaczyna faktycznie korzystać z `Engine`.

## Zanim zaczniesz

- Testy Modułu 1 i 2 (`misja-1`–`misja-4`, `misja-6`–`misja-9`) są już obecne i przechodzą —
  odziedziczona, gotowa podstawa.
- Ostatnia misja tego modułu (Misja 12) nie ma osobnego automatycznego testu — jej prawdziwym
  self-checkiem jest zbudowanie i uruchomienie programu, dokładnie tak jak w Misji 6 Modułu 1.

**Dalej:** [Misja 10: tick i wynik](./01_tick_i_wynik.md).
