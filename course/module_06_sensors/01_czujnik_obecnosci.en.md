[🇵🇱 Polski](01_czujnik_obecnosci.md) | 🇬🇧 English

# 6.1 The presence sensor

## Problem

A real presence sensor isn't an all-seeing eye watching the whole conveyor at once — it's a device
mounted at one specific point. Our presence sensor "sees" a parcel only when it's in the
`PresenceCheck` zone. Anywhere else — even if the parcel physically exists somewhere on the conveyor —
this particular sensor sees nothing there.

## New C++ element

**`class PresenceSensor`** — the fourth class in the course, and the first with a completely different
invariant than `Diverter`/`BeltMotor` (command-vs-actual-state). Here the invariant reads: **never
pretend to remember something you never actually observed.**

```cpp
class PresenceSensor {
public:
    PresenceReading read(const std::optional<Item>& item, std::optional<FaultKind> fault);

private:
    std::optional<bool> lastKnownOccupied_;
};
```

Notice: `lastKnownOccupied_` is `std::optional<bool>`, not a bare `bool`. If it were a plain `bool`,
it would need some initial value (`false`?) — but `false` would mean both "the sensor genuinely
observed no parcel" and "the sensor hasn't observed anything yet" — two completely different
situations that a `bool` can't distinguish. `std::optional<bool>` solves this directly: an empty
`std::optional` means "I have never seen anything trustworthy."

## The precise rule

**Actual state** (the physical truth this sensor could observe at all):
`item.has_value() && item->zone == Zone::PresenceCheck`.

- **No fault, parcel in `PresenceCheck`:** store it in `lastKnownOccupied_` and return `{Ok, true}`.
- **No fault, parcel elsewhere (or absent):** return `{Ok, false}` — this is a genuine, current reading
  ("there's nothing here right now"), but **it is not a value worth remembering** — don't update
  `lastKnownOccupied_`.
- **`Missing`:** return `{Missing, false}`, leave memory untouched.
- **`Stale`:** if `lastKnownOccupied_` has a value, return `{Stale, *lastKnownOccupied_}`. **If it
  doesn't** (there was never a trustworthy reading to repeat), return `{Missing, false}` — `Stale`
  degrading to `Missing` when there's no history.

## What you already have

[`include/psm/reading_status.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/include/psm/reading_status.hpp),
[`include/psm/sensor_snapshot.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/include/psm/sensor_snapshot.hpp),
[`include/psm/fault_kind.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/include/psm/fault_kind.hpp),
[`include/psm/fault_target.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/include/psm/fault_target.hpp) — all types ready.

[`include/psm/presence_sensor.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/include/psm/presence_sensor.hpp) — the class declaration is
complete, as above.

[`src/presence_sensor.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/src/presence_sensor.cpp) — an empty skeleton with a `// TODO`
comment.

## What you need to write

Fill in the body of `PresenceSensor::read` according to the precise rule above.

## Check your work

```bash
ctest --preset test -L misja-20
```

Expected result: `100% tests passed, 0 tests failed out of 1`. The test deliberately checks that an
"elsewhere" reading (a parcel in `Weighing`, not `PresenceCheck`) does **not** corrupt a previously
remembered good reading — only afterward does `Stale` repeat that earlier value.

## Common mistakes

- **Updating `lastKnownOccupied_` to `false` when the parcel is elsewhere** — this is exactly the bug
  this test catches. Memory only updates from a reading taken **in** `PresenceCheck`.
- **`Stale` returning some default value** instead of degrading to `Missing` when `lastKnownOccupied_`
  is empty.
- **Checking `item.has_value()` without checking the zone** — the mere fact that a parcel exists
  somewhere in the system isn't enough; it has to be specifically in `PresenceCheck`.

## Something to think about

`Diverter` (Module 2) and `PresenceSensor` (this mission) are both classes with private state. What's
different about the **kind** of invariant each one protects? Try putting it in one sentence for each.

**Next:** [Mission 21: the weight sensor](./02_czujnik_wagi.en.md).
