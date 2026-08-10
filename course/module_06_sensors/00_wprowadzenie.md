# 6.0 Wprowadzenie

Do tej pory `Controller` bezwarunkowo ufał `Item::mass` — masa paczki była po prostu prawdą,
zawsze dostępną, zawsze poprawną. Ten moduł to zmienia: paczka jest teraz ważona przez prawdziwy
czujnik, który może się zepsuć, i wykrywana przez czujnik obecności, który też może się zepsuć.

Ważna różnica względem aktuatorów z wcześniejszych modułów: czujnik w tym module **nie jest
wszechwiedzący**. Jest fizycznie zamontowany w jednym konkretnym miejscu przenośnika — czujnik
obecności widzi paczkę wyłącznie w strefie `PresenceCheck`, czujnik wagi wyłącznie w `Weighing`.
To nie jest szczegół implementacyjny — to jest właśnie ten problem, który ten moduł uczy rozwiązywać:
skoro te dwie strefy są różne i sekwencyjne, żadna paczka nigdy nie jest w obu naraz. Oznacza to, że
klasyfikacja obliczona podczas ważenia musi zostać **zapamiętana** aż do chwili, gdy paczka dotrze do
`Diverting` — to prawdziwa, umotywowana potrzeba pamięci między tickami, nie sztuczny wymóg.

## Skąd startujesz

```bash
git fetch --tags
git switch -c <nazwa-twojego-brancha> module-06-start
```

Jak zawsze: `Engine::step()` i zachowanie `Plant::advance()` zostają dokładnie takie, jakie zostawił
je Moduł 5, aż do ostatniej misji.

## Mapa modułu

1. **Czujnik obecności** — `PresenceSensor`, czwarta klasa w kursie, z zupełnie nowym rodzajem
   niezmiennika: "nigdy nie udawaj, że pamiętasz coś, czego nigdy naprawdę nie zaobserwowałeś."
2. **Czujnik wagi** — `WeightSensor`, drugie (mniej prowadzone) zastosowanie tego samego wzorca.
3. **Klasyfikacja odporna na awarie** — `decideClassification`, odrzucająca niewiarygodny odczyt.
4. **Pamięć decyzji sterownika** — `ControllerState`, **najtrudniejszy i centralny problem tego
   modułu**: jak połączyć dwa potwierdzenia z dwóch różnych czujników, które nigdy nie są aktualne
   w tym samym ticku.
5. **Silnik z czujnikami** — spięcie wszystkiego, w tym poprawka na prawdziwy błąd projektowy: brak
   wiarygodnej decyzji musi naprawdę zatrzymać paczkę, a nie pozwolić jej jechać dalej.

## Zanim zaczniesz

- Testy Modułów 1–5 (`misja-1`–`misja-4`, `misja-6`–`misja-19`) są już obecne i przechodzą.
- Tak jak zawsze: nie edytujesz plików testowych ani `CMakeLists.txt`.

**Dalej:** [Misja 20: czujnik obecności](./01_czujnik_obecnosci.md).
