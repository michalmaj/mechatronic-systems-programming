[🇵🇱 Polski](01_polecenie_a_rzeczywistosc.md) | 🇬🇧 English

# 2.1 Command vs. actual state

## Problem

In Module 1, Controller returned a `DiverterPosition` directly — the function `toDiverterPosition`
answered with a single fact: "the diverter SHOULD BE at position X." That worked because the diverter
always got there instantly anyway. But now that the diverter **needs time** to move into position,
that same name stops making sense: `DiverterPosition` should describe where the diverter **actually is
right now**, not where the controller would like it to be.

In other words: "what we want" and "what physically exists" are two different things, and conflating
them was a mistake this module fixes.

## New C++ element

**`enum class DiverterCommand`** — a command, a request, not a physical state:

```cpp
enum class DiverterCommand { HoldStraight, Divert };
```

This is the only new piece of syntax in this mission — the enum itself is no different from the ones
you already know from Module 1. What's new is the **concept**, not the syntax: from now on, two
separate types exist side by side in the code, `DiverterCommand` (the request) and `DiverterPosition`
(the actual state), and they must never be confused with each other.

## What you already have

[`include/psm/diverter_command.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-02-start/include/psm/diverter_command.hpp) — `enum class
DiverterCommand` is already defined, complete.

[`include/psm/controller.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-02-start/include/psm/controller.hpp) declares:

```cpp
WeightClass classify(Grams mass);
DiverterCommand toDiverterCommand(WeightClass weightClass);
```

Note: `classify` **doesn't change** — the 500g threshold still works exactly as it did in Module 1
(the `misja-4` test confirms this, and it still passes). Only the second function changes — it now
returns `DiverterCommand`, not `DiverterPosition`.

[`src/controller.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-02-start/src/controller.cpp) has an empty skeleton for `toDiverterCommand`:

```cpp
DiverterCommand toDiverterCommand(WeightClass weightClass) {
    // TODO (Mission 7: polecenie_a_rzeczywistosc): map Light -> HoldStraight, Heavy -> Divert.
    (void)weightClass;
    return DiverterCommand::HoldStraight;
}
```

## What you need to write

Fill in `toDiverterCommand` so that:
- `WeightClass::Light` produces `DiverterCommand::HoldStraight` (a light parcel: the diverter stays
  straight),
- `WeightClass::Heavy` produces `DiverterCommand::Divert` (a heavy parcel: the diverter should
  divert).

## Check your work

```bash
ctest --preset test -L misja-7
```

Expected result: `100% tests passed, 0 tests failed out of 1`.

## Common mistakes

- **Swapped directions** — double-check which value maps to which (Light→HoldStraight,
  Heavy→Divert); the test checks both directions.
- **Reaching for the old `toDiverterPosition`** — that function no longer exists in this module; if
  your editor autocompletes it from old memory or from another file, that's a sign something got mixed
  up.

## Something to think about

`classify` stays unchanged, but the function that turns its result into a diverter action changes both
its name and its return type. Why is that the right place for this change, rather than, say, inside
`classify` itself?

**Next:** [Mission 8: the diverter as a class](./02_dywerter_jako_klasa.en.md).
