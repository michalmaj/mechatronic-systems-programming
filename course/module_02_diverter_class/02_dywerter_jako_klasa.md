# 2.2 Dywerter jako klasa

To najważniejsza teoretycznie misja tego modułu — poświęcimy jej trochę więcej miejsca niż zwykle,
bo wprowadza pierwszą klasę w całym kursie.

## Problem

Prawdziwy rozjazd nie przeskakuje natychmiast z jednej pozycji w drugą — musi się fizycznie
przemieścić, co zajmuje czas. Potrzebujemy typu, który to odzwierciedla: pamięta, czego od niego
chcemy (`DiverterCommand`), pamięta, gdzie faktycznie jest (`DiverterPosition`), i **nie pozwala
nikomu z zewnątrz ustawić tej drugiej wartości na dowolną liczbę** — jedynym sposobem, żeby faktyczna
pozycja się zmieniła, jest wywołanie jednej, konkretnej metody, krok po kroku.

## Dlaczego `class`, a nie `struct`

To ważne, żeby nie zrozumieć tego opacznie: w C++ `struct` też może mieć prywatne pola i chronić
dokładnie ten sam niezmiennik co `class` — jedyna techniczna różnica między `struct` a `class` to
domyślny poziom dostępu (`public` kontra `private`). To nie słowo kluczowe `class` samo w sobie nas
chroni.

Mechanizmem ochronnym jest **enkapsulacja** — sekcja `private` plus publiczny interfejs. `class`
używamy tutaj jako **świadomej konwencji nazewniczej** tego kursu: typ z ukrytym stanem wewnętrznym i
publicznym interfejsem, który go chroni, nazywamy `class`, żeby sama nazwa typu sygnalizowała "tu jest
niezmiennik do ochrony". `Plant` w Module 1 został `struct`-em właśnie dlatego, że nic wtedy nie
chronił — nie było żadnego niezmiennika.

## Nowy element C++: `class`

```cpp
class Diverter {
public:
    void setCommand(DiverterCommand command);
    void resolve();
    DiverterPosition actualPosition() const;
    bool isSettled() const;

private:
    DiverterCommand command_ = DiverterCommand::HoldStraight;
    DiverterPosition actual_ = DiverterPosition::Straight;
};
```

- `public` — metody, przez które świat zewnętrzny wchodzi w interakcję z dywerterem.
- `private` — pola `command_`/`actual_`, niedostępne z zewnątrz w żaden inny sposób niż przez te
  metody.
- `const` na końcu `actualPosition()`/`isSettled()` — te metody tylko odczytują stan, nigdy go nie
  zmieniają (poznasz to dokładniej, gdy będziesz pisać własne klasy w kolejnych modułach; na razie
  wystarczy wiedzieć, że to obietnica "ta metoda nic nie zmienia").

**Deklaracje** (nazwy metod, ich sygnatury, pola wraz z wartościami początkowymi) są już kompletne —
to, co napiszesz, to **ciała** czterech metod.

## Niezmiennik, który chronimy

`actual_` może się zmienić wyłącznie przez `resolve()`, i to o dokładnie jeden krok na raz, zgodnie z
poniższą tabelą. Nic innego w kodzie nie ma prawa przypisać do `actual_` dowolnej wartości.

## Tabela przejść `resolve()`

Pozycja docelowa (`target`) zależy od `command_`: `HoldStraight` → `Straight`, `Divert` → `Diverted`.

| `actual_` przed | `command_`   | `target`   | `actual_` po |
|-------------------|--------------|------------|-----------------|
| `Straight`        | HoldStraight | `Straight` | `Straight` (bez zmian — już tam jest) |
| `Straight`        | Divert       | `Diverted` | `Moving` (zaczyna się ruch) |
| `Diverted`        | HoldStraight | `Straight` | `Moving` (zaczyna się ruch) |
| `Diverted`        | Divert       | `Diverted` | `Diverted` (bez zmian — już tam jest) |
| `Moving`          | HoldStraight | `Straight` | `Straight` (dojeżdża) |
| `Moving`          | Divert       | `Diverted` | `Diverted` (dojeżdża) |

Każde wywołanie `resolve()` przesuwa co najwyżej o jeden krok: ustawiony-poprawnie zostaje na
miejscu, ustawiony-źle zaczyna się ruszać, ruszający się dojeżdża.

## Semantyka `isSettled()`

`isSettled()` to **żywe porównanie**, liczone na nowo przy każdym wywołaniu — nigdy zapamiętana
wcześniej odpowiedź:

```cpp
return actual_ == target(command_);
```

To ma konkretną konsekwencję: jeśli wywołasz `setCommand(...)` i **nie** wywołasz jeszcze
`resolve()`, `isSettled()` musi natychmiast odzwierciedlić nową sytuację. Przykład: `actual_ ==
Straight`, `command_ == HoldStraight` → `isSettled() == true`. Teraz wywołujesz
`setCommand(Divert)`, ale jeszcze nie `resolve()` — `isSettled()` musi od razu zwrócić `false`
(cel to teraz `Diverted`, `actual_` jeszcze się nie ruszył). Gdyby `isSettled()` zwracał zapamiętaną
wcześniej flagę zamiast liczyć na nowo, dałby tu błędną odpowiedź.

## Co masz napisać

W [`src/diverter.cpp`](../../src/diverter.cpp) uzupełnij cztery metody:
- `setCommand(DiverterCommand)` — zapisz argument w `command_`.
- `resolve()` — zaimplementuj dokładnie tabelę przejść powyżej.
- `actualPosition() const` — zwróć `actual_`.
- `isSettled() const` — policz i zwróć porównanie opisane wyżej.

## Self-check

```bash
ctest --preset test -L misja-8
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test przechodzi przez pełną tabelę
przejść w obie strony (`Straight`→`Diverted` i z powrotem) oraz sprawdza `isSettled()` natychmiast po
`setCommand()`, przed jakimkolwiek `resolve()`.

## Częste błędy

- **`isSettled()` liczące na podstawie starej, zapamiętanej wartości** zamiast porównania na żywo —
  dokładnie ten błąd omówiony wyżej.
- **`resolve()` przeskakujące od razu do celu** z `Straight`/`Diverted` (pomijając `Moving`) —
  pamiętaj: settled-źle zawsze najpierw przechodzi przez `Moving`, dopiero kolejne `resolve()`
  dojeżdża do celu.
- **Zapomniany przypadek "już na miejscu"** — jeśli `actual_ == target`, `resolve()` nic nie robi
  (nie ma po co ruszać dywertera, który już tam jest).

## Pytanie do zastanowienia

Gdybyś zamiast `class` z sekcją `private` napisał `struct` z dokładnie tym samym podziałem na
`public`/`private`, czy kod zachowywałby się inaczej? Co faktycznie różni te dwa podejścia w tym
konkretnym przypadku?

**Dalej:** [Misja 9: przenośnik czeka na dywerter](./03_przenosnik_czeka_na_dywerter.md).
