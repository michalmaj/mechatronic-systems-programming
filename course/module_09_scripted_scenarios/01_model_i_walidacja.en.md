[🇵🇱 Polski](01_model_i_walidacja.md) | 🇬🇧 English

# 9.1 The model and validation

## Problem

Until now, every scenario was written by hand, one call at a time. There's no way to write down "what
should happen" as one checkable value, before anything actually runs.

## New C++ elements

```cpp
enum class ScenarioInputKind {
    EmergencyStopPressed, EmergencyStopReleased, Reset, StartRequested, StopRequested
};
struct ScenarioInput { Tick at; ScenarioInputKind kind; };

struct ScriptedItemArrival { Tick at; ItemId id; Grams mass; };

struct ScriptedSensorFault { Tick from; Tick until; SensorTarget target; SensorFaultKind kind; };
struct ScriptedDiverterFault { Tick from; Tick until; DiverterFaultKind kind; };

struct Scenario {
    std::vector<ScenarioInput> operatorInputs;
    std::vector<ScriptedItemArrival> arrivals;
    std::vector<ScriptedSensorFault> sensorFaults;
    std::vector<ScriptedDiverterFault> diverterFaults;
    Tick duration;
};

bool isValidScenario(const Scenario& scenario);
```

`ScriptedSensorFault`/`ScriptedDiverterFault` are two **separate** types, not one shared "fault +
target" type — exactly the same decision as `SensorFaultKind`/`DiverterFaultKind` in Module 7: an
illegal combination should be unrepresentable in the types, not rejected only at runtime.

`duration` lives on `Scenario`, not as a separate parameter — a scenario is meant to be a complete
description of a repeatable experiment, and how long it runs is part of that description.

**The order of elements within each vector doesn't matter.** Every rule below is defined by the
`at`/`from`/`until` values stored in the data, not by position in the vector.

## The precise rules

1. Every fault interval: `from < until` and `until <= duration`.
2. No two `ScriptedSensorFault`s with the same `target` may overlap `[from, until)`. Overlapping for
   *different* targets is fine.
3. No two `ScriptedDiverterFault`s may overlap (there's one diverter).
4. Operator inputs, per tick: no repeated `ScenarioInputKind` on the same tick; at most two different
   kinds on the same tick, and only if that pair is exactly `{EmergencyStopReleased, Reset}`.
   **This is a narrower contract than `Engine` itself** — `nextEStopLatchState`/`modeStep` handle
   `Reset` and `StartRequested` set at the same time safely (unchanged since Module 5), but `Engine`'s
   tolerance for that flag combination is a different contract from what a well-written `Scenario`
   should *say*. A scenario that wants both effects records them on separate ticks.
5. Every `operatorInputs.at` must be `< duration`.
6. Every `arrivals.at` must be `< duration`.
7. No two `arrivals` may share the same `at` — that's always infeasible (`Infeed` holds one parcel), so
   it's statically detectable.
8. Every `ItemId` in `arrivals` must be globally unique across the whole `Scenario` — not just "unique
   among the parcels currently present" (the weaker invariant `Plant` enforces, from Module 8). This is
   a deliberate simplification: a scenario's author doesn't have to reason about the exact moment of
   departure to know whether reusing an id is safe.

`duration == 0` is explicitly a valid, boundary case — an empty scenario with zero duration. The rules
above guarantee this on their own: no `at`/`from` can satisfy `< 0` for a non-negative `Tick`, so a
valid scenario with `duration == 0` must have every vector empty.

## What you already have

[`include/psm/scenario_input.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/include/psm/scenario_input.hpp),
[`scripted_item_arrival.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/include/psm/scripted_item_arrival.hpp),
[`scripted_sensor_fault.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/include/psm/scripted_sensor_fault.hpp),
[`scripted_diverter_fault.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/include/psm/scripted_diverter_fault.hpp),
[`scenario.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/include/psm/scenario.hpp) — all shapes and the `isValidScenario` signature are
ready.

## What you need to write

Fill in the body of `isValidScenario` in [`src/scenario.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/src/scenario.cpp) according to
the eight rules above.

## Check your work

```bash
ctest --preset test -L misja-33
```

Expected result: `100% tests passed`. The test checks each of the eight rules individually, plus the
`duration == 0` boundary case.

## Common mistakes

- **Treating `{Reset, StartRequested}` as an allowed pair** — that's *not* the same question as
  "whether `Engine` can handle it." `Scenario` has a narrower contract.
- **Checking sensor-fault overlap without accounting for `target`** — overlap for different sensors is
  fine, only overlap for the same one is an error.
- **Rejecting `duration == 0`** — that's explicitly a valid case, not a bug to catch.

## Something to think about

Rule 8 requires global id uniqueness across the whole `Scenario`, even though `Plant` itself only
requires uniqueness among currently-present parcels. What specific scenario would be valid under
`Plant`'s rule but rejected by rule 8?

**Next:** [Mission 34: the replayer](./02_odtwarzacz.en.md).
