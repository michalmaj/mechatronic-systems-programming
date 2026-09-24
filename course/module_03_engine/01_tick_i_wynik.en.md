[🇵🇱 Polski](01_tick_i_wynik.md) | 🇬🇧 English

# 3.1 Tick and result

## Problem

In the next mission you'll build `Engine` — something that runs one full simulation cycle at a time.
Before you do, you need a way to describe the **result** of such a cycle: one immutable set of
information about what just happened. And once such a description exists, it'll also help to have a
way to turn it into human-readable text.

## New C++ elements

**`using Tick = std::uint64_t;`** — a type alias. `Tick` is still a plain integer (unsigned, 64-bit —
a tick counter should never "wrap around" back to zero), but the name `Tick` tells a reader of the
code exactly what that number represents, instead of leaving them with a bare `int`.

**`struct TickResult`** — a snapshot of one tick:

```cpp
struct TickResult {
    Tick tick;
    std::optional<Item> item;
    DiverterCommand diverterCommand;
    DiverterPosition diverterActual;
};
```

This is a plain `struct`, with no invariant to protect — a purely passive bundle of data, the same
choice as `Item` or `Plant` in Module 1. Notice that the snapshot carries **both** halves of the
distinction from Module 2: `diverterCommand` (what we asked for) and `diverterActual` (what actually
happened) — leave either one out, and the result wouldn't tell the whole truth about that tick.

## What you already have

[`include/psm/tick.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/include/psm/tick.hpp) and
[`include/psm/tick_result.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/include/psm/tick_result.hpp) — `Tick` and `TickResult` already fully
defined. The `describe` declaration is already there too:

```cpp
std::string describe(const TickResult& result);
```

[`src/tick_result.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/src/tick_result.cpp) has an empty skeleton:

```cpp
std::string describe(const TickResult& result) {
    // TODO (Mission 10: tick_i_wynik): ...
    (void)result;
    return "TODO";
}
```

## What you need to write

Fill in `describe` so that it returns:
- `"tick T: item ID in zone Z"` when `result.item` has a value (`T` = `result.tick`, `ID` =
  `result.item->id`, `Z` = `psm::toString(result.item->zone)` from Module 1),
- `"tick T: empty"` when `result.item` has no value.

You'll need `#include <psm/zone.hpp>` (for `psm::toString`) and a way to turn numbers into text —
`std::to_string` from `<string>` (already pulled in by `tick_result.hpp`) handles that with no extra
effort.

`describe` has no caller anywhere in the program yet — that's deliberate. It gets its first caller
only in Mission 12.

## Check your work

```bash
ctest --preset test -L misja-10
```

Expected result: `100% tests passed, 0 tests failed out of 1`. The test checks both situations —
a parcel present and an empty conveyor — comparing against the exact expected string.

## Common mistakes

- **Wrong string format** — the test compares character by character. Check spaces and colons.
- **Using `result.item->id` without checking `has_value()` first** — as always with `std::optional`,
  check first, then read.
- **Forgetting `#include <psm/zone.hpp>`** — `psm::toString(Zone)` isn't automatically visible through
  `tick_result.hpp` alone.

## Something to think about

`TickResult` stores both `diverterCommand` and `diverterActual`, even though Module 2 already
introduced both concepts separately on `Diverter`. Why repeat that information in the snapshot, when
in theory it could always be pulled directly from the `Diverter` object?

**Next:** [Mission 11: engine formalizes the order](./02_silnik_formalizuje_kolejnosc.en.md).
