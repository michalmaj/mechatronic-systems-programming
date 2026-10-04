🇵🇱 Polski | [🇬🇧 English](00_wprowadzenie.en.md)

# 6.0 Wprowadzenie

Do tej pory sterownik korzystał bezpośrednio z `Item::mass`, zakładając, że masa jest zawsze
dostępna i poprawna. Teraz paczkę zważy czujnik wagi, a jej położenie potwierdzi czujnik obecności.
Oba czujniki mogą zgłaszać usterki.

Każdy czujnik obserwuje tylko jedno miejsce przenośnika, a nie stan całej linii. Czujnik obecności
wykrywa paczkę wyłącznie w strefie `PresenceCheck`, a czujnik wagi dokonuje pomiaru tylko w
`Weighing`.
Ponieważ te strefy są osobne i występują kolejno, paczka nigdy nie znajduje się w obu naraz. Dlatego
klasyfikacja obliczona podczas ważenia musi zostać **zapamiętana** aż do chwili, gdy paczka dotrze do
`Diverting`. Po raz pierwszy sterownik musi więc przechowywać informację między tickami.

## Skąd startujesz

```bash
git fetch --tags
git switch -c <nazwa-twojego-brancha> module-06-start
```

Do ostatniej misji pozostaw `Engine::step()` i `Plant::advance()` w wersji z modułu 5.

## Mapa modułu

1. **Czujnik obecności:** `PresenceSensor` zapamiętuje wyłącznie wartości, które rzeczywiście
   odczytał w swojej strefie.
2. **Czujnik wagi:** `WeightSensor` stosuje te same zasady do pomiaru masy.
3. **Klasyfikacja odporna na awarie:** `decideClassification` odrzuca niewiarygodny odczyt.
4. **Pamięć decyzji sterownika:** `ControllerState` łączy informacje z dwóch czujników, które nie są
   dostępne w tym samym ticku.
5. **`Engine` z czujnikami:** połączenie wszystkich elementów. Brak wiarygodnej klasyfikacji
   zatrzymuje paczkę przed wyborem wyjścia.

## Zanim zaczniesz

- Testy modułów 1–5 (`misja-1`–`misja-4`, `misja-6`–`misja-19`) są już obecne i przechodzą.
- Tak jak zawsze: nie edytujesz plików testowych ani `CMakeLists.txt`.

**Dalej:** [Misja 20: czujnik obecności](./01_czujnik_obecnosci.md).
