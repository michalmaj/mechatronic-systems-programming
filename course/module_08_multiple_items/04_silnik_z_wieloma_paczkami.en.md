[🇵🇱 Polski](04_silnik_z_wieloma_paczkami.md) | 🇬🇧 English

# 8.4 The engine with multiple parcels

## Problem

All the pieces exist separately — the new `Plant`, `advance()`, the two correlation functions — but
nothing in `Engine::step()` connects them yet. This mission's starting skeleton deliberately does
nothing: it compiles, but never calls `advance()`, so no parcel moves.

## Integration: responsibilities and constraints

This isn't new logic — it's combining the mechanisms from Modules 6 and 7, and the rest of this
module, into one whole, now for four slots at once instead of one. Nothing below is a new idea; what's
new is only that all of it has to work together, in one `step()` call.

**Sensors and per-parcel correlation.** Each sensor still reads exactly the slot it's physically
attached to (Mission 31). Two things have to happen here: the correlation state of whichever parcel is
actually present in that slot has to be updated, via `updatePresenceConfirmation`/`updateClassification`
(only when the slot is occupied — calling either on an empty `std::optional<Item>` won't compile), and
`SensorSnapshot` has to record which parcel (`ItemId`) a given reading concerned — but only when the
reading was `Ok` **and** the slot is actually occupied. A `Stale` reading is never attributed to
today's occupant of the slot, even if someone happens to be standing there — it's a repeat of an
earlier trusted value, not a fresh observation.

**The diverter command.** It must come **exclusively** from the parcel that's currently in
`plant_.diverting` — never from a parcel just classified in `plant_.weighing` in that same tick
(Mission 30 explained why that's the only correct choice: such a parcel physically can't be in
`diverting` yet). The gate is the same as in Module 7 (no override, `diverterMayMove(modeForTick)`),
with two extra conditions: the `diverting` slot must be occupied, and that parcel must already have a
classification. This same condition decides, at once, whether a diverter command may be issued at all,
and the value of `routingReady`, which has to be passed to `advance()` — it's one condition, not two
independent ones.

**Ordering that matters.** Reading the sensors, correlation, and the diverter-command decision all have
to happen before `advance()` moves anything — otherwise you'd be checking slot occupancy after the
move, not before it. `advance()`, in turn, has to happen before you compute `Mode` a second time — as
in Module 7, because that second computation needs the event from `AdvanceResult`, which `advance()`
hasn't returned yet. Everything else (input flags, `latch_`, `decision`, the first `modeForTick`
computation, belt gating) stays exactly as in Module 7.

**What `TickResult` must prove.** The complete state of all four slots this tick, `advance()`'s result
(`event`, `departure`), and full `ItemId` correlation in `SensorSnapshot` — the exact format is
described in the section below. This is the only source of truth that tests and the CLI use; neither
of them reaches into `Engine`'s internal state mid-tick.

## Extended `TickResult` and `describe()`

`SensorSnapshot` now carries `presenceObservedItemId`/`weightObservedItemId` — set only when the
reading is `Ok` **and** the slot is actually occupied. A `Stale` reading repeats an *earlier* trusted
value and is never attributed to today's occupant of the slot — even if someone happens to be standing
there.

`describe(TickResult)` becomes the single source of truth for the full text trace of a tick, in exactly
this format (one line):

```text
tick <N>: mode=<M> belt=<B> latch=<L> diverter=<cmd>/<pos>@<id|-> event=<e|-> infeed=<id|->
presenceCheck=<id|-> weighing=<id|-> diverting=<id|-> departure=<id->dest|->
```

where `<dest>` is `"Light"` for `Zone::OutputLight` or `"Heavy"` for `Zone::OutputHeavy` (not the full
`Zone` name). Test `tick_result_test.cpp` contains exact examples of this format — that's your
ultimate contract.

## What you already have

All the types (`Item`, `Plant`, `TickResult`, `SensorSnapshot`, `Engine`) — complete shapes, no changes
needed. The rest of `Engine` besides `step()` (input flags, `injectSensorFault`/`clearSensorFault`/
`injectDiverterFault`/`clearDiverterFault`, `spawnItem`) — ready and unchanged.

## What you need to write

- `Engine::step()` in [`src/engine.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/engine.cpp) — the full integration described above.
- `describe(TickResult)` in [`src/tick_result.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/tick_result.cpp) — the format described
  above.
- [`apps/simulator_cli/main.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/apps/simulator_cli/main.cpp) — a demonstration of at least three
  parcels with different classifications in flight at once (e.g. Light/Heavy/Light — see the trace in
  Mission 30's material), including at least one tick showing a visible "same-tick chain shift": one
  parcel departing and the next entering the just-freed slot in that same tick. Use `psm::describe()`
  to print each tick.

## Check your work

```bash
ctest --preset test -L misja-32
```

This is a dedicated test for this mission (`multiple_items_engine_test`), checking that three parcels
with masses 100g/800g/150g, created one after another via `spawnItem`, depart in the same order, each
with correct, independent routing (Light/Heavy/Light) — it will catch exactly a regression to shared
classification.

Filling in `Engine::step()` also unblocks every remaining `Engine`-level test — run the full suite too:

```bash
ctest --preset test
```

Expected result: all tests green (`misja-1`, `misja-3`–`misja-4`, `misja-6`–`misja-22`,
`misja-24`–`misja-32`).

Run the program for real too:
```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

## Save your work

```bash
git status
git add <files you changed>
git commit -m "..."
```

## Common mistakes

- **Computing the diverter command from `plant_.weighing`** — exactly the mistake Mission 30 and the
  "The diverter command" section above warn about.
- **Calling `updatePresenceConfirmation`/`updateClassification` without checking `.has_value()`** — a
  call on an empty `std::optional<Item>` won't compile (nothing to dereference) — but it's easy to miss
  the `if` gate itself if you copy code without thinking it through.
- **Setting `presenceObservedItemId`/`weightObservedItemId` regardless of the slot's `.has_value()`** —
  a reading can be `Ok` while the slot is still empty (a confirmed empty reading); correlation has to
  tell these apart.
- **Extending `describe()` with a destination format other than `"Light"`/`"Heavy"`** — the test checks
  exactly those two literals, not the full `Zone` names.

## End of Module 8

The simulator now models what every real sorting line does: several parcels at once, each at its own
stage, each with its own state — and yet one shared diverter and one shared belt still work correctly,
because the "at most one parcel per zone" constraint and the output-to-input processing order
eliminate collisions by construction, with no extra arbitration logic.
