[🇵🇱 Polski](03_przepiecie_na_silnik.md) | 🇬🇧 English

# 3.3 Rewiring to the engine

## Problem

`main()` still manually orchestrates `Plant`/`Diverter`/Controller directly — exactly the duplication
`Engine` is meant to eliminate, and which you've been watching next to `Engine` since the start of this
module. Time to remove it.

## New C++ elements

None syntactically new — this mission is about **assembling** elements you already know (`Engine`,
`describe`) into a working program, the same way Mission 6 in Module 1 was your first `main()` of your
own.

## What you already have

[`apps/simulator_cli/main.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/apps/simulator_cli/main.cpp) still looks exactly as Module 2 left it — it
creates its own `Plant`/`Diverter` and manually repeats the tick order. This is your starting point for
rewriting it, not something to preserve.

## What you need to write

Rewrite `main()` so that it:

1. creates a `psm::Engine`,
2. adds one parcel through `engine.spawnItem(...)`,
3. in a loop (e.g. 8 ticks) calls `engine.step()` and prints the result via
   `std::cout << psm::describe(result) << '\n';`.

That's it — all the logic for deciding, setting the diverter, and moving the parcel already lives in
`Engine::step()`. After rewriting, `main()` should be shorter than before, not longer.

## The moment `runTicks` stops being used

From this mission on, `runTicks` (in [`src/loop.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/src/loop.cpp)) has no caller left anywhere in the
program — `Engine` has taken over its role. The file itself **stays** in the repository; physically
removing it (and the accompanying `CMakeLists.txt` change) is out of scope for this mission and isn't
your job. Unused code that still compiles correctly and is waiting to be cleaned up is a normal,
transitional situation in real projects — not every piece of duplication disappears the same day it
stops being needed.

## Check your work

This mission has no separate `ctest` label — just as in Mission 6 of Module 1, the real test is
running the program and reading the result:

```bash
cmake --build --preset dev
./build/dev/apps/simulator_cli/simulator_cli
```

Check that the result looks sensible: the parcel moves through successive zones, eventually reaches
`OutputHeavy` or `OutputLight`, and then disappears (`empty`).

## End of module — full test suite

```bash
ctest --preset test
```

Expected result: all tests green — `misja-1` through `misja-4`, `misja-6` through `misja-11`. (This
mission, like Mission 6, has no separate label — see "Check your work" above.)

## Save your work

```bash
git status
git add <files you changed>
git commit -m "..."
```

## Common mistakes

- **Calling `describe` with an incomplete `TickResult`** — `engine.step()` already returns a complete,
  ready-made snapshot; you don't need to (and shouldn't) build a `TickResult` by hand in `main()`.
- **Leaving the old logic next to the new** — after this mission, `main()` should no longer refer
  directly to `Plant`, `Diverter`, or `classify`/`toDiverterCommand` anywhere. If you still see them in
  your `main.cpp`, something was left unfinished.
- **Forgetting `#include <psm/tick_result.hpp>`** (for `psm::describe`) or `<psm/engine.hpp>` (for
  `psm::Engine`).

## Something to think about

`runTicks` and `Engine::step()` implement the same order, but one is a free function operating on a
`Plant&` passed in, and the other is a method of a class that owns its own `Plant`. Which of these two
designs would be easier to accidentally let drift out of sync with the other today, if someone changed
the step order in only one place? Why is that exactly the problem `Engine` ultimately solves?

## End of Module 3

`Engine` is now the only place where the physical order of one tick exists in the code at all —
`main()` knows nothing about it, it just calls `step()`. In the modules ahead, `Engine` will keep
growing: operating modes, safety, and a second actuator (the belt motor), whose introduction we
deferred back in Module 2, are all still to come.

This is also where the checkpoint after Module 3 falls. Modules 4 onward don't have an English
version yet — for what comes next, see the [Roadmap](../../docs/roadmap.en.md).
