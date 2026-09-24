[🇵🇱 Polski](01_paczka_i_strefy.md) | 🇬🇧 English

# 1.1 Parcel and zones

## Problem

Our sorting cell is physically one conveyor divided into several sections — let's call them
**zones**. A parcel enters at the start, passes in turn through a presence check, a scale, a
diverter, and ends up at one of two output points, depending on how much it weighs.

Before we move anything, we need two things:
- a way to **name** these places in code,
- a way to **describe one specific parcel** — where it currently is and how much it weighs.

## New C++ elements

**`enum class`** — a closed set of named values. We could write the zones as plain numbers (`0`, `1`,
`2`...), but then nothing protects us from a typo like "zone 7", which doesn't exist. `enum class`
lets you write `Zone::Infeed` instead of `0` — the compiler knows exactly which values are allowed,
and you can't accidentally write something outside that set.

```cpp
enum class Zone { Infeed, PresenceCheck, Weighing, Diverting, OutputLight, OutputHeavy };
```

**`struct`** — a way to group several related values into one whole. A parcel has an identity
(`id`), a current location (`zone`), and a weight (`mass`) — three different things, but all
describing the same parcel. `struct` lets you keep them together instead of juggling three separate
variables.

```cpp
struct Item {
    ItemId id;
    Zone zone;
    Grams mass;
};
```

## What you already have

Open [`include/psm/zone.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/include/psm/zone.hpp) and
[`include/psm/item.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/include/psm/item.hpp). Both types — `enum class Zone` and `struct Item`
— are already fully defined, exactly as shown above. You don't need to (and shouldn't) change them.

What's missing is the **behavior**: a way to turn a `Zone` value into a human-readable string. See
[`src/zone.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/src/zone.cpp):

```cpp
std::string_view toString(Zone zone) {
    // TODO (Mission 1: paczka_i_strefy): each zone should produce a different string.
    (void)zone;
    return "TODO";
}
```

## What you need to write

Fill in `toString` so that it returns the matching string for every `Zone` value — exactly the
enumerator's name as text: `Zone::Infeed` → `"Infeed"`, `Zone::PresenceCheck` →
`"PresenceCheck"`, and so on for all six zones.

A `switch` statement is a good fit for choosing between six values:

```cpp
switch (zone) {
    case Zone::Infeed: return "Infeed";
    // ...
}
```

Remove the `(void)zone;` line — it was only there so the compiler wouldn't complain about an unused
parameter before you actually used it.

## Check your work

```bash
ctest --preset test -L misja-1
```

Expected result: `100% tests passed, 0 tests failed out of 1`.

## Common mistakes

- **Missing `return` in one of the `case`s** — execution "falls through" to the next case (so-called
  fall-through) and returns the wrong string. Every `case` in this function should end with its own
  `return`.
- **A typo in the string** — the test compares the string character by character (`"PresenceCheck"`,
  not `"presence_check"` or `"Presence Check"`).
- **A missed `Zone` value** — if you forget a `case` for one of the zones, the compiler will likely
  warn about a non-exhaustive `switch`. It's worth fixing that right away instead of ignoring it.

## Something to think about

Why is `enum class Zone` a better choice here than a plain `int`, given that inside the computer it's
still just a number?

**Next:** [Mission 2: parcel movement](./02_ruch_paczki.en.md).
