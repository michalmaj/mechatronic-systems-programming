[🇵🇱 Polski](00_wprowadzenie.md) | 🇬🇧 English

# Module 9: declarative scenarios

## Where we are

Since Module 1, every `TickResult` trace has been built from manually interleaved calls to
`request*()`/`inject*()`/`clear*()`/`spawnItem()`, woven between successive `step()` calls. This
module adds a declarative alternative: a `Scenario` value that describes a whole experiment up front
(operator inputs, parcel arrivals, sensor/diverter fault windows, duration), plus `runScenario()`,
which replays it on a fresh `Engine` and returns the full trace.

## What changes — and what does NOT change

`Engine` stays **completely unchanged** — no new constructor, no new fields. Scripting is a layer ON
TOP OF `Engine`'s public API, calling exactly the same methods every existing test and the CLI already
use. This is a deliberate departure from the reference simulator, where `Engine` has no
`request*()`/`inject*()`/`clear*()` at all — there, the script *replaces* the imperative API. Here, the
script is an optional overlay, not a replacement.

`Scenario` is one named value: `operatorInputs`, `arrivals`, `sensorFaults`, `diverterFaults`,
`duration`. `isValidScenario` checks it statically, before anything runs. `runScenario` builds a fresh
`Engine` and replays the scenario tick by tick, returning
`std::optional<std::vector<TickResult>>` — `std::nullopt` for a scenario that's invalid, or one that
can't be completed.

## Three missions

- **Mission 33 — the model and validation.** The shape of `Scenario` and its five component types;
  `isValidScenario`.
- **Mission 34 — the replayer.** `runScenario`: a fresh `Engine`, a fixed tick order, the
  reproducibility contract.
- **Mission 35 — CLI integration.** Two demonstrations in `apps/simulator_cli`: reproducing the
  mechanism and timeline of Module 7's recovery trace, and a new scenario with scheduled arrivals of
  multiple parcels.

## Before you start

This module doesn't modify anything existing at the domain level — `Item`, `Plant`, `Engine`,
`TickResult` stay exactly as they are. `apps/simulator_cli/main.cpp`, however, is entirely new in this
module (not a continuation of Module 8's CLI) and **is not** any of your missions — it's a ready-made,
simple program that builds two scenarios, runs them, and checks the outcome itself. Your work is
`isValidScenario`, `runScenario`, and the bodies of two functions that build the specific demonstration
scenarios.

**Next:** [Mission 33: the model and validation](./01_model_i_walidacja.en.md).
