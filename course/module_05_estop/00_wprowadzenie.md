# 5.0 Wprowadzenie

Do tej pory bezpieczeństwo systemu opierało się wyłącznie na tym, żeby `Mode`, `Controller` i reszta
logiki działały poprawnie. Ten moduł dodaje coś fundamentalnie innego: przycisk awaryjny jako
**drugą, niezależną** ścieżkę bezpieczeństwa — niezależną od Controllera, niezależną od tego, czy
reszta logiki routingu w ogóle działa poprawnie.

**Ważne zastrzeżenie na start:** przycisk awaryjny, który tu zbudujesz, to uproszczony model
dydaktyczny zachowania systemu — uczy kształtu problemu (druga, niezależna ścieżka; bezwzględny
priorytet; brak cichego automatycznego wznowienia), na małej, czytelnej maszynie stanów. To **nie**
jest projekt spełniający normy bezpieczeństwa dla prawdziwej maszyny. Prawdziwe obwody bezpieczeństwa
wymagają certyfikowanych komponentów, sprzętowych blokad i norm, których ten kurs nie porusza.

## Skąd startujesz

```bash
git fetch --tags
git switch -c <nazwa-twojego-brancha> module-05-start
```

Tak jak w poprzednim module: **`Engine::step()` zostaje dokładnie taki, jaki zostawił go Moduł 4, aż
do ostatniej misji.** Najpierw budujesz przycisk awaryjny, rozszerzony `Mode` i funkcje bezpieczeństwa
jako samodzielne, osobno testowalne elementy — dopiero ostatnia misja spina je z działającym
`Engine`.

## Mapa modułu

Ten moduł ma **cztery** misje, nie trzy jak poprzednie — temat na to zasługuje.

1. **Przycisk awaryjny** — `EStopLatchState`, trzecia maszyna stanów w kursie (po `Mode`).
2. **Tryb zatrzymania awaryjnego** — `Mode` uczy się o przycisku i zyskuje `EStopped`.
3. **Dwie niezależne ścieżki** — samodzielne funkcje decyzyjne plus jedna celowo mutująca operacja
   awaryjna.
4. **Silnik pod ochroną** — spięcie wszystkiego w `Engine::step()`, naprawdę niezależnie.

## Zanim zaczniesz

- Testy Modułów 1–4 (`misja-1`–`misja-4`, `misja-6`–`misja-15`) są już obecne i przechodzą.
- Tak jak zawsze: nie edytujesz plików testowych ani `CMakeLists.txt`.

**Dalej:** [Misja 16: przycisk awaryjny](./01_przycisk_awaryjny.md).
