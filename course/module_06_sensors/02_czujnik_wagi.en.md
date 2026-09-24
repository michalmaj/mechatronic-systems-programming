[🇵🇱 Polski](02_czujnik_wagi.md) | 🇬🇧 English

# 6.2 The weight sensor

## Problem

The scale has exactly the same physical limitation as the presence sensor from the previous mission —
except it's mounted somewhere else: it sees a parcel only in the `Weighing` zone.

## New C++ element

**`class WeightSensor`** — the second application of the pattern from Mission 20, this time for
`Grams` instead of `bool`, less guided.

```cpp
class WeightSensor {
public:
    WeightReading read(const std::optional<Item>& item, std::optional<FaultKind> fault);

private:
    std::optional<Grams> lastKnownMass_;
};
```

**Actual state:** `item.has_value() && item->zone == Zone::Weighing` → `item->mass`.

The rule is structurally identical to `PresenceSensor`:
- No fault, on the scale: store it in `lastKnownMass_`, return `{Ok, item->mass}`.
- No fault, not on the scale: return `{Ok, 0}` — there's nothing here to weigh right now; **don't**
  update memory (zero isn't the parcel's real mass, just the absence of a measurement).
- `Missing`: `{Missing, 0}`, memory untouched.
- `Stale`: `{Stale, *lastKnownMass_}` if memory has a value, otherwise `{Missing, 0}`.

## What you already have

[`include/psm/weight_sensor.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/include/psm/weight_sensor.hpp) — declaration complete.

[`src/weight_sensor.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/src/weight_sensor.cpp) — an empty skeleton.

## What you need to write

Fill in the body of `WeightSensor::read` according to the rule above.

## Check your work

```bash
ctest --preset test -L misja-21
```

Expected result: `100% tests passed, 0 tests failed out of 1`.

## Common mistakes

- The same ones as Mission 20: updating memory from an "off the scale" reading; not degrading `Stale`
  to `Missing` when there's no prior history.
- **Confusing "zero grams" with "no measurement"** — `{Ok, 0}` for an absent parcel is a correct,
  current reading, but it should never land in `lastKnownMass_` as a "real" mass.

## Something to think about

If someone proposed that `WeightSensor` shouldn't check the zone at all — just return `item->mass`
whenever a parcel exists, regardless of where — what exactly would that break in this module's later
missions?

**Next:** [Mission 22: fault-tolerant classification](./03_klasyfikacja_odporna_na_awarie.en.md).
