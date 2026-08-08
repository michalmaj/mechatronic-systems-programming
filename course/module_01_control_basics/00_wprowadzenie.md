# 1.0 Wprowadzenie

W Module 0 uruchomiłeś dwa programy: `toolchain_check` i gotowy podgląd `simulator_cli`. Nie
napisałeś ani jednej linijki logiki — chodziło wyłącznie o to, żeby Twoje środowisko działało.

Teraz zaczyna się właściwa praca. W tym module zbudujesz od zera mały, ale kompletny system:
paczkę, która porusza się przez kolejne strefy przenośnika i na końcu zostaje skierowana w jedną z
dwóch stron — w zależności od swojej wagi. Na koniec modułu ten sam `simulator_cli`, który wcześniej
tylko obejrzałeś, będzie Twoim własnym programem, wypisującym na konsolę wynik Twojej pracy.

## Skąd startujesz

Kod, który dostajesz na start, **kompiluje się od razu** — ale nie robi jeszcze nic sensownego.
Każda funkcja, którą będziesz uzupełniać, istnieje już w projekcie jako pusty szkielet z komentarzem
`// TODO`. Twoim zadaniem w każdej misji jest wypełnienie jednego takiego miejsca, aż odpowiadający
mu test przestanie zgłaszać błąd.

Pobierz punkt startowy modułu:

```bash
git fetch --tags
git switch -c <nazwa-twojego-brancha> module-01-start
```

`module-01-start` to **tag** — nieruchomy punkt w historii Gita, nie branch. `git switch -c` tworzy
nowy, Twój własny branch, który zaczyna się dokładnie w tym punkcie. Dzięki temu masz gdzie
bezpiecznie zapisywać własną pracę (`git add`, `git commit`) — sam tag się nie zmienia i zawsze
możesz do niego wrócić.

## Jak wygląda jedna misja

Moduł składa się z sześciu misji. Każda z nich:

1. stawia konkretny problem związany z naszą stacją sortującą,
2. wprowadza dokładnie te elementy C++, które są potrzebne, żeby ten problem rozwiązać — nic więcej,
3. mówi, co już jest gotowe, a co masz dopisać,
4. daje Ci szybki sposób sprawdzenia się: uruchomienie **tylko testu tej misji**,
5. na końcu (w ostatniej misji) prosi o uruchomienie **całego** zestawu testów naraz.

Testy do wszystkich sześciu misji już istnieją w projekcie — napisał je kurs, nie Ty. Twoja rola to
sprawić, żeby przechodziły. Pisanie własnych testów to temat na później.

## Mapa modułu

1. **Paczka i strefy** — czym jest paczka i gdzie może się znajdować.
2. **Ruch paczki** — jak przesunąć paczkę o jedną strefę do przodu.
3. **Stan przenośnika** — co się dzieje, gdy przenośnik jest pusty.
4. **Decyzja sortowania** — jak zdecydować, w którą stronę skierować paczkę.
5. **Pętla sterowania** — jak powtórzyć ten cykl wiele razy z rzędu.
6. **Pierwszy przebieg** — Twój własny `main()`, wypisujący wynik na konsolę.

## Zanim zaczniesz

- Nie musisz nic zmieniać w plikach `CMakeLists.txt` — cała konfiguracja budowania jest już gotowa.
- Nie musisz pisać ani modyfikować testów — tylko je uruchamiać.
- Jeśli utkniesz, każda misja ma sekcję z najczęstszymi błędami na tym etapie.

**Dalej:** [Misja 1: paczka i strefy](./01_paczka_i_strefy.md).
