[🇵🇱 Polski](03_dwie_niezalezne_sciezki.md) | 🇬🇧 English

# 5.3 Two independent paths

## Problem

`EStopLatchState` and the extended `Mode` exist, but nothing yet translates them into concrete effects
on the actuators.

## New C++ elements

**Two pure, stateless decision functions:**

```cpp
struct SafetyDecision {
    bool overrideActive;
};

SafetyDecision checkEmergencyOverride(EStopLatchState latch);
bool diverterMayMove(Mode mode);
```

`checkEmergencyOverride` — true when `latch` isn't `Released` (both `Engaged` and `Armed` count).
`diverterMayMove` — true only for `Mode::Running`. Both are ordinary, testable functions, in the same
spirit as `classify`/`toDiverterCommand` from the Controller.

**`BeltMotor::forceStop()` is different in nature.** It's a **deliberately mutating emergency
operation**, not a pure decision. Required behavior: `forceStop()` sets **both** `command_` to `Stop`
**and** `actual_` to `Stopped`, in the same call — the requested command and the physical state become
consistent immediately, skipping `RampingDown`. That's the whole point of the emergency path: it
doesn't wait for a ramp, and it doesn't leave "wants to run" behind as a pending command that the next
tick might accidentally act on.

## Why there's no `filterRoutineBeltCommand` here

It might seem natural to add a function that filters the belt's "requested" command based on `Mode` —
similar to how `diverterMayMove` filters diverter movement. But the only "requested" value that even
exists at this stage is exactly `mode == Running ? Run : Stop` — so a filtering function would be
comparing a value against the very condition that just produced it. That's a tautology; it wouldn't
change anything. At this stage, the routine path **is** simply the existing `Mode → BeltMotor` control
from Module 4, unchanged. Safety filtering will come back in a later module, once a genuinely
independent "request" appears that's actually worth filtering.

## What you already have

[`include/psm/safety_supervisor.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-05-start/include/psm/safety_supervisor.hpp) — declarations
complete, as above.

[`src/safety_supervisor.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-05-start/src/safety_supervisor.cpp) — empty skeletons of both functions.

`BeltMotor::forceStop()` has been declared in
[`include/psm/belt_motor.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-05-start/include/psm/belt_motor.hpp); its empty skeleton is waiting in
[`src/belt_motor.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-05-start/src/belt_motor.cpp).

## What you need to write

- `checkEmergencyOverride` and `diverterMayMove` — two simple, one-line decisions.
- `BeltMotor::forceStop()` — two assignments, matching the required behavior above.

None of these three things touches `Engine` yet — that's Mission 19.

## Check your work

```bash
ctest --preset test -L misja-18
```

Expected result: `100% tests passed, 0 tests failed out of 1`. The test checks both decision
functions, and — importantly — that after `forceStop()`, a subsequent `resolve()` does **not** start
ramping up again (which proves `command_` was really changed, not just `actual_`).

## Common mistakes

- **`forceStop()` setting only `actual_`**, not `command_` — then the next `resolve()` (still
  "wanting" `Run`) would immediately start `RampingUp` again, completely undoing the point of the
  emergency stop.
- **`diverterMayMove` checking something other than exactly `Mode::Running`** — no other `Mode` value
  allows the diverter to move.

## Something to think about

`checkEmergencyOverride` and `diverterMayMove` are pure; `forceStop()` deliberately isn't. Why does
that distinction matter specifically for an **emergency** operation, when it hasn't gotten in the way
of any of the course's earlier, "ordinary" class methods?

**Next:** [Mission 19: the engine under protection](./04_silnik_pod_ochrona.en.md).
