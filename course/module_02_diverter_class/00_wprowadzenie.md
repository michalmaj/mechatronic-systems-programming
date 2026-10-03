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

(Jeśli zastanawiasz się, co stało się z testem `misja-5` z modułu 1 — jego rola została wchłonięta
przez test Misji 9 tego modułu, ponieważ sposób, w jaki cała pętla ticków działa, zmienia się w tym
module. Więcej o tym w Misji 9.)

## Mapa modułu

1. **Polecenie a rzeczywistość** — dlaczego "czego chcemy" i "co fizycznie istnieje" to dwie różne
   rzeczy.
2. **Dywerter jako klasa** — pierwsza klasa w kursie: chroniony stan wewnętrzny i publiczny
   interfejs.
3. **Przenośnik czeka na dywerter** — spięcie wszystkiego w jedną, poprawną całość.

## Zanim zaczniesz

- Tak jak w module 1: nie edytujesz `CMakeLists.txt`, testy są dostarczone przez kurs, a poziom
  prowadzenia maleje w miarę postępu przez misje.
- Ten moduł jest mniejszy niż moduł 1 — trzy misje zamiast sześciu — bo korzysta z poznanych już
  narzędziach (enumy, funkcje wolne, `std::optional`, pętle) zamiast wprowadzać je od nowa.

**Dalej:** [Misja 7: polecenie a rzeczywistość](./01_polecenie_a_rzeczywistosc.md).
