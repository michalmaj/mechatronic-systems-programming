[🇵🇱 Polski](01_final_project_brief.md) | 🇬🇧 English

# Final Project Brief: an input buffer

## Context

Parcels can arrive faster than Infeed can immediately accept them. The system needs an input buffer
in front of the sorting line.

Today (`module-09-solution`), `Scenario::arrivals` tries to call `Engine::spawnItem()` directly. If
Infeed is occupied, `runScenario()` ends with `std::nullopt` — the whole scenario doesn't count. Your
job is to change that: a momentarily occupied Infeed shouldn't automatically mean failure. An
arriving parcel should be able to wait in front of the line.

This is your capstone after the course core (Modules 0–9). You start from `final-project-start-v2` —
functionally the same simulator code as `module-09-solution`. You extend it where this task requires
it. You don't rewrite the system from scratch, and you don't work around existing abstractions with a
parallel implementation.

This brief describes **what** has to work and **how that has to be proven**. It doesn't say
**exactly how to build it** — the architecture inside the required behavior is partly your own design
decision (see "Design choices" below). You know you need a FIFO structure and that `std::deque` is
the natural container candidate — where that structure lives and how it's wrapped is yours to decide
and justify.

## Required behavior

Regardless of the design decisions you make, the following must hold in your solution:

- **The buffer is persistent domain state**, independent of `Scenario` — it exists and behaves the
  same way whether the system is driven directly (imperatively) or through `Scenario`. It is not
  private state inside `runScenario`.
- **Encapsulation.** FIFO order and capacity must be protected by a type with a private container —
  not a bare, public `std::deque<Item>` that any code could modify internally, breaking order or
  capacity without the rest of the system knowing.
- **Two separate APIs.** `Engine::spawnItem(id, mass)` stays **exactly as it is today** — low-level,
  goes straight onto Infeed, rejects on occupancy or an id collision. A new, separate domain-level API
  for accepting arrivals is added (name is yours to choose — `acceptArrival` is suggested), which is
  the only thing that uses the buffer. This new API replaces `spawnItem` inside `runScenario`'s
  arrivals phase.
- **FIFO.** No later-accepted parcel may enter Infeed ahead of an earlier parcel still waiting in the
  buffer.
- **Bounded capacity, minimum 2.** With no upper bound, the buffer would silently absorb what could be
  a real error — and that's not allowed: the last parcel never just "disappears." Overflow must be an
  explicit, observable failure.
- **The moment overflow is evaluated.** Whether "there's room in the buffer" is decided during the
  arrivals phase, against the buffer state left at the end of the previous tick — **before** anything
  else happens that tick. A slot that would only free up later in that same tick doesn't rescue an
  arrival that already found the buffer full.
- **The tick ordering contract.** Buffer→Infeed admission is resolved **after** that same tick's
  Infeed→PresenceCheck transition is resolved (so it can use an Infeed slot just freed that same
  tick). A parcel that was just admitted onto Infeed **does not move any further that same tick**.
- **Gating by belt readiness.** Accepting a parcel into the buffer (enqueue) is not gated by the
  operating mode — parcels can physically arrive even while the line is stopped. Admission from the
  buffer onto Infeed **is** gated by the same belt-readiness gate as the rest of the line
  (`beltMotor_.actualState() == Running`) — including the `RampingUp` delay (the belt needs one tick
  before it's actually moving after starting from `Stopped`). This means: an arrival that coincides
  with a fresh line start may wait in the buffer one tick longer before it enters Infeed — worth
  confirming empirically in your own tests, not assuming.
- **The "at most one arrival per tick" rule in `isValidScenario` stays unchanged.** Don't add support
  for multiple arrivals on the same tick to the shared core — that's an optional extension (see
  below).
- **Active `ItemId` uniqueness.** The public `Engine` API (`spawnItem` and your new arrival API)
  guarantees that two active parcels with the same `ItemId` can never coexist in the buffer or in any
  zone — regardless of whether the call comes from `Scenario` or from imperative code.
- **Observability.** The trace (`TickResult`) must expose at least: the number of parcels currently
  waiting in the buffer, and the `ItemId` of the parcel at the head of the queue. That's the minimum
  needed to unambiguously verify FIFO order across a trace — without bloating `TickResult` beyond
  that.
- **Determinism.** The same valid `Scenario`, run twice, must produce a semantically identical trace.
- **No new safety semantics.** Accepting arrivals into the buffer during E-Stop/Fault is a deliberate,
  pedagogical assumption about an external input stream — it is not a model of real, safety-rated
  behavior of an industrial installation. Beyond what's explicitly described above, E-Stop/Fault
  semantics don't change.

## Invariants (checked, not just declared)

1. Every `ItemId` denotes exactly one parcel — now counting the buffer and the four zones together.
2. Buffer FIFO, exactly as above.
3. No parcel is lost without an explicitly observable outcome (admission, departure, or `nullopt`
   from `runScenario` on overflow).
4. At most one move per parcel on the physical line per tick — now five points instead of four
   (Diverting, Weighing→Diverting, PresenceCheck→Weighing, Infeed→PresenceCheck, Buffer→Infeed).
   External acceptance into the buffer itself is **not** a "move on the line" and doesn't count toward
   this invariant — the counted points start at Buffer→Infeed.
5. The existing `ItemId` correlation safety in the sensors (`presenceObservedItemId` /
   `weightObservedItemId`) is unchanged — the buffer sits entirely upstream of PresenceCheck.
