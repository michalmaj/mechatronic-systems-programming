[🇵🇱 Polski](02_dywerter_jako_klasa.md) | 🇬🇧 English

# 2.2 The diverter as a class

This is the most theoretically important mission in this module — we'll spend a bit more time on it
than usual, since it introduces the first class in the whole course.

## Problem

A real diverter doesn't jump instantly from one position to another — it has to physically move, and
that takes time. We need a type that reflects this: it remembers what we want from it
(`DiverterCommand`), it remembers where it actually is (`DiverterPosition`), and it **doesn't let
anyone from outside set that second value to whatever they like** — the only way the actual position
can change is by calling one specific method, one step at a time.

## Why `class`, not `struct`

It's important not to get this backwards: in C++, a `struct` can also have private fields and protect
exactly the same invariant as a `class` — the only technical difference between `struct` and `class`
is the default access level (`public` versus `private`). It's not the keyword `class` itself that
protects us.

The actual protection mechanism is **encapsulation** — a `private` section plus a public interface.
We use `class` here as a **deliberate naming convention** for this course: a type with hidden internal
state and a public interface that protects it is called a `class`, so that the type's name itself
signals "there's an invariant here to protect." `Plant` in Module 1 was a `struct` precisely because
it wasn't protecting anything back then — there was no invariant at all.

## New C++ element: `class`

```cpp
class Diverter {
public:
    void setCommand(DiverterCommand command);
    void resolve();
    DiverterPosition actualPosition() const;
    bool isSettled() const;

private:
    DiverterCommand command_ = DiverterCommand::HoldStraight;
    DiverterPosition actual_ = DiverterPosition::Straight;
};
```

- `public` — the methods through which the outside world interacts with the diverter.
- `private` — the fields `command_`/`actual_`, inaccessible from outside in any way other than
  through those methods.
- `const` at the end of `actualPosition()`/`isSettled()` — these methods only read state, they never
  change it (you'll get to know this in more depth once you write your own classes in later modules;
  for now it's enough to know it's a promise that "this method changes nothing").

The **declarations** (method names, their signatures, the fields together with their initial values)
are already complete — what you'll write are the **bodies** of the four methods.

## The invariant we're protecting

`actual_` can change only through `resolve()`, and only by exactly one step at a time, following the
table below. Nothing else in the code is allowed to assign an arbitrary value to `actual_`.

## `resolve()` transition table

The target position (`target`) depends on `command_`: `HoldStraight` → `Straight`, `Divert` →
`Diverted`.

| `actual_` before | `command_`   | `target`   | `actual_` after |
|-------------------|--------------|------------|-----------------|
| `Straight`        | HoldStraight | `Straight` | `Straight` (unchanged — already there) |
| `Straight`        | Divert       | `Diverted` | `Moving` (starts moving) |
| `Diverted`        | HoldStraight | `Straight` | `Moving` (starts moving) |
| `Diverted`        | Divert       | `Diverted` | `Diverted` (unchanged — already there) |
| `Moving`          | HoldStraight | `Straight` | `Straight` (arrives) |
| `Moving`          | Divert       | `Diverted` | `Diverted` (arrives) |

Each call to `resolve()` advances things by at most one step: correctly-positioned stays put,
incorrectly-positioned starts moving, already-moving arrives.

## `isSettled()` semantics

`isSettled()` is a **live comparison**, recomputed on every call — never a previously remembered
answer:

```cpp
return actual_ == target(command_);
```

This has a concrete consequence: if you call `setCommand(...)` and **don't** yet call `resolve()`,
`isSettled()` must immediately reflect the new situation. Example: `actual_ == Straight`, `command_ ==
HoldStraight` → `isSettled() == true`. Now you call `setCommand(Divert)`, but not yet `resolve()` —
`isSettled()` must immediately return `false` (the target is now `Diverted`, and `actual_` hasn't
moved yet). If `isSettled()` returned a previously stored flag instead of recomputing it, it would
give the wrong answer here.

## What you need to write

In [`src/diverter.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-02-start/src/diverter.cpp) fill in four methods:
- `setCommand(DiverterCommand)` — store the argument in `command_`.
- `resolve()` — implement the transition table above.
- `actualPosition() const` — return `actual_`.
- `isSettled() const` — compute and return the comparison described above.

## Check your work

```bash
ctest --preset test -L misja-8
```

Expected result: `100% tests passed, 0 tests failed out of 1`. The test walks through the full
transition table in both directions (`Straight`→`Diverted` and back) and checks `isSettled()`
immediately after `setCommand()`, before any `resolve()`.

## Common mistakes

- **`isSettled()` computed from an old, stored value** instead of a live comparison — this mistake is
  discussed above.
- **`resolve()` jumping straight to the target** from `Straight`/`Diverted` (skipping `Moving`) —
  remember: an unsettled position always passes through `Moving` first, and only the next `resolve()`
  call reaches the target.
- **Forgetting the "already there" case** — if `actual_ == target`, `resolve()` does nothing (there's
  no reason to move a diverter that's already there).

## Something to think about

If you wrote a `struct` with exactly the same `public`/`private` split instead of a `class` with a
`private` section, would the code behave any differently? What actually differs between the two
approaches in this specific case?

**Next:** [Mission 9: the conveyor waits for the diverter](./03_przenosnik_czeka_na_dywerter.en.md).
