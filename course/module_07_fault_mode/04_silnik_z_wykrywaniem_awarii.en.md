[🇵🇱 Polski](04_silnik_z_wykrywaniem_awarii.md) | 🇬🇧 English

# 7.4 The engine with fault detection

## Problem

All the pieces exist separately — the blockable diverter, the deadline counter in `Plant`, the
two-step `Mode` computation — but nothing in the running engine connects them yet.

## New C++ elements

**A split fault API.** `SensorFaultKind`/`SensorTarget` are Module 6's types under new, clearer names
(`injectFault`/`clearFault` were also renamed to `injectSensorFault`/`clearSensorFault`) — that's
already done, no work needed from you. What's new:

```cpp
void injectDiverterFault(DiverterFaultKind kind);
void clearDiverterFault();
```

No target parameter — there's only one diverter, so there's nothing to choose between.

## Extending `step()` — the heart of this mission

`step()` stays in almost the same shape as Module 6 (input flags, `latch_`, `decision`, sensors, and
`ControllerState` — unchanged) — four things change, in this order:

1. **Computing `Mode` moves to the very start of `step()`**, before any routing attempt this tick.
   Compute `modeForTick` with exactly the same `modeStep(...)` call as before and store the result in
   a local variable — **don't** assign it to `mode_` yet.
2. **Actuator gating (diverter, belt) uses `modeForTick`, not `mode_`** — the `mode_` field still holds
   its old value this tick, until point 4 below. The diverter's gate additionally passes
   `diverterFault_` to `diverter_.resolve(...)`, the same way the sensors get their faults in
   Module 6.
3. **`psm::advance(...)` is called under the same gate as in Module 6** (the belt actually `Running`),
   but its result now has to be remembered — `advance()` reports whether a `SystemEventKind` occurred
   this tick.
4. **The second `Mode` computation happens only after `advance()`**, not before it: this is the only
   place where `mode_` gets a new value this tick, through `reactToSystemEvent(modeForTick, event)` —
   `modeForTick` from point 1, `event` from point 3.

This tick's `TickResult` gains an `event` field.

## The one-tick belt-stop delay — accepted, not a bug to fix

A tick that detects `RoutingDeadlineMissed` gates its actuators using `modeForTick == Running` (that
was the value known at the start of this tick — the attempt that revealed the missed deadline itself
had to run under `Running` gating) and only reports `mode = Fault` at the very end. The effect: this
tick's `TickResult` can show `mode = Fault` while `beltActual` is still `Running`. The belt only
actually starts slowing down (`RampingDown`) on the **next** tick, once `modeForTick` for that
following tick is already `Fault`.

This is deliberately different from the e-stop, which forces `beltMotor_.forceStop()` immediately, via
`decision.overrideActive` — computed independently of `Mode`, precisely so it takes effect in the same
tick the button press was detected. `Fault` has no such forced path in this module, and isn't meant to:
it's a controlled, routing-related state, not a safety-critical one. The one-tick delay is correct,
accepted behavior, not a gap.

## Example recovery scenario

```text
step 1: item enters Infeed, mode=Running, belt=RampingUp — the parcel is still waiting, the belt is only just ramping up.
step 2: item PresenceCheck, belt=Running — only now is the belt actually moving, the parcel starts.
step 3: item Weighing.
step 4: item Diverting, already classified in ControllerState.
step 5: event=DiverterNotReady — first active attempt, diverter Blocked.
step 6: event=RoutingDeadlineMissed, mode=Fault, belt still Running — the same tick.
step 7: mode=Fault, belt=RampingDown — only now does the belt start slowing down.
step 8: mode=Fault, belt=Stopped.
— clearDiverterFault() —
step 9: mode=Fault — clearing the fault alone isn't enough.
— requestReset() —
step 10: mode=Idle — the latch only gives way now. The parcel is still in Diverting.
— requestStart() —
step 11: mode=Running, belt=RampingUp — the parcel is still waiting, the belt has to ramp up again from scratch.
step 12: belt=Running, event=none — the diverter (fault already cleared, moving since the previous tick) managed to settle before advance() even got to check it — straight to routing, with no DiverterNotReady along the way.
```

Notice step 12: in this particular sequence, recovery does **not** go through `DiverterNotReady` at
all — because the belt also had to ramp up from scratch (it was fully stopped during `Fault`), so
`advance()` doesn't check the diverter until step 12, and the diverter had already had two ticks to
settle (steps 11 and 12). That's not a coincidence: both the belt and the diverter need exactly two
`resolve()` calls to reach their target from a resting state.

If `clearDiverterFault()` hadn't been called, steps 11–12 would repeat exactly the sequence from steps
5–6: `DiverterNotReady` again, then `RoutingDeadlineMissed` and `Fault` again. This shows directly that
`requestReset()` on its own only clears the **symptom** (the mode latch), never the **cause**.

## What you already have

`include/psm/engine.hpp` already has all the needed fields and declarations. `src/engine.cpp` has the
renamed `injectSensorFault`/`clearSensorFault` ready; empty skeletons for
`injectDiverterFault`/`clearDiverterFault`; the body of `step()` still looks exactly as Module 6 left
it — extending it is your job.

## What you need to write

- `Engine::injectDiverterFault(DiverterFaultKind)` — store `kind` in `diverterFault_`.
- `Engine::clearDiverterFault()` — clear (`std::nullopt`) `diverterFault_`.
- `Engine::step()` — extend it as described above: `modeForTick` for actuator gating, `diverterFault_`
  passed to `diverter_.resolve()`, `reactToSystemEvent` setting `mode_` at the end, `event` in
  `TickResult`.
- `apps/simulator_cli/main.cpp` — implement the recovery scenario described above.

## Check your work

```bash
ctest --preset test -L misja-28
```

This is a real, dedicated test for this mission, tracking the recovery scenario above step by step.
Expected result: `100% tests passed, 0 tests failed out of 1`.

Run the program for real too:
```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

## End of module — full test suite

```bash
ctest --preset test
```

Expected result: all tests green — `misja-1` through `misja-4`, `misja-6` through `misja-28`.

## Save your work

```bash
git status
git add <files you changed>
git commit -m "..."
```

## Common mistakes

- **Gating actuators through `mode_` instead of `modeForTick`** — this tick, `mode_` doesn't have its
  new value yet; that only appears after `reactToSystemEvent`.
- **Calling `reactToSystemEvent` before `Plant::advance()`** — then `event` wouldn't be known yet. The
  order in steps 3 and 4 matters here.
- **Adding a forced belt stop for `Fault`** — that's exactly the thing this module deliberately
  doesn't do (see the section on the one-tick delay above).

## Something to think about

The recovery scenario above shows the belt and diverter "coincidentally" finishing their ramp-up in
the same step. Change the belt's ramp-up time in your head (or experimentally, on the side) so it's
one tick longer than the diverter's settle time. What exactly would `TickResult` show in that
hypothetical scenario, at the step where the diverter is already settled but the belt isn't `Running`
yet?

## End of Module 7

`Mode::Fault` now has a real, tested trigger — and you've built a mechanism that separates two
genuinely different responsibilities (operator input versus reacting to a system event) into two
separate, named functions, instead of hiding them under one. This is the same spirit as the two
independent safety paths from Module 5 — this time applied to two different moments in a tick's
timeline, not to two different actuators.
