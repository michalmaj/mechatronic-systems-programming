[🇵🇱 Polski](00_wprowadzenie.md) | 🇬🇧 English

# 6.0 Introduction

Until now, `Controller` has trusted `Item::mass` unconditionally — the parcel's mass was simply the
truth, always available, always correct. This module changes that: the parcel is now weighed by a
real sensor that can fail, and detected by a presence sensor that can also fail.

An important difference from the actuators in earlier modules: a sensor in this module is **not
all-knowing**. It's physically mounted at one specific point on the conveyor — the presence sensor
only sees a parcel in the `PresenceCheck` zone, the weight sensor only in `Weighing`. This isn't an
implementation detail — it's exactly the problem this module teaches you to solve: since these two
zones are different and sequential, no parcel is ever in both at once. That means the classification
computed while weighing has to be **remembered** until the parcel reaches `Diverting` — a real,
motivated need for cross-tick memory, not an artificial requirement.

## Where you're starting from

```bash
git fetch --tags
git switch -c <your-branch-name> module-06-start
```

As always: `Engine::step()` and `Plant::advance()`'s behavior stay exactly as Module 5 left them, all
the way until the last mission.

## Module map

1. **The presence sensor** — `PresenceSensor`, the fourth class in the course, with a completely new
   kind of invariant: "never pretend to remember something you never actually observed."
2. **The weight sensor** — `WeightSensor`, the second (less guided) application of the same pattern.
3. **Fault-tolerant classification** — `decideClassification`, which rejects an untrustworthy reading.
4. **The controller's decision memory** — `ControllerState`, the **hardest and most central problem
   in this module**: how to combine two confirmations from two different sensors that are never
   current on the same tick.
5. **The engine with sensors** — wiring everything together, including a fix for a real design bug: a
   missing trustworthy decision has to actually stop the parcel, not let it keep moving.

## Before you start

- The Module 1–5 tests (`misja-1`–`misja-4`, `misja-6`–`misja-19`) are already present and passing.
- As always: you don't edit test files or `CMakeLists.txt`.

**Next:** [Mission 20: the presence sensor](./01_czujnik_obecnosci.en.md).
