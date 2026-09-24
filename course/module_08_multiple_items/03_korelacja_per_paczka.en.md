[🇵🇱 Polski](03_korelacja_per_paczka.md) | 🇬🇧 English

# 8.3 Per-parcel correlation

## Problem

`ControllerState` assumed that at most one parcel was "mid-confirmation" at any given moment. That
assumption held while `Plant` carried one parcel. It's false now: a presence reading and a weight
reading in the same tick can concern completely different parcels — one in `presenceCheck`, another in
`weighing`. One global state variable can't express that correctly.

## New C++ elements

```cpp
void updatePresenceConfirmation(Item& itemAtPresenceCheck, PresenceReading presence);
void updateClassification(Item& itemAtWeighing, WeightReading weight);
```

Two functions, not one. A presence reading always concerns whichever parcel is currently in
`presenceCheck`. A weight reading always concerns whichever parcel is currently in `weighing` — and
uses **its own** `presenceConfirmed`, not some global one. That's the real lesson of this mission: with
several parcels in flight, no sensor reading may silently "leak" onto the wrong parcel.

## The precise rule

`updatePresenceConfirmation` sets `itemAtPresenceCheck.presenceConfirmed` to `true` if and only if the
given reading has both `status == ReadingStatus::Ok` and `occupied == true`. In every other case — a
bad reading, or `occupied == false` — the field is left unchanged; the function never resets
`presenceConfirmed` back to `false`.

`updateClassification` writes the result of `decideClassification(weight)` into
`itemAtWeighing.classification` only when both hold at once: the parcel in `weighing` already has
`presenceConfirmed == true`, and the given weight reading has `status == ReadingStatus::Ok`. If either
condition is missing, nothing is written — `classification` stays whatever it was before (for a fresh
parcel: `std::nullopt`).

`Engine::step()` (Mission 32) will call each of these only when the corresponding slot is actually
occupied — there's no "reset when there's no parcel" branch here, because an `Item` created by
`spawnItem` already starts with a clean default state (`presenceConfirmed = false`,
`classification = std::nullopt`) — there's nothing to clear.

## Simplifying the sensors

Since `PresenceSensor::read`/`WeightSensor::read` now get passed exactly the slot they're physically
attached to, the actual state is simply `item.has_value()` — the old comparison against
`item->zone == Zone::PresenceCheck` isn't just unnecessary anymore, it's impossible (`Item` no longer
has `zone`). The rest of the logic from Module 6 stays unchanged, and this matters: when a slot is
empty and there's no fault, the sensor still returns `{Ok, 0}`/`{Ok, false}`, **without** updating
`lastKnownMass_`/`lastKnownOccupied_`. A confirmed empty reading is a genuine, trusted observation — it
is never treated as "the mass is zero" for memory purposes.

## What you already have

[`include/psm/controller.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/include/psm/controller.hpp) — declarations for both new
functions, ready. [`include/psm/presence_sensor.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/include/psm/presence_sensor.hpp)/[`weight_sensor.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/include/psm/weight_sensor.hpp)
— signatures unchanged.

## What you need to write

- `updatePresenceConfirmation`/`updateClassification` in [`src/controller.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/controller.cpp)
  according to the precise rule above.
- `PresenceSensor::read` in [`src/presence_sensor.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/presence_sensor.cpp) and
  `WeightSensor::read` in [`src/weight_sensor.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/weight_sensor.cpp): replace the actual-state
  check with `item.has_value()` (and `item->mass` for weight), keeping the rest of Module 6's logic
  unchanged.

## Check your work

```bash
ctest --preset test -L misja-31
```

Expected result: `100% tests passed`. Checks both correlation functions directly on bare `Item`s,
including that two different, simultaneously-processed parcels get fully independent classification;
the simplified logic of both sensors, including that a confirmed empty reading never clears memory.

## Common mistakes

- **Calling `updateClassification` regardless of that specific parcel's `presenceConfirmed`** — this is
  exactly the global-state bug this mission teaches you to avoid.
- **Going back to comparing `item->zone`** — this won't compile, but it's worth noticing why: `Item`
  no longer carries that information.
- **Updating `lastKnownMass_`/`lastKnownOccupied_` on a confirmed empty reading** — exactly the
  regression this section explicitly warns against.

## Something to think about

If `updateClassification` took `bool presenceConfirmed` as a separate parameter instead of reading it
from `itemAtWeighing`, it would give exactly the same result today. Why is reading it directly from the
parcel still the better design choice here?

**Next:** [Mission 32: the engine with multiple parcels](./04_silnik_z_wieloma_paczkami.en.md).
