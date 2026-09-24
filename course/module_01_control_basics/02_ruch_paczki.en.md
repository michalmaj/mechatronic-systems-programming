[🇵🇱 Polski](02_ruch_paczki.md) | 🇬🇧 English

# 1.2 Parcel movement

## Problem

In Mission 1 you learned to name zones and describe a single parcel. But a parcel doesn't move by
itself — you need something that takes a parcel sitting in one zone and moves it to the next.

The order of the zones is fixed: `Infeed` → `PresenceCheck` → `Weighing` → `Diverting`. From there,
at `Diverting`, the parcel will get stuck for now — and that's **deliberate**. The diverter still has
to decide which way to route the parcel, and we won't deal with decisions until Mission 4. For now
you're only concerned with movement "one zone forward".

## New C++ elements

**A free function (not a method)** taking `Item&` — a reference to the parcel it's meant to change.
A reference (`&`) means the function doesn't get a copy of the parcel, but access to the same,
original variable — the changes it makes are visible after the function returns, too.

```cpp
void advanceZone(Item& item);
```

If the parameter were of type `Item` (without `&`), the function would get a copy — it could change
that copy freely, but the original parcel at the call site would stay untouched. Here it's the
opposite we want: we want to change the state of an existing parcel.

**`switch` over an `enum class`** — you already saw this in Mission 1 when converting to text; here
you use it to decide "what's the next zone", instead of returning a string.

## What you already have

[`include/psm/item_motion.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/include/psm/item_motion.hpp) declares the function you'll
fill in:

```cpp
void advanceZone(Item& item);
```

[`src/item_motion.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/src/item_motion.cpp) contains its empty skeleton:

```cpp
void advanceZone(Item& item) {
    // TODO (Mission 2: ruch_paczki): move item.zone one zone forward
    // (Infeed -> PresenceCheck -> Weighing -> Diverting). Stop at Diverting --
    // what happens next is Mission 4's job.
    (void)item;
}
```

## What you need to write

Fill in the body of `advanceZone` so that:
- `Infeed` changes to `PresenceCheck`,
- `PresenceCheck` changes to `Weighing`,
- `Weighing` changes to `Diverting`,
- `Diverting` **stays unchanged** (the parcel waits for a decision we don't know how to make yet).

A `switch` over `item.zone`, assigning a new value to `item.zone` in each case, is the natural tool
for this job.

## Check your work

```bash
ctest --preset test -L misja-2
```

Expected result: `100% tests passed, 0 tests failed out of 1`. The test calls `advanceZone` four
times on a single parcel and checks that after the third call the parcel is in `Diverting`, and after
the fourth it's still in `Diverting`.

## Common mistakes

- **No case for `Diverting`** — if you leave it out, a `switch` without `default` simply does nothing
  for that value, which happens to give the correct effect here, but it's better to add it explicitly
  (`case Zone::Diverting: break;`), so it's clear this is a deliberate decision, not an oversight.
- **Changing `item` instead of `item.zone`** — remember you're only modifying the `zone` field, not
  the whole parcel (`id` and `mass` are meant to stay unchanged).
- **Trying to "skip" `Diverting` straight to an output zone** — that's not this mission's job; that
  temptation is a good sign you've bumped into a problem that Mission 4 will solve.

## Something to think about

The parcel gets stuck at `Diverting`. Is that a bug in your code, or correct behavior at this stage
of the project? How do you know?

**Next:** [Mission 3: conveyor state](./03_stan_przenosnika.en.md).
