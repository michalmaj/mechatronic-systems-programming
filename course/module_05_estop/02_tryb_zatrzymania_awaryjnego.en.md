[🇵🇱 Polski](02_tryb_zatrzymania_awaryjnego.md) | 🇬🇧 English

# 5.2 Emergency-stop mode

This is the theoretically most important mission in this module — we'll spend a bit more space on it.

## Problem

`Mode` from Module 4 knows nothing about the emergency-stop button's existence. It has to find out —
and it has to care about it **more** than anything else it checks today.

## New C++ elements

`modeStep` gains a new parameter, and `Mode` gains a new value:

```cpp
enum class Mode { Idle, Running, EStopped };

Mode modeStep(Mode current, bool startRequested, bool stopRequested,
              EStopLatchState latch = EStopLatchState::Released);
```

Notice two things:

**The `latch` parameter is last, not second.** It could logically sit right after `current`, but
placing it at the end, **with a default value**, has a specific purpose: every existing call to
`modeStep` from before this module (including the Mission 14 test, `mode_test.cpp`, and the body of
`Engine::step()`) still compiles without any change, using the default `Released`. This is a real
example of exactly what default parameter values are for: they let you extend a function's interface
without breaking anyone who already uses it.

**`Mode::EStopped`** — a third value, alongside `Idle` and `Running`.

## Absolute priority

**The e-stop is checked first, before anything else.** If `latch != EStopLatchState::Released`, the
result is `Mode::EStopped` — full stop, no other rule matters in that call. It doesn't matter whether
`startRequested` or `stopRequested` is true — the latch always wins.

## Recovery always goes through `Idle`

**Recovering from `EStopped` always lands in `Idle`, never automatically in `Running`.** The moment
`latch` reads `Released` again, `Mode` becomes `Idle` — resuming operation requires an explicit,
separate `requestStart()` from the operator, exactly as if starting from scratch.

**Edge case: `resetRequested` and `startRequested` both true in the same tick.** The result is still
`Idle`, never `Running`. The "did we just come back from `EStopped`" check takes priority over the
"`Idle` plus `startRequested`" check — a `Start` requested in the same tick as the reset that cleared
the latch does **not** take effect that tick. The operator will see `Idle`, and only a **separate**,
later `requestStart()` will actually resume operation.

## What you already have

[`include/psm/mode.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-05-start/include/psm/mode.hpp) — the updated declaration, as above.

[`src/mode.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-05-start/src/mode.cpp) — the `Idle`/`Running` logic from Module 4 stays **untouched and
working**; two new `// TODO` comments describe what to add, and exactly where (before the rest of the
rules).

## What you need to write

Extend the body of `modeStep` with two checks, in this order, **before** the existing logic:
1. if `latch != EStopLatchState::Released`, return `Mode::EStopped`,
2. if `current == Mode::EStopped` (and the check above didn't fire, meaning `latch` is already
   `Released`), return `Mode::Idle`.

## Check your work

```bash
ctest --preset test -L misja-17
```

Expected result: `100% tests passed, 0 tests failed out of 1`. This is a new, dedicated test file — it
checks e-stop priority from various states, the return to `Idle`, and the
`resetRequested`+`startRequested`-at-once edge case.

It's also worth re-running `ctest --preset test -L misja-14` — it should still pass, even though you
haven't changed anything in it.

## Common mistakes

- **Checking `stopRequested`/`startRequested` before checking `latch`** — the e-stop must always win,
  no exceptions.
- **Returning `Mode::Running` instead of `Mode::Idle`** when recovering from `EStopped` while
  `startRequested` happens to be true in that same call — that's exactly the conflict case above.
- **Placing the new checks at the end of the function** instead of at the start — order matters, the
  e-stop must be checked first.

## Something to think about

If `latch` were `modeStep`'s second parameter, with no default value, what exactly would have to
change in this module — and in how many places?

**Next:** [Mission 18: two independent paths](./03_dwie_niezalezne_sciezki.en.md).
