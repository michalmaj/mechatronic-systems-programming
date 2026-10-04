🇵🇱 Polski | [🇬🇧 English](00_wprowadzenie.en.md)

# 2.0 Wprowadzenie

Po module 1 masz działający, lecz uproszczony system. Rozjazd zmienia w nim położenie natychmiast,
w tym samym ticku. Fizyczny silnik lub siłownik potrzebuje jednak czasu na wykonanie ruchu.

W tym module dywerter zacznie przechodzić między `Straight` i `Diverted` przez stan pośredni
`Moving`. Zmiana położenia zajmie więcej niż jeden tick, więc `Plant` będzie musiał poczekać na
zakończenie ruchu.

To także pierwszy moduł, w którym w kursie pojawia się `class`.

## Skąd startujesz

Pobierz punkt startowy tego modułu:

```bash
git fetch --tags
git switch -c <nazwa-twojego-brancha> module-02-start
```

`module-02-start` zawiera kompletne rozwiązanie modułu 1. Testy `misja-1`–`misja-4` oraz `misja-6`
powinny przechodzić od razu. W tym module wykonasz trzy nowe misje.

Jeśli zastanawiasz się, co stało się z testem `misja-5` z modułu 1, jego rolę przejął test misji 9.
W tym module zmienia się sposób działania całej pętli ticków. Więcej na ten temat dowiesz się w
misji 9.

## Mapa modułu

1. **Polecenie a rzeczywistość:** dlaczego „czego chcemy” i „co fizycznie istnieje” to dwie różne
   rzeczy.
2. **Dywerter jako klasa:** pierwsza klasa w kursie, chroniony stan wewnętrzny i publiczny
   interfejs.
3. **Przenośnik czeka na dywerter:** połączenie wszystkich elementów w działającą całość.

## Zanim zaczniesz

- Tak jak w module 1: nie edytujesz `CMakeLists.txt`, testy są dostarczone przez kurs, a poziom
  prowadzenia maleje w miarę postępu przez misje.
- Ten moduł ma trzy misje zamiast sześciu, ponieważ korzysta z poznanych już narzędzi: typów
  wyliczeniowych, funkcji wolnych, `std::optional` i pętli.

**Dalej:** [Misja 7: polecenie a rzeczywistość](./01_polecenie_a_rzeczywistosc.md).
