[🇵🇱 Polski](01_partie_i_paczki.md) | 🇬🇧 English

# 8.1 Batches and parcels

## Problem

Until now, `Plant` held exactly one parcel. A second `spawnItem` attempt was silently ignored. To
model several parcels at once, `Plant` needs a separate place for each zone — and a way to tell apart
the parcels that are in it at the same time.

## New C++ elements

```cpp
struct Item {
    ItemId id;
    Grams mass;
    bool presenceConfirmed = false;
    std::optional<WeightClass> classification;
    int divertingWaitTicks = 0;
};
```

`Item` no longer has a `zone` field. Since `Plant` will have a separate field for each zone, zone
membership is a fact that follows from which field a given parcel sits in — keeping an extra `zone`
field alongside that would be two sources of the same truth that could drift apart.

```cpp
struct Plant {
    std::optional<Item> infeed;
    std::optional<Item> presenceCheck;
    std::optional<Item> weighing;
    std::optional<Item> diverting;
};

bool spawnItem(Plant& plant, ItemId id, Grams mass);
```

Each field holds at most one parcel. `spawnItem` now takes only `ItemId` and `Grams` — not a whole
`Item` — and constructs a fresh `Item` internally. Since the caller has no way to pass in a "used"
`Item` with non-zero processing state, there's nothing to clear either.

## The contract: what `spawnItem` must guarantee

`ItemId` must be unique among the parcels **currently present** in `Plant` — not among every parcel
that has ever existed. When a parcel leaves the system (departing to `OutputLight`/`OutputHeavy`), its
id is free to be reused. There's no global id registry, and none is needed — `spawnItem` rejects a
collision by checking only what's present *right now*.

The function must return `false` and leave `plant` completely unchanged in two independent cases:

- `infeed` is already occupied — there's nowhere to place the new parcel,
- the given `id` collides with the id of any *other* parcel present anywhere in `Plant`, i.e. in
  `presenceCheck`, `weighing`, or `diverting` (a collision with `infeed` itself is already covered by
  the first condition, so it doesn't need a separate check).

In every other case, the function places a freshly constructed `Item` with the given `id` and `mass`
into `infeed` and returns `true`. The caller (eventually `Engine`, and above it the CLI/tests) is
responsible for retrying on the next tick and for choosing an id that doesn't collide — there's no
internal queue or id generator here.

## What you already have

[`include/psm/item.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/include/psm/item.hpp) — the new `Item` shape, complete.

[`include/psm/plant.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/include/psm/plant.hpp) — the new `Plant` shape, `ItemDeparture`,
`AdvanceResult`, the `spawnItem`/`advance` signatures — all ready.

[`src/plant.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/plant.cpp) — `advance()` deliberately does nothing yet (that's
Mission 30's job, not this one) — don't rely on anything moving until you've written Mission 30. The
body of `spawnItem` is `// TODO`.

## What you need to write

Fill in the body of `spawnItem` in [`src/plant.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/plant.cpp) so it satisfies the contract
described above.

## Check your work

```bash
ctest --preset test -L misja-29
```

Expected result: `100% tests passed`. The test checks: a successful spawn into an empty `infeed`;
rejection when `infeed` is occupied; rejection on an id collision with a parcel present anywhere else
in `Plant`; that a later call with an id that doesn't collide succeeds despite another parcel being
present.

## Common mistakes

- **Checking for an id collision only against `infeed`** — a collision with a parcel in
  `presenceCheck`/`weighing`/`diverting` must be rejected just as firmly.
- **Manually clearing `presenceConfirmed`/`classification`/`divertingWaitTicks` after construction** —
  unnecessary: `Item{id, mass}` already takes the rest of the fields from their default values in the
  class.
- **Trying to accept a whole `Item` instead of `ItemId`/`Grams`** — that's exactly the ambiguity
  ("what does it mean to pass in a 'used' `Item`?") that the narrower signature is meant to avoid.

## Something to think about

`spawnItem` has no internal queue — a second, failed attempt is simply lost unless the caller retries.
Why is that the right responsibility for the caller (e.g. `Engine` or the CLI) to own, rather than
`Plant` itself?

**Next:** [Mission 30: moving a batch](./02_przesuwanie_partii.en.md).
