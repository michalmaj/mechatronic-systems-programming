# 2.0 Wprowadzenie

Moduł 1 zostawił Cię z działającym, ale wyidealizowanym systemem: paczka wjeżdżała, przenośnik
"decydował", a rozjazd natychmiast, w tym samym ticku, ustawiał się tam, gdzie trzeba. W prawdziwym
świecie żadne urządzenie fizyczne nie działa w ten sposób — silnik, siłownik czy rozjazd zawsze
potrzebuje czasu, żeby zmienić położenie.

Ten moduł naprawia właśnie to. Dywerter przestaje teleportować się między `Straight` i `Diverted` —
zaczyna przechodzić przez stan pośredni, w którym fizycznie się przemieszcza, i to może zająć więcej
niż jeden tick. To pierwszy moduł, w którym `Plant` naprawdę **czeka** na urządzenie, zamiast dostawać
gotową decyzję z zewnątrz.

To także pierwszy moduł, w którym w kursie pojawia się `class`.

## Skąd startujesz

Pobierz punkt startowy tego modułu:

```bash
git fetch --tags
git switch -c <nazwa-twojego-brancha> module-02-start
```

`module-02-start` zawiera już **cały, poprawnie działający Moduł 1** — testy `misja-1` do `misja-4`
oraz `misja-6` są obecne i przechodzą od razu, bez żadnej pracy z Twojej strony. To nie jest coś, co
robisz od nowa — to odziedziczona, gotowa podstawa, na której buduje ten moduł. Twoim zadaniem są
wyłącznie trzy nowe misje.

(Jeśli zastanawiasz się, co stało się z testem `misja-5` z Modułu 1 — jego rola została wchłonięta
przez test Misji 9 tego modułu, ponieważ sposób, w jaki cała pętla ticków działa, zmienia się w tym
module. Więcej o tym w Misji 9.)

## Mapa modułu

1. **Polecenie a rzeczywistość** — dlaczego "czego chcemy" i "co fizycznie istnieje" to dwie różne
   rzeczy.
2. **Dywerter jako klasa** — pierwsza klasa w kursie: chroniony stan wewnętrzny i publiczny
   interfejs.
3. **Przenośnik czeka na dywerter** — spięcie wszystkiego w jedną, poprawną całość.

## Zanim zaczniesz

- Tak jak w Module 1: nie edytujesz `CMakeLists.txt`, testy są dostarczone przez kurs, a scaffolding
  maleje w miarę postępu przez misje.
- Ten moduł jest mniejszy niż Moduł 1 — trzy misje zamiast sześciu — bo buduje na już poznanych
  narzędziach (enumy, funkcje wolne, `std::optional`, pętle) zamiast wprowadzać je od nowa.

**Dalej:** [Misja 7: polecenie a rzeczywistość](./01_polecenie_a_rzeczywistosc.md).
