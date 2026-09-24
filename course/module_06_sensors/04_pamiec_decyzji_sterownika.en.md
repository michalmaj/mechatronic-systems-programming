[🇵🇱 Polski](04_pamiec_decyzji_sterownika.md) | 🇬🇧 English

# 6.4 The controller's decision memory

This is the most important and most difficult mission in this module. It's worth reading all the way
through carefully before you start writing code.

## Problem

`PresenceCheck` and `Weighing` are two different, **sequential** zones. A parcel is never in both at
once — it first passes through `PresenceCheck`, and only later, several ticks after, does it reach
`Weighing`. That means something important: **no single tick can confirm presence and weigh the
parcel at the same time.** A reading from one tick is never enough on its own to trust a
classification — we need to combine **two confirmations from two different moments in time**.

## New C++ element

```cpp
struct ControllerState {
    bool presenceConfirmed = false;
    std::optional<WeightClass> classification;
};

void updateControllerState(ControllerState& state, const std::optional<Item>& item,
                            PresenceReading presence, WeightReading weight);
```

`ControllerState` is a plain `struct`, with no methods — the same choice as `Plant` (Module 1) or
`TickResult` (Module 3): there's nothing here that needs protecting through encapsulation; the caller
(eventually `Engine`) simply stores this value and passes it along, tick after tick.

**Why two fields, not one.** `presenceConfirmed` gets confirmed **earlier** (when the parcel passes
`PresenceCheck`), `classification` **later** (when the parcel passes `Weighing`) — and it only makes
sense to set `classification` once `presenceConfirmed` is already true. These are two independent
confirmations from two independently-failing sensors, confirming each other across time — the same
spirit as the two independent safety paths from Module 5, this time applied to **data**, not
actuators.

## The precise update rule

Called every tick, with the current parcel and fresh readings from both sensors:

1. **If there's no parcel, or it's in `Zone::Infeed`:** reset — `state = ControllerState{}`. A new
   parcel starts with a clean slate; nothing from the previous one can carry over. (Harmless if a
   parcel waits in `Infeed` for a few ticks before the belt starts moving — the reset just repeats,
   always to the same empty state.)
2. **Otherwise, if the parcel is in `Zone::PresenceCheck`** and `presence.status == Ok` and
   `presence.occupied`: set `state.presenceConfirmed = true`.
3. **Otherwise, if the parcel is in `Zone::Weighing`**, `state.presenceConfirmed` is already true, and
   `weight.status == Ok`: set `state.classification = decideClassification(weight)`.
4. **Otherwise:** no change.

Notice the condition in step 3: `classification` is only set if `presenceConfirmed` is **already**
true. If, on the exact tick the parcel was passing `PresenceCheck`, the presence sensor happened to be
faulty — `presenceConfirmed` will never be set for that parcel, and even a perfect weight reading
later **will not produce a classification**.

## What you already have

[`include/psm/controller_state.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/include/psm/controller_state.hpp) — declarations
complete, as above.

[`src/controller_state.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/src/controller_state.cpp) — an empty skeleton with a `// TODO`
comment describing exactly these four steps.

## What you need to write

Fill in the body of `updateControllerState` according to the precise rule above, in this order.

## Check your work

```bash
ctest --preset test -L misja-23
```

Expected result: `100% tests passed, 0 tests failed out of 1`. The test moves a parcel through
`Infeed→PresenceCheck→Weighing` and checks that `presenceConfirmed`/`classification` get set at the
right moments — plus two failure cases: a trustworthy weight reading with no prior presence
confirmation (no classification appears), and confirmed presence but a faulty weight reading (also no
classification).

## Common mistakes

- **Setting `classification` without checking `presenceConfirmed`** — that's exactly the problem this
  mission solves; without that condition, classification would rest on weight alone, ignoring the
  presence sensor entirely.
- **Resetting only on `!item.has_value()`, without `Zone::Infeed`** — if the parcel is already in the
  system (e.g. freshly added by `spawnItem`, still in `Infeed`), the old state from the previous
  parcel has to disappear before this one starts moving.
- **The order of checks** — the reset has to be checked first; otherwise a fresh parcel in `Infeed`
  could accidentally "inherit" the previous parcel's state.

## Something to think about

Imagine the presence sensor's fault lasts exactly one tick, precisely when the parcel is passing
`PresenceCheck` — and then the sensor works correctly again. Will that specific parcel ever get
classified, even if the scale works flawlessly? Trace the rule step by step to make sure.

**Next:** [Mission 24: the engine with sensors](./05_silnik_z_czujnikami.en.md).
