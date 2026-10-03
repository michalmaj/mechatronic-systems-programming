🇵🇱 Polski | [🇬🇧 English](00_wprowadzenie.en.md)

# 4.0 Wprowadzenie

Moduł 3 zostawił Cię z `Engine` jako jedynym miejscem, w którym w ogóle istnieje kolejność jednego
ticka. Ten moduł rozbudowuje `Engine` o dwie nowe rzeczy: drugi aktuator (silnik przenośnika,
`BeltMotor`) oraz pierwsze pojęcie dotyczące **całego systemu**, a nie pojedynczej paczki — tryb pracy
(`Mode`).

## Skąd startujesz

```bash
git fetch --tags
git switch -c <nazwa-twojego-brancha> module-04-start
```

Do ostatniej misji nie zmieniaj `Engine::step()`. Najpierw zbudujesz i osobno przetestujesz
`BeltMotor` (misja 13) oraz `Mode` (misja 14). Połączysz je z `Engine` dopiero w misji 15.

## O teście `misja-11`

Test `misja-11` sprawdza teraz jedynie licznik ticków pustego `Engine`. Poprzednia wersja oczekiwała
konkretnych stref w kolejnych tickach, zakładając natychmiastowy ruch paczki. Po dodaniu napędu takie
założenie przestaje obowiązywać, dlatego test został zawężony.

## Mapa modułu

1. **Silnik przenośnika** — `BeltMotor`, drugie użycie wzorca komenda-a-rzeczywistość z modułu 2.
2. **Tryb pracy** — `Mode`, pierwsze pojęcie dotyczące całego systemu, i pierwszy przypadek, w którym
   *nie* budujemy klasy.
3. **Przenośnik pod kontrolą trybu** — moment, w którym wszystko się spina: paczka rusza dopiero, gdy
   pas naprawdę jest `Running`.

## Zanim zaczniesz

- Testy Modułów 1–3 (`misja-1`–`misja-4`, `misja-6`–`misja-11`) są już obecne i przechodzą.
- Tak jak zawsze: nie edytujesz plików testowych ani `CMakeLists.txt`.

**Dalej:** [Misja 13: silnik przenośnika](./01_silnik_przenosnika.md).
