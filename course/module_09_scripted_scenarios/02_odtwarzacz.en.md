[🇵🇱 Polski](02_odtwarzacz.md) | 🇬🇧 English

# 9.2 The replayer

## Problem

A validated `Scenario` is just data. Something has to turn it into a `TickResult` trace, driving a
real `Engine`, in the right order, on every tick.

## New C++ element

```cpp
std::optional<std::vector<TickResult>> runScenario(const Scenario& scenario);
```

It builds a **fresh `Engine` internally** — it doesn't take an `Engine&`. That's exactly what makes
`Scenario` a genuinely reproducible experiment: the same `Scenario` value always starts from the same,
clean state.

`activeSensorFault`/`activeDiverterFault` — functions that check whether a given fault is active on a
given tick — are private implementation details of `runScenario`, living in an anonymous namespace in
`src/scenario.cpp`. They aren't declared in `scenario.hpp`: making them public would require
documenting and maintaining a separate contract ("only works correctly for already-validated,
non-overlapping schedules") that nothing currently needs.

## The fixed tick order

**arrivals → operator inputs → fault state → `Engine::step()` → collect the result.**

Fault state is recomputed from scratch on every tick and applied unconditionally through
`injectSensorFault`/`clearSensorFault`/`injectDiverterFault`/`clearDiverterFault` — there's no need to
track "was it active before," because those four `Engine` methods are already idempotent.

## Signaling failure — through the return type, not `assert`

`assert()` disappears in Release builds (`NDEBUG`), and an invalid `Scenario` is a real, API-level
condition to handle, not a programmer error to ignore in Release. Two cases, both `std::nullopt`:
- The `Scenario` is statically invalid (`isValidScenario` returns `false`) — checked once, before the
  first tick.
- A scheduled arrival fails during replay (`Plant::infeed` is still occupied by an earlier parcel that
  hasn't departed yet). This is **not** statically detectable in general (it depends on the belt/mode
  state resulting from `operatorInputs`) and is **not** retried — it's a scenario failure, reported the
  same way as a static invalidity.

## The reproducibility contract

**For any valid `Scenario` that replays to completion, two separate calls to
`runScenario(scenario)` produce `TickResult` sequences that are semantically identical, field by
field.** ("Field by field," not "byte by byte" — this is a claim about values, not about the C++
objects' memory representation.) This follows directly from `runScenario` building a fresh `Engine` on
every call, and from the fundamental, by-design determinism guarantee (no system clock, no randomness,
no hidden global state — true since Module 0).

## Why `{EmergencyStopReleased, Reset}` on the same tick is safe

This follows from the behavior of the real, unchanged `nextEStopLatchState` (Module 5): setting
`released` and `resetRequested` at the same time, starting from `Engaged`, hits the
`previous == Engaged` branch (checked first) — `resetRequested` isn't consulted at all by that call.
The latch lands in `Armed`, not `Released`. The pair is allowed because it's safe and well defined —
not because it shortcuts the two-step recovery you know from Module 5.

## What you already have

[`include/psm/scenario.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/include/psm/scenario.hpp) — the `runScenario` signature is ready.
`isValidScenario` from Mission 33 is already yours.

## What you need to write

Fill in the body of `runScenario` in [`src/scenario.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/src/scenario.cpp), including the
private `activeSensorFault`/`activeDiverterFault`, according to the fixed tick order above.

## Check your work

```bash
ctest --preset test -L misja-34
```

Expected result: `100% tests passed`. The test builds the same short sequence twice — once
imperatively, once through `Scenario`+`runScenario` — and compares the results field by field, instead
of guessing at a new trace by hand. It also checks: `{EmergencyStopReleased, Reset}` on the same tick;
an arrival onto an occupied `Infeed` returning `std::nullopt`; a statically invalid scenario returning
`std::nullopt` without running anything; `duration == 0` returning an empty but present trace; and
invariance with respect to element order within the vectors.

## Common mistakes

- **Tracking "was this fault active before"** — unnecessary; `inject*`/`clear*` are idempotent,
  recompute the state from scratch on every tick.
- **Automatically retrying a failed arrival** — exactly what this mission deliberately doesn't do.
- **Making `activeSensorFault`/`activeDiverterFault` public** — they stay private to `scenario.cpp`.

## Something to think about

`runScenario` checks `isValidScenario` once, right at the start. What specifically could go wrong if it
instead checked individual rules "on the fly," during replay, rather than up front?

**Next:** [Mission 35: CLI integration](./03_integracja_w_cli.en.md).
