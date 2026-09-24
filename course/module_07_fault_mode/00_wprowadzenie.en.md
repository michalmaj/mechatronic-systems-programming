[🇵🇱 Polski](00_wprowadzenie.md) | 🇬🇧 English

# 7.0 Introduction

`Mode::EStopped` has had a clear reason to exist since Module 5: the emergency-stop button.
`Mode::Fault` has existed since Module 4 — but with no trigger. This module changes that: a diverter
that can't meet its routing deadline (because it's physically blocked) finally gives `Fault` a
concrete reason to appear.

`Diverter::isSettled()` — the check that tells a genuinely blocked diverter apart from one that simply
hasn't been given a move command yet — has existed unchanged since Module 2. This module doesn't
rebuild it; it puts it to a new use.

This module's biggest mechanical change: `Mode` is now computed in **two explicitly separate steps**,
not one. Previously, `modeStep` answered one question: "what do the operator/e-stop/reset inputs say?"
Now that question stays separate from a second one: "what did `Plant::advance()` just report?" —
because these two questions have answers available at two different moments within the same tick. The
`tryb_awarii` mission explains exactly why this had to be two functions, not one function called
twice.

## Where you're starting from

```bash
git fetch --tags
git switch -c <your-branch-name> module-07-start
```

As always: `Engine::step()` stays exactly as Module 6 left it, all the way until the last mission.

## Module map

1. **The blocked diverter** — `Diverter::resolve` learns to react to an actuator fault, the same
   pattern as the sensors in Module 6, but applied to a new, separate type (`DiverterFaultKind`) —
   not to one shared, overloaded type that would allow nonsensical combinations like "a blocked
   presence sensor."
2. **The routing deadline** — `Plant` counts how long the diverter has been falling behind, and
   reports it as a `SystemEventKind`. The biggest piece of theory in this module: why the deadline
   must measure only a *continuous, active* routing attempt.
3. **Fault mode** — `Mode` learns about `Fault`, in two separate steps: `modeStep` (operator input)
   and the new `reactToSystemEvent` (reacting to an event from `Plant`). `Fault` is latched: neither
   `stopRequested` nor `startRequested` can get out of it — only an explicit reset can.
4. **The engine with fault detection** — wiring everything together: the split fault API
   (`injectSensorFault`/`injectDiverterFault`), `step()` using `modeForTick` to gate the actuators and
   `reactToSystemEvent` to determine the final `mode_`, and a full recovery scenario in the CLI.

## Before you start

- The Module 1–6 tests (`misja-1`–`misja-4`, `misja-6`–`misja-24`) are already present and passing.
- As always: you don't edit test files or `CMakeLists.txt`.

**Next:** [Mission 25: the blocked diverter](./01_zablokowany_dywerter.en.md).
