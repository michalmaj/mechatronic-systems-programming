# 4.2 Tryb pracy

## Problem

Do tej pory każde pytanie, jakie zadawaliśmy naszemu kodowi, dotyczyło jednej konkretnej paczki: gdzie
jest, ile waży, dokąd jedzie. Ale nigdy nie zadaliśmy pytania o **cały system**: czy w ogóle jest
uruchomiony? Potrzebujemy jednej wartości, która to opisuje, i sposobu, żeby ją zmieniać.

## Nowe elementy C++

**`enum class Mode { Idle, Running }`** — dwie wartości na start: przenośnik stoi, albo przenośnik
pracuje.

**Wolna funkcja `Mode modeStep(Mode current, bool startRequested, bool stopRequested)`** — bierze
obecny tryb i dwa żądania, zwraca nowy tryb. Ten sam kształt co `classify`/`toDiverterCommand` z
Controllera: brak stanu wewnętrznego, tylko przekształcenie wejścia w wyjście.

## Dlaczego to nie jest klasa

W tym module nie ma **żadnego powodu**, żeby robić z `Mode` klasę. Nic tu nie wymaga ochrony —
`Engine` (który poznasz bliżej w kolejnej misji) po prostu przechowuje wartość `Mode` i przekazuje ją
dalej do `modeStep`, tak samo jak już przechowuje i przekazuje dalej licznik ticków. To jest
świadomy kontrast z Misją 13: `BeltMotor` miał konkretny niezmiennik (`actual_` zmienia się tylko
przez `resolve()`, krok po kroku) — `Mode` na razie żadnego nie ma. Gdyby w przyszłym module pojawił
się konkretny powód, żeby to zmienić, to właśnie wtedy warto by do tego wrócić — nie wcześniej, i nie
"na wszelki wypadek".

## Reguła konfliktu

Co się dzieje, gdy `startRequested` i `stopRequested` są prawdziwe **w tym samym wywołaniu**? Reguła
jest jednoznaczna: **`stopRequested` wygrywa**.

- `stopRequested` prawdziwe → wynik to `Idle`, niezależnie od `startRequested`.
- w przeciwnym razie `startRequested` prawdziwe → wynik to `Running`.
- w przeciwnym razie → bez zmian.

## Co już masz gotowe

[`include/psm/mode.hpp`](../../include/psm/mode.hpp) — `enum class Mode` i deklaracja `modeStep` już
gotowe.

[`src/mode.cpp`](../../src/mode.cpp) — pusty szkielet z komentarzem `// TODO`.

## Co masz napisać

Uzupełnij ciało `modeStep` zgodnie z regułą konfliktu powyżej — dokładnie w tej kolejności
sprawdzeń (najpierw `stopRequested`, potem `startRequested`, potem bez zmian).

## Sprawdź się

```bash
ctest --preset test -L misja-14
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza wszystkie cztery
kombinacje `startRequested`/`stopRequested`, w tym oba jednocześnie prawdziwe.

## Częste błędy

- **Sprawdzenie `startRequested` przed `stopRequested`** — to odwraca regułę konfliktu i
  da złą odpowiedź dokładnie w przypadku, gdy oba są prawdziwe naraz.
- **Zwrócenie `Running`/`Idle` na sztywno** zamiast `current` w przypadku "bez zmian" — funkcja musi
  zwrócić dokładnie to, co dostała, gdy żadne z żądań nie jest aktywne.

## Pytanie do zastanowienia

`BeltMotor` (Misja 13) i `Mode` (ta misja) to oba "coś, co się zmienia w czasie" — ale jedno jest
klasą, a drugie nie. Gdyby ktoś zaproponował zamianę `Mode` na klasę "dla porządku, żeby było
spójnie z `BeltMotor`", czy to byłby dobry pomysł? Czego by to nie zmieniło, a co by niepotrzebnie
skomplikowało?

**Dalej:** [Misja 15: przenośnik pod kontrolą trybu](./03_przenosnik_pod_kontrola_trybu.md).
