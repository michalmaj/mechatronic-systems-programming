[🇵🇱 Polski](03_klasyfikacja_odporna_na_awarie.md) | 🇬🇧 English

# 6.3 Fault-tolerant classification

## Problem

`classify(mass)` (Module 1) still trusts the number it's given unconditionally. Now that this number
comes from a sensor that can report `Missing` or `Stale`, we need a layer in between that rejects an
untrustworthy reading before it ever reaches `classify`.

## New C++ element

```cpp
std::optional<WeightClass> decideClassification(WeightReading weight);
```

Deliberately only **one** sensor as input — we're not yet trying to combine the weight reading with
presence confirmation (that's Mission 23's problem, and a considerably harder one). This mission
teaches one isolated thing: don't classify based on a reading you can't trust.

## The rule

- If `weight.status != ReadingStatus::Ok`: return `std::nullopt` — no matter how plausible the number
  in `weight.grams` looks.
- Otherwise: return `classify(weight.grams)`.

## What you already have

[`include/psm/controller.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/include/psm/controller.hpp) — the `decideClassification`
declaration has already been added, alongside the existing `classify`/`toDiverterCommand` (those two
stay unchanged).

[`src/controller.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/src/controller.cpp) — an empty `decideClassification` skeleton with a
`// TODO` comment; `classify`/`toDiverterCommand` already work correctly.

## What you need to write

Fill in the body of `decideClassification` according to the rule above.

## Check your work

```bash
ctest --preset test -L misja-22
```

Expected result: `100% tests passed, 0 tests failed out of 1`. The test deliberately includes a
`{Stale, 750}` case — the number looks entirely plausible, but the status says clearly: don't trust
it.

## Common mistakes

- **Checking `weight.grams` instead of `weight.status`** as the signal to reject — that's the exact
  opposite of what this function is for.

## Something to think about

Why doesn't this mission accept `PresenceReading` as a second argument yet, given that the real
problem (Mission 23) will need both sensors? What exactly would go wrong if we tried to combine both
sensors in this same function, called within a single tick?

**Next:** [Mission 23: the controller's decision memory](./04_pamiec_decyzji_sterownika.en.md).
