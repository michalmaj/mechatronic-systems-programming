[🇵🇱 Polski](02_przesuwanie_partii.md) | 🇬🇧 English

# 8.2 Moving a batch

This is the theoretically most important mission in this module. Read it in full before you start
writing code.

## Problem

Since `Plant` can now hold several parcels at once, `advance()` has to move all of them in the right
order — in one call, without moving any single parcel more than once, but also without an artificial
delay when a vacated zone could immediately accept the next parcel behind it.

## New C++ elements

```cpp
struct ItemDeparture {
    ItemId id;
    Zone destination;
};

struct AdvanceResult {
    std::optional<SystemEventKind> event;
    std::optional<ItemDeparture> departure;
};

AdvanceResult advance(Plant& plant, const Diverter& diverter, bool routingReady = true);
```

`event` concerns only the parcel in `diverting`, if it didn't depart this tick. `departure` is set
only on a successful route — by construction, the two fields are never set at the same time.

## Transition rules, in the required order

`advance()` must perform four transitions, **each exactly once per call**, in this fixed order — from
output to input:

**1. Resolving `diverting`** (only when the slot is occupied). This is exactly the routing-deadline
rule from Mission 26 — except the `divertingWaitTicks` counter now lives on the parcel itself
(`Item::divertingWaitTicks`), not somewhere alongside it in `Engine`:
- when `routingReady` is `false`, the parcel's counter goes back to zero and this transition ends here
  for this tick (the parcel stays in `diverting`, no event);
- when `routingReady` is `true`, but the diverter hasn't settled at the commanded position yet
  (`!diverter.isSettled()`), the parcel's counter increases by one; the resulting event is
  `SystemEventKind::DiverterNotReady` on the counter's first such tick, and
  `SystemEventKind::RoutingDeadlineMissed` on every following one;
- when the diverter is settled, the parcel departs: `result.departure` gets its id and destination
  (`Zone::OutputLight` for the `Straight` position, `Zone::OutputHeavy` otherwise), and the `diverting`
  slot is freed.

**2–4. Three transitions between neighboring zones** — `weighing`→`diverting`,
`presenceCheck`→`weighing`, `infeed`→`presenceCheck`, in exactly this order. Each applies the same
pattern: if the destination zone is now empty (which may itself be the effect of a transition that
just ran, in this same call) and the source zone is occupied, the parcel moves to the destination
zone, and the source zone is freed. The one exception: a parcel entering `diverting` starts with
`divertingWaitTicks` reset to zero — this is a fresh entry into the zone, not a continuation of
someone else's countdown.

## Why this guarantees "at most one move per parcel per tick"

Not because each *slot* is touched only once — a slot is read as a source by one transition and
checked as a destination by its neighbor, so it's referenced more than once. The guarantee comes from
the fact that each of the four *transitions* (resolving `Diverting`, `Weighing`→`Diverting`,
`PresenceCheck`→`Weighing`, `Infeed`→`PresenceCheck`) runs **exactly once per call, in this fixed
order, from output to input**. A parcel that reaches a zone through one transition can't be picked up
by an *earlier* transition in the same call — because that earlier transition has already run. At the
same time, each transition checks whether its target is occupied at the moment it runs — which may
already reflect the effect of an earlier transition from this same tick — so a freed spot is
immediately visible to the transition behind it, letting several parcels move together with no
artificial gap.

## Where the diverter command comes from

The diverter command decided in a given `Engine::step()` always comes from the classification of the
parcel that's **already** in `diverting` — never from a parcel just classified in `weighing` in that
same tick, because that parcel can't have reached `diverting` before `advance()` is called, and the
command is decided before that call. See the example run below — it shows this clearly.

## Example run: three parcels, Light/Heavy/Light

```text
tick 0: infeed=2 presenceCheck=1 weighing=- diverting=-  cmd=Hold(for -)      event=-                 departure=-
tick 1: infeed=3 presenceCheck=2 weighing=1 diverting=-  cmd=Hold(for -)      event=-                 departure=-
tick 2: infeed=- presenceCheck=3 weighing=2 diverting=1  cmd=Hold(for -)      event=-                 departure=-
tick 3: infeed=- presenceCheck=- weighing=3 diverting=2  cmd=Hold(for 1)      event=-                 departure=1->Light
tick 4: infeed=- presenceCheck=- weighing=3 diverting=2  cmd=Divert(for 2)    event=DiverterNotReady  departure=-
tick 5: infeed=- presenceCheck=- weighing=- diverting=3  cmd=Divert(for 2)    event=-                 departure=2->Heavy
tick 6: infeed=- presenceCheck=- weighing=- diverting=3  cmd=Hold(for 3)      event=DiverterNotReady  departure=-
tick 7: infeed=- presenceCheck=- weighing=- diverting=-  cmd=Hold(for 3)      event=-                 departure=3->Light
```

(This trace comes from a bare `Plant`/`advance()`, called directly, without `Engine` — without belt
startup. Through a real `Engine` the tick numbers will differ, because the belt needs its own time to
ramp up; the pattern of events itself stays the same.)

Notice tick 3: parcel 1 departs, and parcel 2 enters the just-freed `diverting` in that same
`advance()` call — yet the command decided that tick still concerned parcel 1 (decided before
`advance()`). That's a direct, visible illustration of the rule from the section above.

## What you already have

[`include/psm/plant.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/include/psm/plant.hpp) — `ItemDeparture`, `AdvanceResult`, the
`advance()` signature — all ready. `spawnItem` from Mission 29 is already yours.

## What you need to write

Implement the body of `advance()` in [`src/plant.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-08-start/src/plant.cpp) according to the transition
rules described above: four transitions, in the given order, each performed once per call.

## Check your work

```bash
ctest --preset test -L misja-30
```

Expected result: `100% tests passed`. The tests check: a parcel passing through all four zones with
the diverter command supplied late (`plant_diverter_test`); the full routing deadline on a parcel's
own `divertingWaitTicks`, including the reset on `!routingReady` (`plant_deadline_test`).

## Common mistakes

- **Processing zones from input to output** — then a parcel could move twice in one call (once as
  "entering the next zone," once as "leaving it" a moment later in the same run).
- **Forgetting to reset `divertingWaitTicks` on entering `diverting`** — the next parcel would inherit
  someone else's partially used counter.
- **Inferring the diverter command from the parcel in `weighing`** — break this deliberately in a
  prototype to see why the test rejects it.

## Something to think about

If `advance()` processed zones in the opposite order (from input to output), which exact behavior from
the run above would break, and specifically how?

**Next:** [Mission 31: per-parcel correlation](./03_korelacja_per_paczka.en.md).
