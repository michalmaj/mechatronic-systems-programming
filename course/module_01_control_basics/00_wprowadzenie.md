🇵🇱 Polski | [🇬🇧 English](00_wprowadzenie.en.md)

# 1.0 Wprowadzenie

W module 0 uruchomiłeś dwa programy: `toolchain_check` i gotowy podgląd `simulator_cli`. Nie
napisałeś jeszcze żadnej logiki programu. Celem było przygotowanie działającego środowiska.

Teraz zaczyna się właściwa praca. W tym module zbudujesz od zera mały, ale kompletny system:
paczkę, która porusza się przez kolejne strefy przenośnika i na końcu zostaje skierowana w jedną z
dwóch stron, zależnie od swojej wagi. Na koniec modułu ten sam `simulator_cli`, który wcześniej
tylko oglądałeś, pokaże na konsoli działanie napisanego przez Ciebie kodu.

## Skąd startujesz

Kod startowy **kompiluje się od razu**, ale nie wykonuje jeszcze użytecznej pracy.
Każda funkcja, którą będziesz uzupełniać, istnieje już w projekcie jako pusty szkielet z komentarzem
`// TODO`. Twoim zadaniem w każdej misji jest wypełnienie jednego takiego miejsca, aż odpowiadający
mu test przestanie zgłaszać błąd.

Pobierz punkt startowy modułu:

```bash
git fetch --tags
git switch -c <nazwa-twojego-brancha> module-01-start
```

`module-01-start` to **tag**, czyli stały punkt w historii Gita, a nie branch. Polecenie
`git switch -c` tworzy w tym miejscu Twój branch. Możesz na nim zapisywać pracę przez `git add` i
`git commit`, nie zmieniając samego tagu.

## Jak wygląda jedna misja

Moduł składa się z sześciu misji. Każda z nich:

1. stawia konkretny problem związany z naszą komórką sortującą,
2. wprowadza elementy C++ potrzebne do rozwiązania tego problemu,
3. mówi, co już jest gotowe, a co masz dopisać,
4. kończy się uruchomieniem **testu tej misji**,
5. na końcu (w ostatniej misji) prosi o uruchomienie **całego** zestawu testów naraz.

Testy do wszystkich sześciu misji są już w projekcie. Twoim zadaniem jest
sprawić, żeby przechodziły. Pisanie własnych testów to temat na później.

## Mapa modułu

1. **Paczka i strefy:** czym jest paczka i gdzie może się znajdować.
2. **Ruch paczki:** jak przesunąć paczkę o jedną strefę do przodu.
3. **Stan przenośnika:** co się dzieje, gdy przenośnik jest pusty.
4. **Decyzja sortowania:** jak zdecydować, w którą stronę skierować paczkę.
5. **Pętla sterowania:** jak powtórzyć ten cykl wiele razy z rzędu.
6. **Pierwszy przebieg:** Twój własny `main()`, wypisujący wynik na konsolę.

## Zanim zaczniesz

- Nie musisz nic zmieniać w plikach `CMakeLists.txt`. Konfiguracja budowania jest już gotowa.
- Nie musisz pisać ani modyfikować testów. W tym module tylko je uruchamiasz.
- Jeśli utkniesz, każda misja ma sekcję z najczęstszymi błędami na tym etapie.

**Dalej:** [Misja 1: paczka i strefy](./01_paczka_i_strefy.md).
