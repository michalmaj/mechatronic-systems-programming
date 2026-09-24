[🇵🇱 Polski](04_silnik_pod_ochrona.md) | 🇬🇧 English

# 5.4 The engine under protection

## Problem

`EStopLatchState`, the extended `Mode`, and both safety functions exist — but nothing in the running
simulation calls them yet.

## New C++ elements

**`Engine::requestEStop()`, `releaseEStop()`, `requestReset()`** — three more input operations, the
same category as `spawnItem`/`requestStart`/`requestStop`: this is input, not advancing the
simulation, so it's fine to call between ticks.

A new private field, `latch_`.

## Two independent paths, genuinely independent

This is where it's easy to make a subtle architectural mistake — worth naming directly so you avoid
it. **It's not enough** to compute `Mode::EStopped` and then gate the diverter/belt *solely through*
`Mode`. Doing it that way would make the emergency path depend on `modeStep` being correct — exactly
the dependency this rule forbids (a future bug in the order of `modeStep`'s checks could silently
disable the emergency-stop button). That's why `Engine::step()` checks
`checkEmergencyOverride(latch_)` **directly**, and branches on it explicitly: when the override is
active, the routine belt/diverter logic **doesn't run at all** that tick — not "runs and happens to
agree," but skipped, mutually exclusive with the routine path in the very same `if`/`else`.

## The extended `step()` order

1. Consume and clear pending requests (eStop/release/reset/start/stop), then compute
   `latch_ = nextEStopLatchState(latch_, ...)`.
2. Compute `const SafetyDecision decision = checkEmergencyOverride(latch_);` — read directly from
   `latch_`, not derived from `Mode`.
3. Compute `mode_ = modeStep(mode_, startRequested, stopRequested, latch_)` — `Mode` is still needed
   (it's what's observable, and it drives the *routine* path); the emergency path simply no longer
   trusts it.
4. **The belt, as an `if`/`else`, never both at once:** when `decision.overrideActive` is true, call
   only `beltMotor_.forceStop()` and nothing else. Otherwise, behave exactly as in Module 4:
   `beltMotor_.setCommand(mode_ == Mode::Running ? BeltMotorCommand::Run :
   BeltMotorCommand::Stop)`, then `beltMotor_.resolve()`.
5. **The diverter, gated by both signals directly, not solely through `Mode`:**
   `if (!decision.overrideActive && diverterMayMove(mode_))` — only then does the Controller's
   decision → `diverter_.setCommand` → `diverter_.resolve()` run. Otherwise the diverter stays
   **completely untouched** that tick, frozen exactly where it is.
6. The gate on `psm::advance(plant_, diverter_)` based on `beltMotor_.actualState() == Running`,
   unchanged from Module 4.
7. Assemble `TickResult` (now with a `latch` field), increment `tick_`.

## Example run

The system is running (a parcel is moving, the belt is `Running`) → `requestEStop()` → **in that same
tick**: `mode = EStopped`, `latch = Engaged`, `beltActual = Stopped` (immediately, via `forceStop()`,
no ramp, no competing routine call that tick), the parcel frozen in place → `releaseEStop()` →
`latch = Armed`, **`mode` stays `EStopped`** (the latch still isn't `Released`) → `requestReset()` →
`latch = Released`, `mode = Idle` (**not** `Running`, per the Mission 17 rule) → resuming requires a
fresh `requestStart()`.

## What you already have

`include/psm/engine.hpp` already has all the needed fields and declarations. `src/engine.cpp` has
empty skeletons for `requestEStop()`/`releaseEStop()`/`requestReset()`; the body of `step()` still
looks exactly as Module 4 left it — extending it is your job.

## What you need to write

- `Engine::requestEStop()` — set `eStopPressed_` to `true`.
- `Engine::releaseEStop()` — set `eStopReleased_` to `true`.
- `Engine::requestReset()` — set `resetRequested_` to `true`.
- `Engine::step()` — extend it with the seven steps described above.
- `apps/simulator_cli/main.cpp` — demonstrate the run (press, release, reset, resume) and print
  `mode`/`latch` alongside the existing output.

## Check your work

```bash
ctest --preset test -L misja-19
```

This is a real, dedicated test for this mission. Expected result: `100% tests passed, 0 tests failed out of 1`.

Run the program for real too:
```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

## End of module — full test suite

```bash
ctest --preset test
```

Expected result: all tests green — `misja-1` through `misja-4`, `misja-6` through `misja-19`.

## Save your work

```bash
git status
git add <files you changed>
git commit -m "..."
```

## Common mistakes

- **Calling the routine belt/diverter logic even when `decision.overrideActive`** — that's exactly the
  independence gap this mission fixes. Check that your `if`/`else` really is mutually exclusive.
- **Gating the diverter solely through `diverterMayMove(mode_)`**, without `!decision.overrideActive`
  — again, the same gap, this time on the diverter side.
- **Forgetting `requestStart()` after recovery** — after `EStopped` the system returns to `Idle`, not
  `Running`; without a fresh start request, nothing more happens.

## Something to think about

This mission checks `decision.overrideActive` separately, instead of trusting `Mode::EStopped` alone.
Come up with a specific (hypothetical) bug in `modeStep` that would make relying on `Mode` alone
actually fail — while a direct check of `checkEmergencyOverride` would still work correctly.

## End of Module 5

The system now has a genuinely independent emergency path, alongside the existing routine path driven
by `Mode`. In the modules ahead, this architecture will keep growing — sensors, hardware faults, and
system events will arrive, finally giving `Mode::Fault` a concrete reason to exist.
