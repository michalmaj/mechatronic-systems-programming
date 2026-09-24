[🇵🇱 Polski](00_wprowadzenie.md) | 🇬🇧 English

# Module 8: multiple parcels at once

## Where we are

Since Module 1, `Plant` has modeled exactly one parcel at a time — `std::optional<Item> item`, one
field. A real sorting line doesn't work that way: several parcels are physically in flight at once,
each in a different zone, each at a different stage of the process. This module changes that.

## What changes

`Item` loses its `zone` field — since `Plant` will have a separate field for each zone, a parcel's
zone membership is a fact that follows from which field it sits in, not a separate marker that could
drift out of sync with that fact. In exchange, `Item` gains its own processing state:
`presenceConfirmed`, `classification`, `divertingWaitTicks` — what used to live in one global
`ControllerState` now travels with the specific parcel, because with several parcels in flight, sensor
readings in the same tick can concern completely different `ItemId`s.

`Plant` gains four named fields: `infeed`, `presenceCheck`, `weighing`, `diverting`. Each holds at
most one parcel — a natural, physical constraint: a parcel can't enter an occupied zone. This means
the diverter never has to arbitrate between two parcels at once — `diverting` always holds at most
one.

`advance()` now performs four transitions instead of one, in a fixed order, from output to input
(`Diverting` → `Weighing` → `PresenceCheck` → `Infeed`) — this lets a parcel enter, in the same tick, a
zone just vacated by another parcel, with no artificial delay, while still guaranteeing that no parcel
ever moves more than once in one tick.

The output (`OutputLight`/`OutputHeavy`) stops being a `Plant` field — it becomes a sink: a departing
parcel is reported as an `ItemDeparture` and disappears from `Plant` in the same `advance()` call,
never silently.

## Four missions

- **Mission 29 — batches and parcels.** The new `Item`/`Plant` shape, `spawnItem(id, mass)`.
- **Mission 30 — moving a batch.** The full `advance()` algorithm, in output-to-input order.
- **Mission 31 — per-parcel correlation.** `ControllerState` is retired; two new, explicitly separate
  correlation functions take its place.
- **Mission 32 — the engine with multiple parcels.** Full integration into `Engine::step()`, an
  extended `TickResult`/`describe()`, a demonstration in the CLI.

## Before you start

This module can't be built incrementally the way earlier ones were — Module 7's `Engine::step()`
refers directly to `Plant::item` and `Item::zone`, which no longer exist at all. That's why this
module's starting point looks different: it compiles right away, but `Engine::step()` deliberately
does nothing yet — it doesn't move any parcel, until you write Mission 32. That's normal and
intentional, not a bug in the starting point.

**Next:** [Mission 29: batches and parcels](./01_partie_i_paczki.en.md).
