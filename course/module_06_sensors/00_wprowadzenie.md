🇵🇱 Polski | [🇬🇧 English](00_wprowadzenie.en.md)

# 6.0 Wprowadzenie

Do tej pory `Controller` bezwarunkowo ufał `Item::mass`: masa była zawsze dostępna i poprawna.
Teraz paczka będzie ważona przez
czujnik, który może się zepsuć, i wykrywana przez czujnik obecności, który też może się zepsuć.

W odróżnieniu od dotychczasowego modelu czujnik **nie zna stanu całej linii**. Jest zamontowany w
jednym miejscu przenośnika: czujnik
obecności widzi paczkę wyłącznie w strefie `PresenceCheck`, czujnik wagi wyłącznie w `Weighing`.
Ponieważ te strefy są osobne i występują kolejno, paczka nigdy nie znajduje się w obu naraz. Dlatego
klasyfikacja obliczona podczas ważenia musi zostać **zapamiętana** aż do chwili, gdy paczka dotrze do
`Diverting` — to prawdziwa, umotywowana potrzeba pamięci między tickami, nie sztuczny wymóg.

## Skąd startujesz

```bash
git fetch --tags
git switch -c <nazwa-twojego-brancha> module-06-start
```

Do ostatniej misji pozostaw `Engine::step()` i `Plant::advance()` w wersji z modułu 5.

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
