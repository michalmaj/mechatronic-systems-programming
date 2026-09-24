[🇵🇱 Polski](03_integracja_w_cli.md) | 🇬🇧 English

# 9.3 CLI integration

## Problem

Nothing yet shows `Scenario`/`runScenario` genuinely replacing hand-written imperative sequences in a
running program — and nothing besides the unit test confirms that this demonstration is real and
complete.

## What you already have

[`apps/simulator_cli/main.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/apps/simulator_cli/main.cpp) is **entirely ready and unchanging**
in this module — **this is not your mission**. It builds both demonstration scenarios, runs them
through `runScenario`, prints every tick through `describe()`, and checks the result itself: if the
recovery scenario never shows `Mode::Fault`, or the multi-parcel scenario produces fewer than three
departures, the program exits with a code other than `0`.

[`apps/simulator_cli/scenario_demos.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/apps/simulator_cli/scenario_demos.hpp) — the
declarations for `recoveryDemoScenario()`/`multiParcelDemoScenario()`, ready.

## What you need to write

Both bodies in [`apps/simulator_cli/scenario_demos.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-09-start/apps/simulator_cli/scenario_demos.cpp):

### `recoveryDemoScenario()`

Reproduce **the same mechanism and timeline** as the blocked-diverter recovery trace from Module 7
([`course/module_07_fault_mode/04_silnik_z_wykrywaniem_awarii.md`](../module_07_fault_mode/04_silnik_z_wykrywaniem_awarii.en.md))
— **not** a literally identical, historical CLI text, because that's no longer possible: the shape of
`TickResult` and the `describe()` format changed in Module 8 (a separate field for each zone instead of
one `item`/`zone`). What **hasn't** changed since Module 7 is the mechanism itself — `Diverter`,
`Mode`, `BeltMotor`, `EStopLatch` — so the same tick numbers at which `DiverterNotReady`/
`RoutingDeadlineMissed`/`Mode::Fault`/recovery appear are still exactly reproducible, just rendered
through today's `describe()` format.

Specifically: one arrival (750g) at tick 0, `StartRequested` at tick 0, a
`ScriptedDiverterFault{Blocked}` active from tick 0 to tick 8 (the equivalent of `clearDiverterFault()`
in the original), `Reset` at tick 9, `StartRequested` at tick 10, `duration = 12`.

### `multiParcelDemoScenario()`

Three arrivals with different classifications (e.g. 100g/800g/150g — Light/Heavy/Light), with at least
two ticks between arrivals, so that no arrival lands on a still-occupied `Infeed` (a parcel only leaves
`Infeed` once the belt is genuinely moving — ramp-up takes one tick). This demonstrates a capability
Mission 34 alone couldn't show yet: several parcels declared up front, declaratively, instead of
through a manual loop retrying the spawn every tick (compare
`tests/multiple_items_engine_test.cpp` from Module 8).

## Check your work

```bash
ctest --preset test -L misja-35
```

This is a real test (`simulator_cli_scenario_smoke`), not just the old `simulator_cli_smoke`
(misja-6). Both run exactly the same command — but they now mean something different:
`simulator_cli_smoke` always only checked "the program started and returned 0"; the CLI's own
verification means that same exit code is now genuinely tied to whether both demonstrations actually
succeeded. That's why `misja-6`, though historically separate, shares its result with `misja-35` in
this module, until this mission is done.

Run the program for real too and compare the printed recovery trace against Module 7's:
```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

## End of module — full test suite

```bash
ctest --preset test
```

Expected result: all tests green (`misja-1`, `misja-3`–`misja-4`, `misja-6`–`misja-35`).

## Save your work

```bash
git status
git add <files you changed>
git commit -m "..."
```

## Common mistakes

- **Trying to literally reproduce Module 7's old CLI text** — impossible and unnecessary; what matters
  is the mechanism and timeline, not a literal string.
- **Too-tight gaps between arrival ticks in `multiParcelDemoScenario`** — an arrival scheduled before
  the previous parcel has managed to leave `Infeed` will make `runScenario` return `std::nullopt`.
- **Editing `main.cpp`** — it's a ready-made program skeleton for the whole module; your work is
  entirely in `scenario_demos.cpp`.

## Something to think about

`main.cpp` itself checks only two things: whether `Mode::Fault` appeared at all, and whether there were
at least three departures. That's a deliberately minimal bar. What broken, but still "working" (exit
code `0`) `recoveryDemoScenario()` would pass that check, despite not reproducing Module 7's mechanism
at all?

## End of Module 9

The simulator now has two parallel ways to drive an experiment: imperative, step by step (since
Module 1), and declarative, as one `Scenario` value to record, pass around, and replay unchanged.
Neither replaced the other — both go through exactly the same public `Engine` API, which never had to
find out that this choice exists at all.

What comes after Module 9 — the checkpoint, the pre-project test-writing exercise, and the final
project — doesn't have an English version yet. For what comes next, see the
[Roadmap](../../docs/roadmap.en.md).
