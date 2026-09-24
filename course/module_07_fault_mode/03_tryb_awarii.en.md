[🇵🇱 Polski](03_tryb_awarii.md) | 🇬🇧 English

# 7.3 Fault mode

## Problem

`Mode` doesn't yet know the routing deadline can be missed, and there's no way to recover from it. But
there's something deeper here than just a missing rule: `Mode` now has to answer two **different**
questions, available at two **different** moments within the same tick.

An earlier draft of this project tried two shortcuts. First: moving `Plant::advance()` before the
`Mode` computation. Rejected — it would silently shift the exact moment of parcel movement by one
tick for every existing test that checks it, including the already-existing Module 4 tests. Second:
calling the existing `modeStep()` twice in one tick, with some flags manually forced to `false`. That
technically works — but it obscures the fact that two genuinely different responsibilities have
appeared under one function name.

## New C++ elements

```cpp
enum class Mode { Idle, Running, EStopped, Fault };

// Step 1: previous Mode + this tick's operator/e-stop/reset inputs -> the Mode the actuators will use
// for gating IN THIS tick. Same spirit as Module 5; one new parameter, one new rule.
Mode modeStep(Mode current, bool startRequested, bool stopRequested,
              EStopLatchState latch = EStopLatchState::Released,
              bool resetRequested = false);

// Step 2: modeForTick + what Plant::advance() just reported -> the Mode actually stored and returned
// in TickResult. A completely new function.
Mode reactToSystemEvent(Mode modeForTick, std::optional<SystemEventKind> event);
```

`modeStep` **knows nothing at all** about `SystemEventKind` — it's not in any parameter.
`reactToSystemEvent` **knows nothing at all** about the operator, the e-stop latch, or reset — it has
exactly two parameters and one job. Each function's name states directly what it's responsible for —
neither is a disguised second version of the other.

## The corrected rule order in `modeStep`

The rule chain from Module 5 stays in the same order, unchanged: the e-stop latch's unconditional
priority, then recovery from `EStopped` to `Idle`, then `stopRequested`, then `startRequested`, and
finally `current` unchanged if none of the above applies. The new rule enters as **third** in this
order — right after recovery from `EStopped`, before `stopRequested` — and applies only to the
`current == Mode::Fault` case. Its precise behavior (when it returns to `Idle`, when it stays in
`Fault`) is described in the "`Fault` is latched" section below.

Notice what's **not** here: no `routingDeadlineMissed` parameter, no rule mentioning it at all. That
responsibility now belongs entirely to `reactToSystemEvent`.

## The whole `reactToSystemEvent` rule

The only case where the function returns `Fault` is when both conditions hold at once:
`modeForTick == Mode::Running` and `event == SystemEventKind::RoutingDeadlineMissed`. In every other
situation — a different `modeForTick`, no event, or `event == SystemEventKind::DiverterNotReady` — the
function returns `modeForTick` unchanged.

The function has no access to `start`/`stop`/`reset`/`latch`, so it can't re-run any `modeStep` rule.
It can only fire from `Running` — and since `latch != Released` always forces `EStopped` already in
`modeStep`'s step 1, `modeForTick` is never simultaneously `Running` and under an active e-stop in the
same tick. No combination of inputs can trigger both paths at once.

## `Fault` is latched

Rule 3 is a full "catch-all" — checked **before** rules 4 and 5. Since `current == Fault` always
returns either `Idle` (on reset) or `Fault`, neither rule 4 nor rule 5 is ever reached while `Fault`
holds. Neither `stopRequested` nor `startRequested` has any effect. The only way out is an explicit
`resetRequested` — and even then you land in `Idle`, never straight into `Running`. Recovery always
goes through `Idle` — exactly the same pattern as recovering from the e-stop in Module 5.

## Why the e-stop still takes priority over `Fault`

Rule 1 (`latch != Released -> EStopped`) is checked unconditionally, before `modeStep` even looks at
`current`. Even if `current == Fault`, a pressed e-stop immediately forces `EStopped`. This is
deliberate: the e-stop stays the most urgent path in the system, above everything else.

## What you already have

[`include/psm/mode.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/include/psm/mode.hpp) — `Mode::Fault`, both signatures are already
there.

[`src/mode.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-07-start/src/mode.cpp) — the existing `latch`/`EStopped`/`stopRequested`/
`startRequested` rules are complete and in the same order; only rule 3 (`// TODO`) and the entire body
of `reactToSystemEvent` (`// TODO`) are missing.

## What you need to write

Add rule 3 to `modeStep`, in the right place in the chain. Fill in `reactToSystemEvent`.

## Check your work

```bash
ctest --preset test -L misja-27
```

Expected result: `100% tests passed, 0 tests failed out of 1`. The test checks the whole
`reactToSystemEvent` rule, `Fault`'s latching against `stopRequested`/`startRequested`, recovery
through `Idle` on reset, and the e-stop's priority over `Fault`.

## Common mistakes

- **Placing the `Fault` rule after the `stopRequested`/`startRequested` rules** — then either one
  could get you out of it, which is exactly what the latch is supposed to prevent.
- **Trying to pass `routingDeadlineMissed` into `modeStep`** — that information isn't known yet at the
  moment `modeStep` is called; that's exactly why there's a second function.
- **`reactToSystemEvent` checking `current` instead of `modeForTick`** — these are two different names
  for the same argument in this signature, but the name `modeForTick` is used here deliberately, to
  emphasize that it's the result of step 1, not the raw `mode_` field.

## Something to think about

`reactToSystemEvent` doesn't take `latch` or `resetRequested`. Given that `Fault` is only latched
because of rule 3 in `modeStep`, what exactly would break if `reactToSystemEvent` could also return
`Fault` from any state, not just from `Running`?

**Next:** [Mission 28: the engine with fault detection](./04_silnik_z_wykrywaniem_awarii.en.md).
