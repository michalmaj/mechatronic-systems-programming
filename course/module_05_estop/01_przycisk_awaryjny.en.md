[🇵🇱 Polski](01_przycisk_awaryjny.md) | 🇬🇧 English

# 5.1 The emergency-stop button

## Problem

A physical emergency-stop button isn't a simple on/off switch. It has a subtlety that's easy to miss:
**releasing the button must not silently resume normal operation**. An operator who releases the
button might have done so by accident, or because the problem still isn't fixed — the system has to
wait for an explicit confirmation before it considers resuming.

## New C++ element

**`enum class EStopLatchState { Released, Engaged, Armed }`** plus the free function
**`nextEStopLatchState(previous, pressed, released, resetRequested)`** — the third example of the
enum-plus-function pattern in this course (after `Mode` in Module 4).

## The rule

- **`pressed` always wins** — from any state, it moves immediately to `Engaged`.
- From `Engaged`: only `released` (the button physically let go) moves to `Armed`.
- From `Armed`: only an explicit `resetRequested` moves back to `Released`. `Armed` is the "released,
  but still waiting for confirmation" state — that's exactly why `Mode` (Mission 17) will never resume
  operation automatically.

**Edge case: `released` and `resetRequested` both true in the same call.** Only `released` counts. The
result is `Armed`, not `Released` — a reset can't "skip ahead" of a transition that was only just
observed. In other words: from `Engaged`, `released=true` together with `resetRequested=true` gives
`Armed`; only a **separate, later** call with `resetRequested=true` (once `previous` is already
`Armed`) leads to `Released`. This mirrors physical reality: you can't confirm the reset of a
transition that hasn't finished yet.

## What you already have

[`include/psm/estop_latch.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-05-start/include/psm/estop_latch.hpp) — `enum class EStopLatchState`
and the `nextEStopLatchState` declaration are already there.

[`src/estop_latch.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-05-start/src/estop_latch.cpp) — an empty skeleton with a `// TODO` comment.

## What you need to write

Fill in the body of `nextEStopLatchState` according to the rule above, including the
`released`+`resetRequested` edge case.

## Check your work

```bash
ctest --preset test -L misja-16
```

Expected result: `100% tests passed, 0 tests failed out of 1`. The test walks through the full
`Released→Engaged→Armed→Released` cycle, checks that reset is ignored in `Engaged`, checks the
`released`+`resetRequested`-at-once edge case, and checks that `pressed` takes priority over
everything else.

## Common mistakes

- **Checking `resetRequested` before `pressed`** — `pressed` must always win, no matter what else is
  true.
- **Reacting to `resetRequested` in the `Engaged` state** — a reset only makes sense in `Armed`; in
  `Engaged` (the button still held down) there's nothing to reset.
- **Returning `Released` instead of `Armed`** for the `released`+`resetRequested`-at-once case — that's
  exactly the case described above.

## Something to think about

Why does the `Armed` state exist at all? What specifically would go wrong if `Engaged` moved straight
back to `Released` the moment the button is released, with no intermediate step?

**Next:** [Mission 17: emergency-stop mode](./02_tryb_zatrzymania_awaryjnego.en.md).
