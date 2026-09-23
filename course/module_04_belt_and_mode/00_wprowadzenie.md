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

Ważna różnica względem struktury tego modułu: **`Engine::step()` zostaje dokładnie taki, jaki
zostawił go Moduł 3, aż do ostatniej misji.** Najpierw budujesz `BeltMotor` (Misja 13) i `Mode`
(Misja 14) jako samodzielne, osobno testowalne elementy — tak, jak w prawdziwej historii
tego projektu budowano tego typu elementy przed jakąkolwiek integracją. Dopiero Misja 15 spina
wszystko razem.

## O teście `misja-11`

Zauważysz, że test `misja-11` (odziedziczony po Module 3) wygląda inaczej, niż mógłbyś pamiętać —
sprawdza teraz tylko, że licznik ticków działa poprawnie na pustym `Engine`, nic więcej. To celowe i
trwałe zawężenie: jego poprzedni, szczegółowy kontrakt (dokładne strefy paczki w konkretnych tickach)
zakładał natychmiastowy ruch bez żadnej bramki na pasie — coś, co ten moduł świadomie zmienia. Nie
musisz nic w tym teście poprawiać ani rozumieć, dlaczego jest inny niż wcześniej — po prostu jest
teraz taki, jaki ma być.

## Mapa modułu

1. **Silnik przenośnika** — `BeltMotor`, drugie użycie wzorca komenda-a-rzeczywistość z Modułu 2.
2. **Tryb pracy** — `Mode`, pierwsze pojęcie dotyczące całego systemu, i pierwszy przypadek, w którym
   *nie* budujemy klasy.
3. **Przenośnik pod kontrolą trybu** — moment, w którym wszystko się spina: paczka rusza dopiero, gdy
   pas naprawdę jest `Running`.

## Zanim zaczniesz

- Testy Modułów 1–3 (`misja-1`–`misja-4`, `misja-6`–`misja-11`) są już obecne i przechodzą.
- Tak jak zawsze: nie edytujesz plików testowych ani `CMakeLists.txt`.

**Dalej:** [Misja 13: silnik przenośnika](./01_silnik_przenosnika.md).
