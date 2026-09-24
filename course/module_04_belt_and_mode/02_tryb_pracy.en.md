[🇵🇱 Polski](02_tryb_pracy.md) | 🇬🇧 English

# 4.2 Operating mode

## Problem

Until now, every question we asked our code was about one specific parcel: where it is, how much it
weighs, where it's headed. But we've never asked a question about the **whole system**: is it even
running? We need a single value that describes that, and a way to change it.

## New C++ elements

**`enum class Mode { Idle, Running }`** — two values to start with: the conveyor is stopped, or the
conveyor is running.

**The free function `Mode modeStep(Mode current, bool startRequested, bool stopRequested)`** — takes
the current mode and two requests, returns the new mode. The same shape as `classify`/
`toDiverterCommand` from the Controller: no internal state, just turning input into output.

## Why this isn't a class

In this module there's **no reason at all** to make `Mode` a class. Nothing here needs protecting —
`Engine` (which you'll get to know more closely in the next mission) simply stores the `Mode` value
and passes it on to `modeStep`, the same way it already stores and passes on the tick counter. This is
a deliberate contrast with Mission 13: `BeltMotor` had a concrete invariant (`actual_` only changes
through `resolve()`, one step at a time) — `Mode` doesn't have one yet. If a future module comes up
with a concrete reason to change that, that's the point where it would be worth revisiting — not
before, and not "just in case."

## The conflict rule

What happens when `startRequested` and `stopRequested` are both true **in the same call**? The rule is
unambiguous: **`stopRequested` wins**.

- `stopRequested` true → the result is `Idle`, regardless of `startRequested`.
- otherwise, `startRequested` true → the result is `Running`.
- otherwise → no change.

## What you already have

[`include/psm/mode.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-04-start/include/psm/mode.hpp) — `enum class Mode` and the `modeStep` declaration
are already there.

[`src/mode.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-04-start/src/mode.cpp) — an empty skeleton with a `// TODO` comment.

## What you need to write

Fill in the body of `modeStep` according to the conflict rule above — checking in exactly this order
(`stopRequested` first, then `startRequested`, then no change).

## Check your work

```bash
ctest --preset test -L misja-14
```

Expected result: `100% tests passed, 0 tests failed out of 1`. The test checks all four combinations
of `startRequested`/`stopRequested`, including both true at once.

## Common mistakes

- **Checking `startRequested` before `stopRequested`** — this reverses the conflict rule and gives the
  wrong answer exactly in the case where both are true at once.
- **Returning `Running`/`Idle` hard-coded** instead of `current` in the "no change" case — the
  function must return exactly what it was given when neither request is active.

## Something to think about

`BeltMotor` (Mission 13) and `Mode` (this mission) are both "something that changes over time" — but
one is a class and the other isn't. If someone proposed turning `Mode` into a class too, "for
consistency with `BeltMotor`," would that be a good idea? What would it fail to change, and what would
it needlessly complicate?

**Next:** [Mission 15: the conveyor under mode control](./03_przenosnik_pod_kontrola_trybu.en.md).
