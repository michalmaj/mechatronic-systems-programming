[🇵🇱 Polski](00_wprowadzenie.md) | 🇬🇧 English

# 5.0 Introduction

Until now, the system's safety has rested entirely on `Mode`, `Controller`, and the rest of the logic
working correctly. This module adds something fundamentally different: an emergency-stop button as a
**second, independent** safety path — independent of the Controller, independent of whether the rest
of the routing logic works correctly at all.

**An important disclaimer up front:** the emergency-stop button you build here is a simplified,
pedagogical model of the behavior — it teaches the *shape* of the problem (a second, independent path;
absolute priority; no silent automatic resumption) on a small, readable state machine. This is **not**
a design that meets safety standards for a real machine. Real safety circuits require certified
components, hardware interlocks, and standards this course doesn't cover.

## Where you're starting from

```bash
git fetch --tags
git switch -c <your-branch-name> module-05-start
```

Same as in the previous module: **`Engine::step()` stays exactly as Module 4 left it, all the way
until the last mission.** You first build the emergency-stop button, the extended `Mode`, and the
safety functions as standalone, separately testable pieces — only the last mission wires them into the
running `Engine`.

## Module map

This module has **four** missions, not three like the previous ones — the topic earns it.

1. **The emergency-stop button** — `EStopLatchState`, the third state machine in the course (after
   `Mode`).
2. **Emergency-stop mode** — `Mode` learns about the button and gains `EStopped`.
3. **Two independent paths** — standalone decision functions plus one deliberately mutating emergency
   operation.
4. **The engine under protection** — wiring everything into `Engine::step()`, genuinely
   independently.

## Before you start

- The Module 1–4 tests (`misja-1`–`misja-4`, `misja-6`–`misja-15`) are already present and passing.
- As always: you don't edit test files or `CMakeLists.txt`.

**Next:** [Mission 16: the emergency-stop button](./01_przycisk_awaryjny.en.md).
