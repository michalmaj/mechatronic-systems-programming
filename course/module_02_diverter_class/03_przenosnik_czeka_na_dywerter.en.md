[🇵🇱 Polski](03_przenosnik_czeka_na_dywerter.md) | 🇬🇧 English

# 2.3 The conveyor waits for the diverter

## Problem

You already have `DiverterCommand` (Mission 7) and a working `Diverter` (Mission 8). But `Plant`
still knows nothing about them — in Module 1 it received a ready-made position and routed the parcel
immediately. Now that the diverter can need several ticks to get into position, `Plant` has to respect
that: if a parcel has reached `Diverting` and the diverter hasn't settled yet, the parcel **waits**.

## New C++ elements

**Passing a class object by `const&`** — `Plant::advance` now takes `const Diverter& diverter`
instead of a raw `DiverterPosition` value. `const&` means: access to that specific object, without
copying it, and without the right to change it — `Plant` only **asks** the diverter about its state,
it never modifies it.

**Calling member methods** — instead of comparing a raw enum value, you call `diverter.isSettled()`
and `diverter.actualPosition()`.

## Minimal ordering of one tick

This still isn't the final simulation engine (we'll get there in later modules) — but something has
to fix the ordering, or the tests would become ambiguous. This module uses exactly this ordering,
followed by both `runTicks` and `main()`:

1. **Controller decision** — if `plant.item` has a value: `classify(mass)`, then
   `toDiverterCommand(...)`.
2. **`diverter.setCommand(...)`** — tell the diverter what we want now.
3. **`diverter.resolve()`** — the diverter takes its one physical step for this tick.
4. **`advance(plant, diverter)`** — `Plant` reacts to the diverter's state **after** that
   `resolve()`, not from before it.
5. **Observation** (only in `main()`) — printing the result; `runTicks` stays a pure function with no
   printing, just as in Module 1.

The order of steps 3 and 4 isn't arbitrary: if you swapped them, `Plant::advance` would see the
diverter's position from before this tick instead of the current one — every routing decision would
silently shift by one tick.

## What you already have

In [`src/plant.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-02-start/src/plant.cpp) the `Infeed`/`PresenceCheck`/`Weighing` branches (moving
forward via `advanceZone`) and the `OutputLight`/`OutputHeavy` branches (clearing `plant.item`) are
already in place and working — this is the logic from Module 1, unchanged. Only the `Diverting`
branch is missing:

```cpp
case Zone::Diverting:
    // TODO (Mission 9: przenosnik_czeka_na_dywerter): if !diverter.isSettled(), the parcel
    // waits (do nothing -- it stays in Diverting). If diverter.isSettled(), route the
    // parcel to OutputLight or OutputHeavy based on diverter.actualPosition().
    (void)diverter;
    return;
```

`src/loop.cpp` and `apps/simulator_cli/main.cpp` have empty skeletons for their whole functions —
both now need their own local `Diverter` object, driven according to the ordering described above.

## What you need to write

1. **The `Diverting` branch in `src/plant.cpp`** — implement what the TODO comment says.
2. **`runTicks` in `src/loop.cpp`** — for each of the `tickCount` ticks, carry out the five steps
   from the "Minimal ordering" section (without step 5 — `runTicks` doesn't print anything).
3. **`main()` in `apps/simulator_cli/main.cpp`** — create a `Plant` and a `Diverter`, add a parcel via
   `spawnItem`, and in a loop (e.g. 8 ticks) carry out all five steps, printing the tick number, the
   parcel's zone (`psm::toString`), and `diverter.actualPosition()`.

## Why the provided test requires a delayed command

If the Controller decides **right away**, from tick 0 (as `runTicks` does), the diverter always has
enough time to settle before the parcel even reaches `Diverting` — 3 ticks of travel are always
enough for 2 ticks of settling. In that scenario the waiting is never visible from outside! That's
why the provided test, besides checking the ordinary, "eager" path (exactly the same arithmetic as in
Module 1: 4 ticks to `OutputHeavy`), also includes a scenario where the `Divert` command is issued
**only once the parcel is already waiting in `Diverting`** — that's the only way to actually see the
conveyor waiting for the device.

## Check your work

```bash
ctest --preset test -L misja-9
```

Expected result: `100% tests passed, 0 tests failed out of 1`.

Also run the program for real:

```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

## End of module — full test suite

```bash
ctest --preset test
```

Expected result: all tests green — `misja-1` through `misja-4`, `misja-6` through `misja-9`.

## Save your work

```bash
git status
git add <files you changed>
git commit -m "..."
```

## Common mistakes

- **Swapped order of `resolve()`/`advance()`** — see the tick-ordering section above; this is the
  easiest mistake to make in this mission, and the only one that shifts the results by exactly one
  tick.
- **`runTicks` without its own `Diverter`** — the diverter has to live for the whole duration of the
  loop (so it remembers its state between ticks); it can't be recreated fresh on every iteration.
- **Checking `diverter.isSettled()` before `resolve()`** instead of after — remember, `Plant::advance`
  looks at the diverter's state **after** that step.

## Something to think about

The provided test deliberately uses two different scenarios — "eager" and "delayed" — just to be able
to observe the waiting at all. Would an integration test based only on `runTicks` (which always
decides eagerly) be enough to catch a bug in your implementation of the `Diverting` branch? Why, or
why not?

## End of Module 2

You now have a system where a physical device truly has its own reaction time, and the conveyor
respects it. This is the same step this project itself went through very early in its own history —
later modules will add more pieces: a second actuator (the conveyor's motor), the system's operating
modes, and safety.
