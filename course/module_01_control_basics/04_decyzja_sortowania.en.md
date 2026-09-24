[🇵🇱 Polski](04_decyzja_sortowania.md) | 🇬🇧 English

# 1.4 Sorting decision

## Problem

Since Mission 2 you've had a parcel stuck at `Diverting`. Time to release it from there — but not in
just any direction: light parcels should go to one output, heavy ones to the other. In other words:
you need a **decision**, not just movement.

This is a good moment to name a division of responsibility that will hold for the rest of the module:
**`Plant` describes physical state** (where the parcel is), and **the decision about where to route
it is a separate concern** — we'll collectively call it the "Controller". For now that's just a name
for a couple of functions, not a new class (more on that below).

## New C++ elements

**`enum class WeightClass`** — a named decision result (`Light`/`Heavy`) instead of a bare `bool`. A
`bool` value of `true`/`false` says nothing about *meaning* — you'd have to remember whether `true`
means "light" or "heavy". `WeightClass::Light` says it directly, right at the point of use.

```cpp
enum class WeightClass { Light, Heavy };
```

**A function returning a value based on an `if` with a threshold:**

```cpp
WeightClass classify(Grams mass);
```

**`enum class DiverterPosition`** — the diverter's physical position, which we map the
classification result onto:

```cpp
enum class DiverterPosition { Straight, Diverted };
```

## What you already have

[`include/psm/weight_class.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/include/psm/weight_class.hpp) and
[`include/psm/diverter_position.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/include/psm/diverter_position.hpp) — both `enum class` types are already
complete.

[`include/psm/controller.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/include/psm/controller.hpp) declares two functions:

```cpp
WeightClass classify(Grams mass);
DiverterPosition toDiverterPosition(WeightClass weightClass);
```

[`src/controller.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/src/controller.cpp) has empty skeletons for both, with `// TODO` comments.

`src/plant.cpp`'s `advance` from Mission 3 has a **second** `// TODO (Mission 4: ...)` comment inside
the same function body, which you'll fill in today — this isn't a new function, just an extension of
the one you started in the previous mission.

## What you need to write

**`classify(Grams mass)`** — compare `mass` against a threshold of **500 grams**: below `500` is
`WeightClass::Light`, `500` and above is `WeightClass::Heavy`.

**`toDiverterPosition(WeightClass weightClass)`** — map `Light` to `DiverterPosition::Straight`,
`Heavy` to `DiverterPosition::Diverted`.

**Extend `advance` in `src/plant.cpp`** with the missing part: when the parcel **is** in `Diverting`,
use the passed-in `diverterPosition` parameter to decide whether to move it to `Zone::OutputLight`
(for `Straight`) or `Zone::OutputHeavy` (for `Diverted`). In addition: once the parcel is already in
`OutputLight` or `OutputHeavy`, clear `plant.item` (`std::nullopt`) — the parcel leaves the system.

## Check your work

```bash
ctest --preset test -L misja-4
```

Expected result: `100% tests passed, 0 tests failed out of 1`. The test checks several `mass` values
around the 500g threshold, and both directions of the `WeightClass` → `DiverterPosition` mapping.

It's also worth rerunning Mission 3's test (`ctest --preset test -L misja-3`) — it should still pass,
even though you've added code to that same `advance` function.

## Why "Controller", not a class

We could now wrap `classify` and `toDiverterPosition` in a `Controller` class with methods. At this
stage that would be unnecessary complexity: neither function holds any state of its own between
calls — they take input, return a result, done. "Controller" is, for now, a convenient name for this
pair of functions, not a signal that they need to be locked inside a class. This same pattern —
a decision expressed as a set of stateless functions — is, in fact, exactly what the decision-making
module looks like in the real, much larger project this simulator is based on.

## Common mistakes

- **Using `<=` instead of `<` for the threshold** — exactly 500g must be `Heavy`, not `Light`. The
  test checks this explicitly.
- **Forgetting to clear `plant.item` once it reaches an output zone** — without this, the parcel
  "gets stuck", this time for good, in `OutputLight`/`OutputHeavy`.
- **Changing `advance`'s signature** — the `diverterPosition` parameter has been there since Mission
  3; you don't need to (and shouldn't) add new parameters or change existing ones.

## Something to think about

What would happen if `Plant::advance` called `classify` and `toDiverterPosition` itself, internally,
instead of receiving a ready-made `DiverterPosition` as a parameter? Would that still be a "state vs.
decision" split?

**Next:** [Mission 5: control loop](./05_petla_sterowania.en.md).
