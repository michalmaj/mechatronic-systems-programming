[🇵🇱 Polski](00_wprowadzenie.md) | 🇬🇧 English

# 2.0 Introduction

Module 1 left you with a system that works but is idealized: a parcel would arrive, the conveyor
would "decide", and the diverter would snap into position immediately, in that same tick. No physical
device in the real world behaves like that — a motor, an actuator, or a diverter always needs time to
change position.

This module fixes exactly that. The diverter stops teleporting between `Straight` and `Diverted` — it
starts passing through an intermediate state where it's physically in motion, and that can take more
than one tick. This is the first module where `Plant` actually **waits** for a device, instead of
being handed a ready-made decision from outside.

It's also the first module in the course where `class` shows up.

## Where you're starting from

Fetch this module's starting point:

```bash
git fetch --tags
git switch -c <your-branch-name> module-02-start
```

`module-02-start` already contains **all of Module 1, working correctly** — the `misja-1` through
`misja-4` tests, plus `misja-6`, are already present and pass right away, with no work on your part.
This isn't something you rebuild from scratch — it's an inherited, ready-made foundation that this
module builds on. Your job is only the three new missions.

(If you're wondering what happened to the `misja-5` test from Module 1 — its role has been absorbed
into this module's Mission 9 test, because the way the whole tick loop works changes in this module.
More on that in Mission 9.)

## Module map

1. **Command vs. actual state** — why "what we want" and "what physically exists" are two different
   things.
2. **The diverter as a class** — the course's first class: protected internal state and a public
   interface.
3. **The conveyor waits for the diverter** — tying everything together into one correct whole.

## Before you start

- Same as in Module 1: you don't edit `CMakeLists.txt`, tests are provided by the course, and the
  amount of hand-holding decreases as you progress through the missions.
- This module is smaller than Module 1 — three missions instead of six — because it builds on tools
  you already know (enums, free functions, `std::optional`, loops) instead of introducing them from
  scratch.

**Next:** [Mission 7: command vs. actual state](./01_polecenie_a_rzeczywistosc.en.md).
