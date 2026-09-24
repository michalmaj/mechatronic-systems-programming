[🇵🇱 Polski](03_stan_przenosnika.md) | 🇬🇧 English

# 1.3 Conveyor state

## Problem

So far you've worked with a single parcel that you always assumed exists. But a real conveyor is
sometimes empty — before the first parcel arrives, or after the last one leaves. `Item` by itself
can't express "there's nothing here" — it's always *some* specific parcel, with a concrete `id`,
`zone`, and `mass`.

You also need something that represents the **whole conveyor**, not just a single parcel — the place
where that possible parcel lives.

## New C++ elements

**`std::optional<Item>`** — a type that either holds an `Item` value, or is empty (`std::nullopt`).
That maps exactly onto the question "is something here": `std::optional<Item>` with a value means
"yes, here it is"; empty means "no, the conveyor is free". Without `std::optional` you'd have to
invent workarounds like "a special id meaning no parcel", which is easy to confuse with a real
parcel.

**`struct Plant`** — groups the conveyor's state. For now that's exactly one field:

```cpp
struct Plant {
    std::optional<Item> item;
};
```

This is deliberately a plain `struct`, not a `class` with private fields and methods. At this stage
there's nothing we need to protect against misuse — encapsulation makes sense when there's an
invariant to guard, and we don't have one yet. Once such an invariant shows up (in later modules),
that will be the concrete reason to switch to `class` — not before.

**Free functions operating on `Plant&`** — just as `advanceZone` from Mission 2 operated on `Item&`,
you'll now write functions operating on `Plant&`, and **reuse `advanceZone`** rather than writing the
movement logic again from scratch.

## What you already have

[`include/psm/plant.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/include/psm/plant.hpp):

```cpp
struct Plant {
    std::optional<Item> item;
};

void spawnItem(Plant& plant, Item item);
void advance(Plant& plant, DiverterPosition diverterPosition);
```

Notice the `diverterPosition` parameter in `advance` — **ignore it** for now. It comes back into play
in Mission 4; here you just need to know it's there, so the function's signature won't have to change
later.

[`src/plant.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/src/plant.cpp) contains empty skeletons for both functions with `// TODO` comments.

## What you need to write

**`spawnItem(Plant&, Item)`** — if `plant.item` is currently empty, place the given parcel into it
(in zone `Infeed`). If the conveyor is already occupied, do nothing — the new parcel simply waits (in
this module we don't model an input queue, we just reject the attempt).

**`advance(Plant&, DiverterPosition)`** — if `plant.item` has a value and that parcel is **not** yet
in `Diverting`, move it one zone forward (use `advanceZone` from Mission 2, called on `*plant.item`).
If `plant.item` is empty — do nothing. Leave the behavior at `Diverting` unchanged for now (the
parcel waits) — that's Mission 4's job again.

## Check your work

```bash
ctest --preset test -L misja-3
```

Expected result: `100% tests passed, 0 tests failed out of 1`. The test checks an empty `Plant`,
correctly adding a parcel, rejecting a second `spawnItem` attempt while the conveyor is occupied, and
that three `advance` calls in a row bring the parcel to `Diverting`.

## Common mistakes

- **Forgetting to check `has_value()`** — calling `*plant.item` on an empty `std::optional` is
  undefined behavior (the program might crash, or worse, appear to "work" while producing wrong
  results). Always check `has_value()` (or the equivalent `if (plant.item)` condition) before using
  `*plant.item`.
- **Overwriting an occupied conveyor in `spawnItem`** — remember the "only if empty" condition.
- **Rewriting the zone-advancing logic from scratch** instead of calling `advanceZone` — in this
  mission you don't need to (and shouldn't) duplicate what you already have from Mission 2.

## Something to think about

Why is `std::optional<Item>` a better choice than, say, adding a `bool exists` field to `Item`?

**Next:** [Mission 4: sorting decision](./04_decyzja_sortowania.en.md).
