[🇵🇱 Polski](00_wprowadzenie.md) | 🇬🇧 English

# 4.0 Introduction

Module 3 left you with `Engine` as the only place where the order of one tick exists at all. This
module extends `Engine` with two new things: a second actuator (the belt motor, `BeltMotor`) and the
first concept that describes the **whole system**, not a single parcel — the operating mode (`Mode`).

## Where you're starting from

```bash
git fetch --tags
git switch -c <your-branch-name> module-04-start
```

An important difference from how earlier modules were structured: **`Engine::step()` stays exactly as
Module 3 left it, all the way until the last mission.** You first build `BeltMotor` (Mission 13) and
`Mode` (Mission 14) as standalone, separately testable pieces — the same way this project's own
history actually built pieces like these before any integration. Only Mission 15 wires everything
together.

## About the `misja-11` test

You'll notice that the `misja-11` test (inherited from Module 3) looks different from what you might
remember — it now only checks that the tick counter works correctly on an empty `Engine`, nothing
more. This is a deliberate, permanent narrowing: its earlier, detailed contract (exact parcel zones on
specific ticks) assumed instant movement with no gate on the belt at all — something this module
deliberately changes. You don't need to fix anything in that test or understand why it's different
from before — it's simply what it's meant to be now.

## Module map

1. **The belt motor** — `BeltMotor`, the second use of the command-vs-actual-state pattern from
   Module 2.
2. **Operating mode** — `Mode`, the first concept describing the whole system, and the first case
   where we *don't* build a class.
3. **The conveyor under mode control** — the point where everything comes together: a parcel only
   starts moving once the belt is actually `Running`.

## Before you start

- The Module 1–3 tests (`misja-1`–`misja-4`, `misja-6`–`misja-11`) are already present and passing.
- As always: you don't edit test files or `CMakeLists.txt`.

**Next:** [Mission 13: the belt motor](./01_silnik_przenosnika.en.md).
