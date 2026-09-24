[🇵🇱 Polski](03_przenosnik_pod_kontrola_trybu.md) | 🇬🇧 English

# 4.3 The conveyor under mode control

## Problem

`BeltMotor` and `Mode` exist, but nothing connects them to the running simulation yet — and nothing
yet stops a parcel from moving on a belt that nobody asked to run.

## New C++ elements

**`Engine::requestStart()` / `Engine::requestStop()`** — explicit input operations, exactly the same
category as `spawnItem` (the Module 3 invariant: this is input, not advancing the simulation, so it's
fine to call between ticks).

New private `Engine` fields: `mode_`, `beltMotor_`, and two flags (`startRequested_`, `stopRequested_`)
that record a request to be consumed on the next `step()`.

## `Mode` vs. `BeltMotorState` — two different questions

**`Mode`** is **intent** — what the whole system wants, and it changes immediately, in the same tick
where `modeStep` computes the new value. **`BeltMotorState`** is the **actual state of the physical
device** — and it deliberately lags behind the intent, because that's exactly what ramping means.

Consequence: `Mode::Idle` together with `BeltMotorState::RampingDown`, and `Mode::Running` together
with `BeltMotorState::RampingUp`, are **correct, expected, transitional** combinations — not bugs, not
something to special-case. You'll see them in this mission's verification.

## The extended `step()` order

1. Consume and clear any pending start/stop requests, then compute `mode_ = modeStep(mode_, ...)` (the
   conflict rule from Mission 14 applies here too).
2. `beltMotor_.setCommand(mode_ == Running ? Run : Stop)`, `beltMotor_.resolve()`.
3. Controller decision → `diverter_.setCommand` → `diverter_.resolve()` — unchanged from Module 3.
4. Call `psm::advance(plant_, diverter_)` **only if** `beltMotor_.actualState() == Running` —
   otherwise skip that call entirely; the parcel simply doesn't move this tick.
5. Assemble the now-larger `TickResult`, increment `tick_`.

## Example run

A fresh `Engine`, a parcel already added via `spawnItem`. You call `requestStart()`, then `step()`
twice:

- **First `step()`**: `mode` becomes `Running` in this same call, `beltActual` becomes `RampingUp`
  (the belt was `Stopped`, now it wants to be `Running`) — **the parcel doesn't move** (the gate
  doesn't pass: not `Running` yet).
- **Second `step()`**: `mode` stays `Running`, `beltActual` becomes `Running` (one more `resolve()`
  step from `RampingUp`) — the gate now passes, and the parcel moves **exactly one step**.

This exact run is what the provided test checks — your implementation has to reproduce it to the
letter.

## What you already have

`include/psm/engine.hpp` and `include/psm/tick_result.hpp` already have all the needed fields and
declarations. `src/engine.cpp` has empty skeletons for `requestStart()`/`requestStop()`; the body of
`step()` still looks exactly as Module 3 left it — extending it is your job.

## What you need to write

- `Engine::requestStart()` — set `startRequested_` to `true`.
- `Engine::requestStop()` — set `stopRequested_` to `true`.
- `Engine::step()` — extend it with the five steps described above.
- `apps/simulator_cli/main.cpp` — call `engine.requestStart()` before the loop (otherwise the belt
  never moves) and print `mode`/`beltActual` alongside the existing `describe()` line.

**`describe()` stays untouched** — its contract (the text describing `tick`/`item`) doesn't change;
you print mode/belt information separately in `main()`.

## Check your work

```bash
ctest --preset test -L misja-15
```

This is a real, dedicated test for this mission — not a fragment of something else. Expected result:
`100% tests passed, 0 tests failed out of 1`.

Run the program for real too:
```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

## End of module — full test suite

```bash
ctest --preset test
```

Expected result: all tests green — `misja-1` through `misja-4`, `misja-6` through `misja-15`.

## Save your work

```bash
git status
git add <files you changed>
git commit -m "..."
```

## Common mistakes

- **Calling `advance()` before checking the gate** — the gate must check the belt's state **after**
  `beltMotor_.resolve()` in that same tick, not before it.
- **Forgetting `requestStart()` in `main()`** — without it, `Mode` stays `Idle` forever, the belt never
  moves, and the program prints nothing but empty/stationary ticks.
- **Modifying `describe()`** — there's no need to; mode/belt information goes into a separate line in
  `main()`.

## Something to think about

The example run shows that the first `step()` after `requestStart()` does **not** move the parcel, and
only the second one does. If someone swapped the order of steps 2 and 4 (check the gate first, only
then call `beltMotor_.resolve()`), would the run look any different? Try tracing it on paper before
running the test.

## End of Module 4

The conveyor now has two actuators with real physical delay, and the system as a whole has a concept
of operating mode. In the modules ahead, `Mode` will grow with safety-related states — the emergency
stop button, which we deliberately avoided in this module, will finally find its place.
