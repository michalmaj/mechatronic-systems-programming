# 4.1 Silnik przenośnika

## Problem

Cały przenośnik porusza się dzięki jednemu silnikowi napędzającemu pas. Tak jak rozjazd z Modułu 2 nie
przeskakiwał natychmiast między pozycjami, tak i ten silnik nie osiąga pełnej prędkości ani nie
zatrzymuje się w jednej chwili — potrzebuje czasu na rozpędzenie i na zatrzymanie.

## Nowy element C++

**`class BeltMotor`** — drugie użycie wzorca komenda-a-rzeczywistość z Modułu 2 (`Diverter`), tym
razem na maszynie z **czterema** stanami zamiast trzech:

```cpp
enum class BeltMotorCommand { Run, Stop };
enum class BeltMotorState { Stopped, RampingUp, Running, RampingDown };

class BeltMotor {
public:
    void setCommand(BeltMotorCommand command);
    void resolve();
    BeltMotorState actualState() const;

private:
    BeltMotorCommand command_ = BeltMotorCommand::Stop;
    BeltMotorState actual_ = BeltMotorState::Stopped;
};
```

To już drugi raz budujesz tę samą kombinację: `class`, prywatny stan, publiczny interfejs, komenda
oddzielona od rzeczywistości. Tym razem dostajesz mniej gotowego niż przy `Diverter` w Module 2 —
zamiast pełnej tabeli przejść, samą regułę, opisaną słowami. Spróbuj samodzielnie przełożyć ją na
`switch`, tak jak zrobiłeś to poprzednio.

## Reguła

- Z `Stopped`: komenda `Run` zaczyna rampowanie w górę (→ `RampingUp`).
- Z `RampingUp`: kontynuacja `Run` kończy się w `Running`; zmiana na `Stop` **zawraca** w stronę
  `RampingDown`.
- Z `Running`: komenda `Stop` zaczyna rampowanie w dół (→ `RampingDown`).
- Z `RampingDown`: kontynuacja `Stop` kończy się w `Stopped`; zmiana na `Run` **zawraca** w stronę
  `RampingUp`.

Każde `resolve()` przesuwa stan o co najwyżej jeden krok.

**Ważne zastrzeżenie:** `RampingUp`/`RampingDown` to uproszczony model **opóźnienia** rozruchu i
zatrzymania — nie model prędkości fizycznej. Nigdzie w tej klasie nie ma liczby opisującej, jak szybko
faktycznie jedzie pas. Paczka porusza się wyłącznie wtedy, gdy `actualState() == Running` — dokładnie
to, i nic pomiędzy.

## Co już masz gotowe

[`include/psm/belt_motor_command.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-04-start/include/psm/belt_motor_command.hpp),
[`include/psm/belt_motor_state.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-04-start/include/psm/belt_motor_state.hpp) i
[`include/psm/belt_motor.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-04-start/include/psm/belt_motor.hpp) — deklaracje kompletne, dokładnie
jak wyżej.

[`src/belt_motor.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-04-start/src/belt_motor.cpp) — puste szkielety trzech metod z komentarzami
`// TODO`.

## Co masz napisać

Uzupełnij ciała `setCommand`, `resolve` i `actualState` zgodnie z regułą powyżej.

## Sprawdź się

```bash
ctest --preset test -L misja-13
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test przechodzi przez pełny cykl
rozpędzenia i zatrzymania, a także przez zawrócenie w połowie rampowania.

## Częste błędy

- **Przeskoczenie od razu do `Running`/`Stopped`** z `Stopped`/`Running` bez przejścia przez stan
  rampowania — pamiętaj, `resolve()` zawsze przesuwa o jeden krok, nigdy więcej.
- **Brak obsługi zawrócenia w połowie rampowania** — jeśli komenda zmienia się z `Run` na `Stop`,
  gdy `actual_` to `RampingUp`, wynikiem jest `RampingDown`, nie kontynuacja w stronę `Running`.
- **Traktowanie `RampingUp`/`RampingDown` jako "trochę ruszonego"** — dla ruchu paczki liczy się
  wyłącznie dokładna wartość `Running`; nic pomiędzy nie wystarczy.

## Pytanie do zastanowienia

`Diverter` (Moduł 2) miał trzy stany, `BeltMotor` ma cztery. Co konkretnie w tej dodatkowej parze
stanów (`RampingUp`/`RampingDown` zamiast jednego wspólnego `Moving`) pozwala odróżnić "jadę w stronę
uruchomienia" od "jadę w stronę zatrzymania" — i dlaczego rozjazdowi to rozróżnienie nie było
potrzebne?

**Dalej:** [Misja 14: tryb pracy](./02_tryb_pracy.md).
