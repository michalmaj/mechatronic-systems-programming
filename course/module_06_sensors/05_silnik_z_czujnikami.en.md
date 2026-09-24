[🇵🇱 Polski](05_silnik_z_czujnikami.md) | 🇬🇧 English

# 6.5 The engine with sensors

## Problem

Both sensors, `decideClassification`, and `ControllerState` exist — but nothing in the running
simulation uses them yet. `Engine` still trusts `Item::mass` directly.

## New C++ elements

**`Engine::injectFault(FaultTarget, FaultKind)` / `clearFault(FaultTarget)`** — different from every
input method so far (`requestStart`, `requestEStop`, ...): a fault is **persistent**. It doesn't
disappear after one tick the way the request flags from Modules 4–5 do — it lasts until explicitly
cleared by `clearFault`. This mirrors reality: a sensor fault doesn't heal itself.

New private fields: `presenceSensor_`, `weightSensor_`, `controllerState_`, `presenceFault_`,
`weightFault_` (the last two: `std::optional<FaultKind>`).

## The bug this mission fixes: `nullopt` must not silently become `HoldStraight`

It's important to understand this directly before you write any code. The naive approach would look
like: "if `controllerState_.classification` has a value, use it; otherwise leave the diverter's
default command (`HoldStraight`)." **That's a bug.** `HoldStraight` is a valid command — the diverter
would settle into position, and the parcel **would keep moving**, even though it was never actually
classified. A missing trustworthy decision has to mean **no routing**, not "route toward the default
position."

That's why `Plant::advance` (from this mission on) takes a third parameter:

```cpp
void advance(Plant& plant, const Diverter& diverter, bool routingReady = true);
```

Used **only** in the `Diverting` branch — every other zone moves unchanged. When `routingReady` is
false, the parcel stays in `Diverting`, exactly as if the diverter weren't settled yet.

## The extended `step()` order

Steps 1–4 (requests, `latch_`, the emergency path, `Mode`) stay unchanged from Module 5. New:

5. **Read both sensors, unconditionally, every tick** — regardless of `Mode` or an active override:
   `presenceSensor_.read(plant_.item, presenceFault_)`, `weightSensor_.read(plant_.item,
   weightFault_)`. A read is an observation, not advancing the simulation.
6. **Call `updateControllerState`** (Mission 23) with the current parcel and the fresh readings.
7. **Compute `routingReady`:**
   `!decision.overrideActive && diverterMayMove(mode_) && controllerState_.classification.has_value()`.
8. **The diverter decision** (still gated by `!decision.overrideActive && diverterMayMove(mode_)`,
   unchanged) — but now based on `controllerState_.classification`, not a direct
   `classify(item->mass)`.
9. **Call `psm::advance(plant_, diverter_, routingReady)`** — the first three-argument call in this
   module.
10. Assemble `TickResult` (now with a `sensors` field), increment `tick_`.

## Example run in the CLI

**`Stale` doesn't cause "different routing" — it shows that a plausible-looking number is NOT used for
a new classification.** The most convincing way to demonstrate this: send **one** parcel through
normally (no fault), so the weight sensor genuinely remembers a real mass. Then inject `Stale` on the
weight sensor and send a **second** parcel. The weight sensor will now be repeating the **first**
parcel's mass (a real, plausible-looking number) — but `decideClassification` rejects it anyway,
because the status is `Stale`, not `Ok`. The second parcel reaches `Diverting` and stays there.

## What you already have

`include/psm/engine.hpp` already has all the needed fields and declarations. `src/engine.cpp` has
empty skeletons for `injectFault`/`clearFault`; the body of `step()` still looks exactly as Module 5
left it — extending it is your job.

## What you need to write

- `Engine::injectFault(FaultTarget, FaultKind)` — store `kind` into `presenceFault_` or
  `weightFault_`, depending on `target`.
- `Engine::clearFault(FaultTarget)` — clear (`std::nullopt`) the corresponding field.
- `Engine::step()` — extend it with steps 5–10 described above.
- `apps/simulator_cli/main.cpp` — implement the example run (two parcels, a fault in between) and
  print `sensors` alongside the existing output.

## Check your work

```bash
ctest --preset test -L misja-24
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

Expected result: all tests green — `misja-1` through `misja-4`, `misja-6` through `misja-24`.

## Save your work

```bash
git status
git add <files you changed>
git commit -m "..."
```

## Common mistakes

- **Silently turning `nullopt` into `HoldStraight`** — exactly the bug described above. Check that
  `routingReady` really does reach `psm::advance`.
- **Gating only through `Mode`, without `controllerState_.classification.has_value()`** —
  `routingReady` has to combine all three conditions at once.
- **Updating `ControllerState` only sometimes** (e.g. only when `diverterMayMove` is true) — reading
  the sensors and updating state have to happen every tick, unconditionally; that's separate from
  gating the movement decision.

## Something to think about

This mission introduces `routingReady` as `Plant::advance`'s third, generic parameter — not
`WeightClass` or `DiverterCommand`. Why is that the right level of detail for this particular boundary
between `Plant` and `Controller`, given that `Plant` has never known anything about classification
since Module 1?

## End of Module 6

The system now genuinely **doesn't** trust its sensors unconditionally — and you've built a mechanism
that combines two independent, sequential confirmations into one trustworthy decision. In the modules
ahead, system events and actuator faults will arrive, finally giving `Mode::Fault` a concrete reason to
exist.

Modules 7 onward don't have an English version yet — for what comes next, see the
[Roadmap](../../docs/roadmap.en.md).