6. No bypassing E-Stop/Fault/RampingUp semantics.
7. Determinism is preserved.
8. The order of elements in `Scenario::arrivals` still carries no meaning.
9. Active `ItemId` uniqueness on the public `Engine` API, as above.

## Acceptance criteria

Your solution must pass a shared acceptance scenario (a description of behavior, not ready-made code
— you build it yourself as a `Scenario`):

1. The line starts, then an E-Stop follows almost immediately.
2. Exactly two arrivals occur while stopped — both parcels must go into the buffer, since the line
   isn't moving.
3. The E-Stop is released, reset, and the line restarts — the system starts draining the backlog.
4. A third arrival comes in after the resumption.
5. All three parcels eventually reach the correct outputs (according to their mass/classification).
6. The trace explicitly shows: parcels are admitted in arrival order (FIFO), none are lost, none are
   skipped during the stop.

**This must work already at the minimum allowed buffer capacity (2)** — don't design the acceptance
scenario (or its outcome) to depend on a larger capacity you happen to have chosen.

Running the same scenario twice must produce semantically identical traces — the same determinism
required since Module 9.

## Your own tests

Minimum requirement: **at least 3 small tests, covering at least 3 different categories** from the
list below (not 3 variants of the same case):

- FIFO under buffer load.
- An arrival while the buffer is busy but not full (success, where today this would fail).
- Buffer overflow — including the evaluation moment: a slot freed only within that same tick doesn't
  rescue a rejected arrival.
- Active `ItemId` uniqueness on the public `Engine` API.
- Buffer behavior through a stop, E-Stop, Fault, and recovery — with no silent parcel loss.
- Repeatability: the same `Scenario` run twice → an identical trace.

Apply exactly the same discipline you practiced in the Project Kickoff: every test has to be able to
actually turn red on broken code, not just pass on correct code.

## Demonstration through a Scenario

Beyond unit tests, your solution must include a **demonstration through `Scenario`** — a complete,
runnable scenario (similar to `recoveryDemoScenario`/`multiParcelDemoScenario` from Module 9) that
shows the buffer working in practice. The acceptance scenario from the section above can serve this
role.

## Design rationale (short, written)

Before you start coding: write a few sentences justifying two decisions (see "Design choices" below):

- **Where the buffer lives** (which entity owns it) — and why there, not somewhere else. Consider
  both directions symmetrically, not just one: is this physical state or experiment state; does the
  buffer fit better with the zones `Plant` already controls, or with the "device" components `Engine`
  controls (like `Diverter`/`BeltMotor`); does the buffer exist in imperative mode too (without
  `Scenario`) under both options; how does each option affect `Engine::step()` and testability — the
  impact differs between the two, and evaluating and justifying that difference is exactly your job
  here, not something this brief settles for you in advance.
- **How you chose the capacity** and the mechanism for setting it.

This is a short document — a dozen or so sentences is enough, not a separate report — but it will be
the starting point for your individual defense.

## Design choices — you decide and justify

- **Where the buffer is owned.** As part of `Plant` (alongside the existing zones) — or as a separate
  component owned by `Engine`, similar to `Diverter`/`BeltMotor`, which `Engine` already owns today.
  Both options are deliberately on equal footing here — neither is suggested as "more correct"; the
  choice and its justification are yours (see "Design rationale" above). The only excluded option is
  a buffer that exists as private state solely inside `runScenario`/`Scenario`, i.e. invisible to
  imperative mode.
- **The exact capacity value above the minimum of 2**, and how it's set (a constant, or a configurable
  field).
- **The exact name of the new arrival API** (`acceptArrival` is suggested, but the choice is yours).
- **The exact names and types of the new `TickResult` fields** — the information (count waiting + the
  head-of-queue id) is required; the naming isn't.
- **The buffer's internal representation** — as long as it stays private and protects FIFO order and
  capacity.
- **The naming and decomposition of new helper functions.**

## Optional extensions — beyond the required behavior

The following are **not required** for full baseline credit and sit outside the required-path budget.
Take these on only if you have the time and interest — the uneven difficulty between them is
deliberate, don't try to do all of them:

- Multiple arrivals on the same tick, with an explicit, deterministic tie-break rule (never based on
  incidental `std::vector` position).
- Richer signaling or a configurable overflow strategy.
- An `ItemId` generator.
- Differentiated arrival priorities.
- `Scenario` serialization.
- A richer trace/statistics (e.g. maximum buffer depth over a run).

## Git workflow and submitting the project

- Your starting point is the `final-project-start-v2` tag, on your own branch.
- Your commit history should show real progress — several commits reflecting successive steps (e.g. a
  sketch of the buffer type, integration with the rest of the line, extending the trace, your own
  tests, the acceptance scenario), not one giant commit at the end. No advanced Git workflow is
  required (no forced rebase/squash) — an honest, readable history on your own branch is enough.
  Committing directly to a shared branch isn't allowed.
- Partway through, it's worth doing a light, informal check-in with yourself: does the chosen
  architecture direction actually hold up when you try to justify it — it's easier to correct course
  early than late.
- Final submission: a clean branch/PR containing the implementation, your own tests, the demonstration
  scenario, and the short design rationale.

## Defense

Green tests aren't proof of understanding. After you submit the project, there's a short,
**individual** defense (even if the project was done in a pair) — explaining your own decisions,
predicting the system's behavior in new situations, and a small, previously unseen problem to solve
based on your own code. Write code you can explain, not just code that passes tests.
