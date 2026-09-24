[🇵🇱 Polski](01_silnik_przenosnika.md) | 🇬🇧 English

# 4.1 The belt motor

## Problem

The whole conveyor moves thanks to one motor driving the belt. Just as the diverter from Module 2
didn't jump instantly between positions, this motor doesn't reach full speed or stop in a single
instant either — it needs time to ramp up and to ramp down.

## New C++ element

**`class BeltMotor`** — the second use of the command-vs-actual-state pattern from Module 2
(`Diverter`), this time on a machine with **four** states instead of three:

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

This is the second time you're building this same combination: `class`, private state, public
interface, command kept separate from actual state. This time you're given less to start from than
with `Diverter` in Module 2 — instead of a full transition table, just the rule, described in words.
Try translating it into a `switch` yourself, the same way you did last time.

## The rule

- From `Stopped`: a `Run` command starts ramping up (→ `RampingUp`).
- From `RampingUp`: continuing `Run` ends at `Running`; switching to `Stop` **turns back** toward
  `RampingDown`.
- From `Running`: a `Stop` command starts ramping down (→ `RampingDown`).
- From `RampingDown`: continuing `Stop` ends at `Stopped`; switching to `Run` **turns back** toward
  `RampingUp`.

Each `resolve()` advances the state by at most one step.

**Important caveat:** `RampingUp`/`RampingDown` are a simplified model of start/stop **delay** — not a
model of physical speed. Nowhere in this class is there a number describing how fast the belt is
actually moving. A parcel moves only when `actualState() == Running` — exactly that, and nothing in
between.

## What you already have

[`include/psm/belt_motor_command.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-04-start/include/psm/belt_motor_command.hpp),
[`include/psm/belt_motor_state.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-04-start/include/psm/belt_motor_state.hpp), and
[`include/psm/belt_motor.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-04-start/include/psm/belt_motor.hpp) — declarations complete, exactly
as above.

[`src/belt_motor.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-04-start/src/belt_motor.cpp) — empty skeletons of the three methods with `// TODO`
comments.

## What you need to write

Fill in the bodies of `setCommand`, `resolve`, and `actualState` according to the rule above.

## Check your work

```bash
ctest --preset test -L misja-13
```

Expected result: `100% tests passed, 0 tests failed out of 1`. The test walks through the full
ramp-up-and-stop cycle, as well as turning back mid-ramp.

## Common mistakes

- **Jumping straight to `Running`/`Stopped`** from `Stopped`/`Running` without passing through a
  ramping state — remember, `resolve()` always advances by one step, never more.
- **Not handling turning back mid-ramp** — if the command switches from `Run` to `Stop` while
  `actual_` is `RampingUp`, the result is `RampingDown`, not a continuation toward `Running`.
- **Treating `RampingUp`/`RampingDown` as "somewhat moving"** — for parcel movement, only the exact
  value `Running` counts; nothing in between is enough.

## Something to think about

`Diverter` (Module 2) had three states; `BeltMotor` has four. What specifically about this extra pair
of states (`RampingUp`/`RampingDown` instead of one shared `Moving`) lets you distinguish "heading
toward running" from "heading toward stopped" — and why didn't the diverter need that distinction?

**Next:** [Mission 14: operating mode](./02_tryb_pracy.en.md).
