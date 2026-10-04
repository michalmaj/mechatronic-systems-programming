🇵🇱 Polski | [🇬🇧 English](01_silnik_przenosnika.en.md)

# 4.1 Silnik przenośnika

## Problem

Taśmę przenośnika napędza jeden silnik. Tak jak rozjazd z modułu 2 nie przeskakiwał natychmiast
między pozycjami, tak i ten silnik nie osiąga pełnej prędkości ani nie zatrzymuje się w jednej
chwili. Potrzebuje czasu na rozpędzenie i zatrzymanie.

## Nowy element C++

**`class BeltMotor`** ponownie rozdziela polecenie od rzeczywistego stanu urządzenia, podobnie jak
`Diverter` w module 2. Tym razem urządzenie ma **cztery** stany zamiast trzech:

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

Podobnie jak `Diverter`, klasa `BeltMotor` ma prywatny stan, publiczny interfejs i osobno przechowuje
polecenie oraz stan rzeczywisty. Tym razem dostajesz jednak mniej gotowego. Zamiast pełnej tabeli
przejść masz opis zasad działania. Przełóż go samodzielnie na instrukcję `switch`, tak jak w
poprzednim module.

## Zasady przejść między stanami

- W stanie `Stopped` polecenie `Run` rozpoczyna rozruch i zmienia stan na `RampingUp`.
- W stanie `RampingUp` polecenie `Run` doprowadza silnik do `Running`, a `Stop` rozpoczyna
  zatrzymywanie i zmienia stan na `RampingDown`.
- W stanie `Running` polecenie `Stop` rozpoczyna zatrzymywanie i zmienia stan na `RampingDown`.
- W stanie `RampingDown` polecenie `Stop` doprowadza silnik do `Stopped`, a `Run` ponownie rozpoczyna
  rozruch i zmienia stan na `RampingUp`.

Każde wywołanie `resolve()` wykonuje najwyżej jedno przejście między stanami.

`RampingUp` i `RampingDown` opisują jedynie **opóźnienie** rozruchu i zatrzymania, a nie prędkość
taśmy. Paczka porusza się tylko wtedy, gdy `actualState() == Running`.

## Co już masz gotowe

[`include/psm/belt_motor_command.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-04-start/include/psm/belt_motor_command.hpp),
[`include/psm/belt_motor_state.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-04-start/include/psm/belt_motor_state.hpp) i
[`include/psm/belt_motor.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-04-start/include/psm/belt_motor.hpp)
zawierają kompletne deklaracje pokazane wyżej.

W pliku
[`src/belt_motor.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-04-start/src/belt_motor.cpp)
znajdziesz puste szkielety trzech metod z komentarzami `// TODO`.

## Co masz napisać

Uzupełnij ciała `setCommand`, `resolve` i `actualState` zgodnie z regułą powyżej.

## Sprawdź się

```bash
ctest --preset test -L misja-13
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test przechodzi przez pełny cykl
rozpędzenia i zatrzymania. Sprawdza też zmianę polecenia w trakcie każdego z tych procesów.

## Częste błędy

- **Przejście od razu do `Running` lub `Stopped`** z pominięciem stanu pośredniego: `resolve()`
  wykonuje najwyżej jedno przejście między stanami.
- **Brak reakcji na zmianę polecenia podczas rozruchu lub zatrzymywania:** jeśli `actual_` ma wartość
  `RampingUp`, a polecenie zmienia się z `Run` na `Stop`, wynikiem jest `RampingDown`, a nie
  `Running`.
- **Uznanie `RampingUp` lub `RampingDown` za ruch taśmy:** paczka może się przesuwać wyłącznie w
  stanie `Running`.

## Pytanie do zastanowienia

`Diverter` z modułu 2 miał trzy stany, a `BeltMotor` ma cztery. Dlaczego silnik potrzebuje osobnych
stanów `RampingUp` i `RampingDown`, podczas gdy rozjazdowi wystarczał jeden wspólny stan `Moving`?

**Dalej:** [Misja 14: tryb pracy](./02_tryb_pracy.md).
