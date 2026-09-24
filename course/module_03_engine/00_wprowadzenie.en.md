[🇵🇱 Polski](00_wprowadzenie.md) | 🇬🇧 English

# 3.0 Introduction

Module 2 (Mission 9) established a fixed order for one tick: Controller decision → `setCommand` →
`resolve` → `advance` → observation. But that order only lived by convention — it was manually
duplicated in both `runTicks` and `main()`. Nothing guaranteed the two copies couldn't drift apart.

This module puts an end to that: a new `Engine` class takes ownership of `Plant` and `Diverter` and
becomes the **only** place in the code where that order exists at all.

## Where you're starting from

```bash
git fetch --tags
git switch -c <your-branch-name> module-03-start
```

An important difference from previous modules: **`runTicks` and the existing `main()` are still here
and still work** — exactly as Module 2 left them. Nothing has been rewired yet. You'll see the old,
manual orchestration (`runTicks`) sitting right next to the new, still-empty `Engine` that's meant to
replace it. That's deliberate — before you remove the duplication, it's worth seeing it with your own
eyes first.

Only the module's last mission rewires `main()` to use `Engine`.

## Module map

1. **Tick and result** — one immutable description of what happened during a given tick.
2. **Engine formalizes the order** — the course's second class: `Engine`, which *owns* `Plant` and
   `Diverter`.
3. **Rewiring to the engine** — the moment `main()` actually starts using `Engine`.

## Before you start

- The tests from Modules 1 and 2 (`misja-1`–`misja-4`, `misja-6`–`misja-9`) are already in place and
  passing — an inherited, ready-made foundation.
- This module's last mission (Mission 12) has no separate automated test — its real check is building
  and running the program, exactly as in Mission 6 of Module 1.

**Next:** [Mission 10: tick and result](./01_tick_i_wynik.en.md).
