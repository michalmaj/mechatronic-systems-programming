[🇵🇱 Polski](06_pierwszy_przebieg.md) | 🇬🇧 English

# 1.6 First run

## Problem

Everything you've written so far has been checked for you by tests — running quietly, with no
visible output. Time to see it with your own eyes: write a program that creates one parcel, runs it
through the system, and shows on the console what happens to it, step by step.

## New C++ elements

**`main()`** — the program's entry point. You already saw it in `toolchain_check` (Module 0) and in
the ready-made preview of `simulator_cli`, but this is the first time you write it yourself, for your
own code.

**`std::cout`** — printing text to the console, the same mechanism as in `toolchain_check`.

## What you already have

[`apps/simulator_cli/main.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/apps/simulator_cli/main.cpp):

```cpp
#include <iostream>

#include <psm/controller.hpp>
#include <psm/plant.hpp>

int main() {
    // TODO (Mission 6: pierwszy_przebieg): ...
    return 0;
}
```

## What you need to write

In `main()`:

1. Create a `Plant` and add one parcel to it via `spawnItem` — pick any mass (e.g. `750` grams, to
   see it route through `OutputHeavy`, or something below `500`, to see `OutputLight`).
2. Write a loop (e.g. for 6 ticks) that, on each iteration: if `plant.item` has a value, computes
   `WeightClass` and `DiverterPosition` (exactly as in `runTicks` from Mission 5), calls
   `advance(plant, ...)`, and then **prints** the tick number and the parcel's current zone (or a
   string like `"empty"`, if `plant.item` no longer has a value). To turn a zone into text, use
   `psm::toString(zone)` from Mission 1 — remember `#include <psm/zone.hpp>`.

This loop **deliberately doesn't call `runTicks`** from Mission 5 — `runTicks` doesn't print anything
(it's pure, easy to test), and here you specifically need printing after every step. Write your own,
small loop in `main()`, using the same `classify`/`toDiverterPosition`/`advance` as before.

An example shape for one line of output (the exact format is up to you):

```
tick 0: zone=PresenceCheck
tick 1: zone=Weighing
...
tick 4: empty
```

## Check your work

First, a smoke test — it only checks that the program starts and exits without an error:

```bash
ctest --preset test -L misja-6
```

But that's not enough — actually run the program and **read the output**:

```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

(on Windows: `.\build\dev\apps\simulator_cli\Debug\simulator_cli.exe`, the exact path depends on your
IDE's generator).

Check that you see a sensible sequence of zones, ending with reaching `OutputLight` or `OutputHeavy`
(depending on the mass you chose), followed by `empty`.

## End of module — full test suite

Now that all six missions are done, run the whole suite at once, with no filter:

```bash
ctest --preset test
```

Expected result: **all tests pass** (`100% tests passed`).

## Save your work

If you haven't already done so along the way:

```bash
git status
git add <files you changed>
git commit -m "..."
```

## Common mistakes

- **Calling `plant.item->mass` without checking `has_value()`** — same as in Mission 5, this is
  undefined behavior when the conveyor is empty.
- **The program exits immediately with no output at all** — check whether you're really printing
  something inside the loop, and not just once at the end (or not at all).
- **Missing `#include <psm/zone.hpp>`** when trying to use `psm::toString` — `plant.hpp` doesn't pull
  it in automatically in any way you should rely on; include it explicitly.

## Something to think about

Look at the code in `main()` and at `runTicks` from Mission 5 side by side. What exactly repeats
between them, and what's different? Does that bit of duplication bother you — and why (or why not) is
it acceptable at this stage of the project?

## End of Module 1

You now have a working, complete (if small) system: a parcel moving through zones and sorted by
weight, with state kept separate from decisions. This is the same architecture this project arrived
at right at the very start of its history — in the modules that follow you'll extend it with more
elements: actuators with their own state, operating modes, safety, sensors, and faults.
