[🇵🇱 Polski](02_silnik_formalizuje_kolejnosc.md) | 🇬🇧 English

# 3.2 Engine formalizes the order

This is the module's second, and theoretically most important, mission — it gets a bit more room than
the others.

## Problem

The one-tick order from Mission 9 of Module 2 is correct, but it only lives by convention — it's
manually duplicated in both `runTicks` and `main()`. Open [`src/loop.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/src/loop.cpp) and see that order
with your own eyes — it's exactly the same sequence you're about to lock down in one place. Nothing
today guarantees the two separate copies of that sequence won't drift apart, and nothing proves they
agree with each other.

## New C++ element: a second class, this time **composition**

`Diverter` (Module 2) protected a single field against invalid changes. `Engine` is a different kind
of class — it demonstrates **composition**: `Engine` *owns* `Plant` and `Diverter` as plain member
fields, not references or pointers:

```cpp
class Engine {
public:
    void spawnItem(Item item);
    TickResult step();

private:
    Tick tick_ = 0;
    Plant plant_;
    Diverter diverter_;
};
```

`plant_` and `diverter_` live and die together with `Engine` — they don't exist anywhere else, and
nobody else holds access to them. This is a different use of `class` than in Module 2, but the same
mechanism (encapsulation) and the same naming convention (§1 Module 2: "a type with hidden state and a
public interface is called a `class`").

## The invariant — the precise version

**Physically advancing the simulation** (the `tick_` counter, moving `Plant`, settling `Diverter`)
happens **exclusively** inside `step()`, in the fixed order below — never independently, from outside
`Engine`.

**`spawnItem()` is an explicit exception** — it isn't "advancing the simulation," it's how a parcel
enters the system in the first place. Calling it between ticks is fully allowed and expected — just as
you did in Modules 1 and 2 with `spawnItem(Plant&, Item)`.

## The order of `step()` — step by step

1. If `plant_`'s current parcel has a value: decide its `DiverterCommand` through the free functions
   `classify` and `toDiverterCommand` (`Engine` **does not own** a Controller — there's no Controller
   object at all, just stateless functions that `step()` simply calls, as the first step, before
   `setCommand`).
2. `diverter_.setCommand(...)`.
3. `diverter_.resolve()`.
4. `psm::advance(plant_, diverter_)` — a **free function** from Module 1/2, not a method. `Plant` is
   still a plain `struct`, with no methods — for the same reason, `Engine::spawnItem` has to call the
   free function `psm::spawnItem(plant_, item)`, **never** `plant_.spawnItem(item)` (no such method
   exists).
5. Assemble and return the `TickResult` for the tick you just processed — the tick number is `tick_`
   **before** incrementing it — and only then increment `tick_`.

## The numbering rule

The first call to `step()` returns `tick == 0`. `TickResult` describes the state **after** that tick
has been processed — the first call already reflects the full decision→`setCommand`→`resolve`→`advance`
cycle, not the state before it.

## What you already have

[`include/psm/engine.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/include/psm/engine.hpp) — declarations complete, as shown above.

[`src/engine.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-03-start/src/engine.cpp) — empty skeletons for both methods, with `// TODO` comments.

## What you need to write

- `Engine::spawnItem(Item)` — one line: call the free function `psm::spawnItem(plant_, item)`.
- `Engine::step()` — the five steps described above, in that exact order.

## Check your work

```bash
ctest --preset test -L misja-11
```

Expected result: `100% tests passed, 0 tests failed out of 1`. The test runs six consecutive `step()`
calls on a 750-gram parcel and checks the tick number, the parcel's zone, and
`diverterCommand`/`diverterActual` at several of them — the same dependencies you already saw in
Module 2's tests, now verified by a single call instead of four manually arranged ones.

## Common mistakes

- **Swapping the order of step 3 and step 4** — `resolve()` has to run before `advance()`, exactly as
  in Mission 9 of Module 2. Swapping them shifts every routing decision by one step.
- **Calling `plant_.spawnItem(item)` or `plant_.advance(diverter_)`** — `Plant` has no such methods.
  The compiler will catch it, but the error message can be less than obvious the first time you meet
  it.
- **Setting the tick number in `TickResult` after incrementing instead of before** — the first
  `step()` would then return `tick == 1`, not `tick == 0`, breaking the numbering rule above.

## Something to think about

`Engine` "owns" `Plant` and `Diverter` as plain fields, not references. What would change if `Engine`
instead stored references to a `Plant`/`Diverter` created somewhere else? Would
`Engine::spawnItem`/`step()` still make sense?

**Next:** [Mission 12: rewiring to the engine](./03_przepiecie_na_silnik.en.md).
