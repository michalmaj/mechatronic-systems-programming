[🇵🇱 Polski](05_petla_sterowania.md) | 🇬🇧 English

# 1.5 Control loop

## Problem

You already have all the pieces: `Plant` represents state, the Controller (`classify` +
`toDiverterPosition`) makes the decision, `advance` moves the parcel one step, taking that decision
into account. But so far you've called all of this by hand, one call at a time, in tests. A real
system doesn't take a single step — it repeats the same cycle (look at the state → decide → act)
over and over, until something ends it.

## New C++ elements

**`for`** — a loop that repeats a block of code a fixed number of times. Here: "run one simulation
step, `tickCount` times in a row".

```cpp
for (int i = 0; i < tickCount; ++i) {
    // one simulation step
}
```

Nothing else is new in this mission — the whole challenge is correctly **assembling** elements you
already know, in one place.

## What you already have

[`include/psm/loop.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/include/psm/loop.hpp):

```cpp
void runTicks(Plant& plant, int tickCount);
```

[`src/loop.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/src/loop.cpp) has an empty skeleton with a `// TODO` comment.

## What you need to write

Fill in `runTicks` so that for each of the `tickCount` "ticks" it does:

1. if `plant.item` has a value — compute `WeightClass` via `classify(plant.item->mass)`, then
   `DiverterPosition` via `toDiverterPosition(...)` on that result,
2. call `advance(plant, diverterPosition)`, where `diverterPosition` is that just-computed value (if
   `plant.item` is empty, `advance` won't do anything anyway, so you can pass any value, e.g.
   `DiverterPosition::Straight`, when there's no parcel).

In other words: `runTicks` **wires together** what you previously tested separately — the Controller
and `Plant::advance` — and does it `tickCount` times in a row.

## Check your work

```bash
ctest --preset test -L misja-5
```

Expected result: `100% tests passed, 0 tests failed out of 1`. The test creates a `Plant` with a
single 750-gram parcel, calls `runTicks(plant, 4)`, and checks that the parcel reached `OutputHeavy`,
and that the next tick clears it from the system.

## Still "Controller", not a class

We call `classify` + `toDiverterPosition` together the "Controller" — a name for a pair of
cooperating functions, not a hint that a class is coming. The same lack of need for encapsulation we
discussed for `Plant` in Mission 3 applies here too: neither function has any state of its own to
protect.

## Common mistakes

- **Calling `classify`/`toDiverterPosition` when `plant.item` is empty** — `plant.item->mass` on an
  empty `std::optional` is undefined behavior. Check `has_value()` first.
- **A `for` loop with the wrong stop condition** (`<=` instead of `<`) — it will run one time too
  many.
- **Calling `advance` only once, outside the loop** — the whole point of this mission is *repeating*
  the step, not performing it once.

## Something to think about

This mission's test only checks `Plant`'s state after a series of ticks — it never sees what happens
"inside" along the way. Is that a problem, or a deliberate feature of a test like this?

**Next:** [Mission 6: first run](./06_pierwszy_przebieg.en.md).
