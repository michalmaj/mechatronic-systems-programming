[🇵🇱 Polski](01_zablokowany_dywerter.md) | 🇬🇧 English

# 7.1 The blocked diverter

## Problem

Module 6 taught the sensors to honestly admit a fault, instead of pretending everything works. An
actuator deserves the same treatment: a physically blocked diverter **can't** move, and
`Diverter::resolve()` shouldn't pretend otherwise.

## New C++ element

```cpp
enum class DiverterFaultKind { Blocked };
```

Notice: this is a **separate** type from `SensorFaultKind` (`Missing`/`Stale` — what until recently
was just called `FaultKind`, before this module split the name into `SensorFaultKind` so its scope is
explicit in the name itself). If the diverter shared one common fault type with the sensors, nothing
would stop a call like "blocked presence sensor" or "missing diverter" — combinations with no meaning
at all, which would still compile. A separate type means a mistake like that simply doesn't get past
the compiler.

```cpp
void resolve(std::optional<DiverterFaultKind> fault = std::nullopt);
```

## The precise rule

When `fault == DiverterFaultKind::Blocked`, `resolve()` **does nothing** — `actual_` doesn't change at
all, not even toward `Moving`. Otherwise, behavior is identical to before — all the three-state logic
from Module 2 stays unchanged, just preceded by this one check.

`isSettled()` **needs no change at all**. It already compares `actual_` against the target of the
*current command* — and that's exactly what's needed to correctly report "not settled" for a diverter
frozen mid-transition by the block.

## What you already have

[`include/psm/diverter_fault_kind.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/include/psm/diverter_fault_kind.hpp) — the type is
ready.

[`include/psm/diverter.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/include/psm/diverter.hpp) — the `resolve` signature is already
updated.

[`src/diverter.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/src/diverter.cpp) — all the existing three-state logic is present and
unchanged; only the `Blocked` check at the very start is missing (`// TODO`).

## What you need to write

Add a check for `fault == DiverterFaultKind::Blocked` at the start of `resolve()`, before anything
else runs.

## Check your work

```bash
ctest --preset test -L misja-25
```

Expected result: `100% tests passed, 0 tests failed out of 1`. The test checks that with `Blocked`,
`resolve()` does nothing (including freezing exactly in the `Moving` state, if the fault appears
mid-transition), and that ordinary, fault-free behavior stays exactly as in Module 2.

## Common mistakes

- **Checking `fault.has_value()` instead of comparing against `DiverterFaultKind::Blocked`** — right
  now this has the same effect (there's only one value), but comparing against the specific value is
  clearer and robust to the type being extended later.
- **Placing the `Blocked` check after the existing logic**, instead of before it — the diverter would
  then get to move one step before the block stops it.

## Something to think about

`SensorFaultKind` has two values (`Missing`, `Stale`); `DiverterFaultKind` currently has only one
(`Blocked`). Why is it still worth defining it as a separate `enum`, rather than, say, a `bool
isBlocked`?

**Next:** [Mission 26: the routing deadline](./02_termin_rutowania.en.md).
