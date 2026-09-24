[🇵🇱 Polski](02_termin_rutowania.md) | 🇬🇧 English

# 7.2 The routing deadline

This is the theoretically most important mission in this module. Read it in full before you start
writing code.

## Problem

`Plant` waits in the `Diverting` zone for the diverter to settle — today with no time limit at all,
and no way for anyone to find out it's been waiting too long. The blocked diverter from Mission 25
freezes a parcel forever, silently.

## New C++ element

```cpp
enum class SystemEventKind { DiverterNotReady, RoutingDeadlineMissed };
```

We're introducing this type right here, where it's immediately put to use — not as a separate, empty
mission on its own.

`Plant` gains a counter:

```cpp
struct Plant {
    std::optional<Item> item;
    int divertingWaitTicks = 0;
};
```

`advance()` changes its return type from `void` to `std::optional<SystemEventKind>` — a free,
backward-compatible change: every existing call (`plant_test.cpp`, `plant_diverter_test.cpp`,
`loop.cpp`, `Engine::step()`) calls `advance(...)` as a standalone statement, discarding the result —
and that stays legal C++ regardless of the return type. None of those calls need editing.

## The precise rule

In the `Diverting` branch:

- If `!routingReady`: **reset `divertingWaitTicks` to `0`** and return `std::nullopt`. **Don't**
  freeze the counter — see below for why that matters.
- If `routingReady`, but `!diverter.isSettled()`: increment `divertingWaitTicks`; return
  `DiverterNotReady` if the counter is still `<= 1`, `RoutingDeadlineMissed` if the counter has gone
  above `1`.
- If settled: route normally, reset the counter to `0`.

The counter also resets on the `Weighing` → `Diverting` transition — a fresh parcel starts with a
clean counter, regardless of what was left over from the previous one.

## Why the deadline measures only a *continuous, active* attempt

This is the heart of this mission. Imagine `!routingReady` **froze** the counter instead of resetting
it. Then a pause unrelated to the diverter itself — an e-stop, the mode, a classification that isn't
available yet — would silently eat into the diverter's own time budget. A parcel could get
`RoutingDeadlineMissed` not because the diverter was genuinely stuck, but because something completely
different had paused routing attempts a few ticks earlier. That would mix two completely different
causes into one signal.

Resetting on `!routingReady` guarantees the deadline measures exactly what it's supposed to: how many
ticks in a row the diverter got a **real** chance to settle and didn't take it.

## What you already have

[`include/psm/system_event_kind.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/include/psm/system_event_kind.hpp) — the type is ready.

[`include/psm/plant.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/include/psm/plant.hpp) — `divertingWaitTicks` and the new `advance()`
signature are already there.

[`src/plant.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/src/plant.cpp) — the `Infeed`/`PresenceCheck`/`Weighing`/`Output*` branches
are complete and unchanged (including resetting the counter on `Weighing`→`Diverting`); only the
`Diverting` branch's own logic is `// TODO`.

## What you need to write

Fill in the `Diverting` branch of `advance()` according to the precise rule above.

## Check your work

```bash
ctest --preset test -L misja-26
```

Expected result: `100% tests passed, 0 tests failed out of 1`. The test checks: `DiverterNotReady` on
the first failed active attempt, `RoutingDeadlineMissed` on the second in a row; that `!routingReady`
resets the counter instead of freezing it (and that routing resumed after such a pause gets a full,
fresh deadline); and that a settled diverter routes normally and resets the counter.

## Common mistakes

- **Freezing the counter instead of resetting it on `!routingReady`** — exactly the problem described
  above.
- **Returning `RoutingDeadlineMissed` already on the first failed attempt** — check the ordering
  carefully: `<= 1` is `DiverterNotReady`, only `> 1` is missing the deadline.
- **Forgetting to reset the counter after a successful route** — the next parcel would inherit
  someone else's partially used counter.

## Something to think about

The effective grace period is one tick (`DiverterNotReady` at count `<= 1`, `RoutingDeadlineMissed`
only at count `> 1`). A fault-free diverter needs at most two `resolve()` calls to settle from any
state. Trace through why this means no existing, fault-free scenario from Modules 1–6 will ever report
`RoutingDeadlineMissed`.

**Next:** [Mission 27: fault mode](./03_tryb_awarii.en.md).
