[🇵🇱 Polski](handbook.md) | 🇬🇧 English

# Course Handbook

← [README](../README.en.md) · [Roadmap](roadmap.en.md)

Coherent teaching material for Course Core 0–9 and an introduction to the final project. The source of
truth for every chapter is the current code and tests of the matching tag (`module-XX-start` /
`module-XX-solution`) — if anything here looks different from the code you're working with, the code
wins; please report it via Issues.

**Contents:** [0](#0-how-to-use-this-handbook) · [1](#1-modeling-a-simple-process) ·
[2](#2-a-command-is-not-physical-state) · [3](#3-orchestrating-the-system) ·
[4](#4-an-actuator-and-an-operating-mode) · [5](#5-an-independent-e-stop-path) ·
[6](#6-sensors-and-data-quality) · [7](#7-faults-and-system-events) ·
[8](#8-multiple-parcels-and-invariants) · [9](#9-scenarios-as-data) ·
[10](#10-how-to-read-and-write-a-test) ·
[11](#11-the-final-project--how-to-approach-a-new-requirement) ·
[Appendix A](#appendix-a--glossary) · [Appendix B](#appendix-b--commands) ·
[Appendix C](#appendix-c--type--api-map)

---

## 0. How to use this handbook

This course teaches through three recurring steps: **code → experiment → test.** You write a piece of
implementation, run the simulator (via the CLI or a test), look at the result, and only then confirm it
with the provided test. Not the other way around — the test exists to confirm what you've already
observed, not to guess what the code is supposed to do.

A few things hold true throughout the course and are worth understanding once, up front:

- **The simulation runs in discrete, deterministic steps — ticks.** Every call to `Engine::step()` is
  one tick: read the current state, decide what changes, return the result (`TickResult`). The same
  sequence of inputs always produces the same sequence of results.
- **Tests are provided by the course.** You don't write them yourself until Project Kickoff (chapter
  10) — your job is to understand and run them, not design them from scratch. A test is a mission's
  executable contract: a green result confirms the required behavior and finishes the mission, but it
  isn't proof of understanding on its own — that's what the checkpoints after Modules 3, 7, and 9
  check.
- **CMake/CTest are tools, not a subject to learn.** Configure a preset, build, run tests — that's all
  you need to know about CMake itself in this course (see Appendix B).
- **You work on tags.** Every module starts with `git switch -c <your-branch> module-XX-start`. You
  commit on your own branch; the course tags stay untouched and are your reference point.

---

## 1. Modeling a simple process

**Module 1 · `module-01-start` · missions 1–6**

You start with the smallest possible version of the problem: **one parcel, one zone at a time.** A
parcel arrives, gets classified, and "moves" through zones — no actuator yet, no reaction time, no
multiple parcels at once.

The first domain model:

```cpp
enum class Zone { Infeed, PresenceCheck, Weighing, Diverting, OutputLight, OutputHeavy };

struct Item {
    ItemId id;
    Zone zone;
    Grams mass;
};

struct Plant {
    std::optional<Item> item;
};
```

This is deliberately the simplest possible shape — one `std::optional<Item>`, no container. Here you
pick up the basic C++ vocabulary the course uses everywhere after this: `enum class` for a closed set
of states, `struct` for plain data, `std::optional` for "might not have a value" instead of a pointer
or a sentinel constant, and free functions (`spawnItem`, `advance`, `classify`) as the basic unit of
logic — no classes yet.

The first control loop in the CLI (simplified) already shows the whole rhythm of the course: decide a
command from the current state, take a step, print the result. This rhythm — *decide → step → observe*
— comes back in every later module, just with progressively richer state.

---

## 2. A command is not physical state

**Module 2 · `module-02-start` · missions 7–9**

The key lesson of this module: **sending a command to a device does not mean the device has already
carried it out.** A diverter commanded to move to `Diverted` needs time to get there. Until you model
that, code that assumes instantaneous movement is simply wrong — even if it compiles and "looks fine."

```cpp
enum class DiverterCommand { HoldStraight, Divert };
enum class DiverterPosition { Straight, Moving, Diverted };

class Diverter {
public:
    void setCommand(DiverterCommand command);
    void resolve();
    DiverterPosition actualPosition() const;
    bool isSettled() const;
};
```

This is your first own class in the course. You learn encapsulation not as an abstract principle but as
a concrete tool: `Diverter` protects the distinction between what you **commanded** (`command`) and
what is **actually happening** (`actualPosition`) — nothing else in the system should be able to
overwrite the actual state directly. This *command/requested vs. actual* pattern comes back later with
`BeltMotor` (Module 4) and is one of the most important design habits of the whole course.

`Plant::advance()` now has to **wait** until the diverter has settled (`isSettled()`) before a parcel
can actually leave — the first time a duration (a number of ticks) becomes part of the logic, not just
a loop counter.

---

## 3. Orchestrating the system

**Module 3 · `module-03-start` · missions 10–12**

Up to now, `main()` in the CLI did all the orchestration by hand. In this module that responsibility
moves to one place: `Engine`.

```cpp
struct TickResult { /* a full, unambiguous description of one tick */ };

class Engine {
public:
    bool spawnItem(ItemId id, Grams mass);
    TickResult step();
private:
    Plant plant_;
    Diverter diverter_;
};
```

Key concept: **composition, not inheritance.** `Engine` *owns* `Plant` and `Diverter` as members — it
does not inherit from either, and it is not either of them. This distinction ("has-a" vs. "is-a") comes
back repeatedly in the course and is one of the more common wrong mental shortcuts: `Engine`
**composes** domain objects and **calls** control logic (functions from `Controller`), but the control
logic itself stays a collection of free functions, not `Engine` methods.

`TickResult` is a new, important pattern: instead of many separate getters, `Engine::step()` returns
**one, complete value** describing everything that happened in that tick. Tests and the CLI only ever
look at this value — they never need to ask `Engine` about mid-tick state.

This module also formalizes the **order of operations within a single step** — something that was
implicit in Module 1 and becomes here an explicit, tested contract.

---

## 4. An actuator and an operating mode

**Module 4 · `module-04-start` · missions 13–15**

A production line can't just "run" — it needs an **operating mode**, and its main drive needs to ramp
up and down, not switch instantly.

```cpp
enum class BeltMotorCommand { Stop, Run };
enum class BeltMotorState { Stopped, RampingUp, Running, RampingDown };
enum class Mode { Idle, Running };
```

(`Mode` grows in later modules — `EStopped` arrives in Module 5, `Fault` in Module 7. That's not an
oversight here: each state shows up together with the mechanism that needs it.)

`BeltMotor` repeats the command/actual pattern from Module 2, with an important addition: the
transition between states takes more than one tick (`Stopped → RampingUp → Running`). This is not an
implementation detail without consequences — later in the course (and in the final project) that single
tick of delay directly affects exactly when something is allowed to move on the line.

`Mode` is the first state machine that **gates** the rest of the system's behavior: physical movement
(`advance()`) only runs while the belt is actually `Running`. This introduces a key architectural habit
of the course: instead of scattering `if (mode == something)` checks throughout the code, one
well-tested decision point (a gate) protects one specific operation.

From this module on, several independent state machines cooperate at once (`Mode`, `DiverterPosition`,
`BeltMotorState`) — each has its own scope of responsibility, and none of them knows the details of the
others.

---

## 5. An independent E-Stop path

**Module 5 · `module-05-start` · missions 16–19**

> **Important disclaimer:** what you build in this module is a simplified, pedagogical model of the
> "independent safety path" pattern in code. **This is not the design of a real safety-rated system**
> and it does not replace a functional-safety course. You're learning an engineering pattern here, not
> a safety standard.

The main lesson: safety should not be "one more `if`" woven into business logic — it should be a
**separate, priority path** that can override everything else, regardless of what the rest of the
system happens to be doing at that moment.

```cpp
enum class EStopLatchState { Released, Engaged, Armed };
```

`EStopLatchState` is a *latch* — a state that **does not revert on its own.** Pressing the button
drives it to `Engaged`; releasing the button alone is not enough to return to `Released` — an explicit
`Reset` is still needed, and only from the intermediate `Armed` state.

That clears the latch itself, but not restarting the line. Release + Reset unlock the system and bring
`Mode` back to `Idle` — that's not `Running` yet. Actually moving the line again needs a separate,
later `StartRequested`. Sending `Reset` and `StartRequested` on the same tick does **not** restart the
system right away — that tick's `modeStep` still only brings `Mode` to `Idle`; reaching `Running`
needs its own start call once `Mode` is already `Idle`. This is a deliberate design choice: resuming
the line is always meant to be the result of a separate, explicit operator decision, never a side
effect of unlocking the latch.

`Mode::EStopped` has higher priority than everything else — that is exactly what "independent path"
means: the rest of the system doesn't need to know about the E-Stop for the E-Stop to be able to
override it.

---

## 6. Sensors and data quality

**Module 6 · `module-06-start` · missions 20–24**

Up to now, the system always "knew" exactly what was happening. In a real system, a sensor can fail,
give a stale reading, or not respond at all.

```cpp
enum class ReadingStatus { Ok, Missing, Stale };

struct PresenceReading { ReadingStatus status; bool occupied; };
struct WeightReading { ReadingStatus status; Grams grams; };
```

The key distinction in this module: **ground truth** (what's actually on the scale — something the
simulator always knows, because we're the ones building it) versus **observation** (what the sensor
actually reported, with its own `ReadingStatus`). Control logic should never reach for ground truth
directly — it operates only on observations, exactly the way a real controller with real sensors would.

When a reading is `Stale`, the sensor substitutes the last trusted value (the **last-known-good**
pattern) — but only if such a value already exists; otherwise `Stale` degrades to the same behavior as
`Missing`. `Missing`, in turn, never falls back to last-known-good — it's an explicit "nothing is
known" signal, distinct from "known, but out of date." The same rule applies identically to the
presence and weight readings. This is what "fault-tolerant classification" means: a decision that
degrades predictably instead of failing randomly.

Classification uses these readings, but it also has to remember something of its own across ticks:
presence confirmation (`PresenceCheck`) and the weight reading (`Weighing`) are two separate,
sequential zones — a parcel passes through them one after another, on different ticks. To trust the
weight later, the system has to remember that presence was already confirmed earlier. That's what
`ControllerState` is for — a small record (`presenceConfirmed`, `classification`) kept across ticks for
the parcel currently being processed, updated every tick as it moves through the zones.

---

## 7. Faults and system events

**Module 7 · `module-07-start` · missions 25–28**

This module teaches a distinction that comes back in every later module: **external input** versus
**emergent event**.

```cpp
enum class DiverterFaultKind { Blocked };
enum class SystemEventKind { DiverterNotReady, RoutingDeadlineMissed };
```

`DiverterFaultKind::Blocked` is external input — something you *inject* into the system from outside
(e.g. in a test or a scenario) to simulate a mechanical failure. `SystemEventKind` is something
completely different: **an event the system inferred itself** from what it observed — e.g. that the
diverter didn't reach its position within a reasonable time (`DiverterNotReady`), and if that persists
too long, `RoutingDeadlineMissed`.

This distinction has architectural weight: external input is something you *give* the system; a system
event is something the system *produces* as a result of its own logic. Confusing the two categories is
a common design mistake — this module teaches you to keep them separate in the types, not just in the
names.

`Mode::Fault` reacts to `RoutingDeadlineMissed` and blocks further movement until the operator resets
the system — a similar recovery pattern to Module 5, but triggered by an entirely different cause.

---

## 8. Multiple parcels and invariants

**Module 8 · `module-08-start` · missions 29–32**

The biggest single architectural jump in Course Core: the system has to handle **more than one parcel
at a time**.

> **Important distinction:** "multiple parcels at once" **does not mean concurrent programming.** The
> simulator's core stays fully sequential and deterministic — one thread, one `Engine::step()` at a
> time. What changes is how much data `Plant` holds at once, not the execution model.

`Plant` stops holding a single `std::optional<Item>` and gets **four named slots** instead:

```cpp
struct Plant {
    std::optional<Item> infeed;
    std::optional<Item> presenceCheck;
    std::optional<Item> weighing;
    std::optional<Item> diverting;
};
```

This is a deliberate decision: **fixed slots, not a container.** Each physical zone on the line holds
exactly one parcel at a time — that's a physical constraint, so the type (`std::optional`, not
`std::vector`) enforces it directly, instead of merely describing it in a comment.

`advance()` processes transitions in one fixed direction: **downstream → upstream** (the zone closest
to the output first, Infeed last) — this lets a parcel that just vacated a zone be immediately followed
into it within the same tick (a "chain shift"), while preserving the key invariant: **no parcel can
move more than once in the same tick.** That's not a coincidence — it's a direct consequence of every
transition being evaluated exactly once, in a fixed order.

`ItemId` now becomes the per-parcel correlation key everywhere the mere fact of "is something here"
used to be enough — the trace (`TickResult`) has to say unambiguously *which* parcel a given observation
or event is about, even when several are on the line at once.

---

## 9. Scenarios as data

**Module 9 · `module-09-start` · missions 33–35**

Up to now, every experiment meant calling `Engine` methods by hand, step by step. This module introduces
a **declarative** way to describe a whole experiment as a single piece of data:

```cpp
struct Scenario {
    std::vector<ScenarioInput> operatorInputs;
    std::vector<ScriptedItemArrival> arrivals;
    std::vector<ScriptedSensorFault> sensorFaults;
    std::vector<ScriptedDiverterFault> diverterFaults;
    Tick duration;
};

bool isValidScenario(const Scenario& scenario);
std::optional<std::vector<TickResult>> runScenario(const Scenario& scenario);
```

Key property: `Scenario` is a **pure layer added on top of `Engine`**, not a new control mechanism —
`runScenario` internally just calls the same public `Engine` methods you already know from Module 3, at
the order and moments recorded in the data.

Fault time windows (`from`, `until`) are **half-open**: `[from, until)` — the fault is active on tick
`from`, but no longer on tick `until`. This is the standard, predictable way intervals are written
throughout the course, not an ad-hoc convention of this one type.

An important distinction between two kinds of failure: **static failure** (`isValidScenario` returns
`false` — the scenario never runs at all, because its own description is internally inconsistent)
versus **dynamic failure** (the scenario is statically valid, but something can't be carried out while
running — `runScenario` returns `std::nullopt` after it has already started). This distinction —
*whether the failure is visible without running anything, or only shows up while running* — comes back
in the final project.

`runScenario` builds a **fresh `Engine`** on every call — this guarantees **reproducibility**: the same
valid `Scenario` run twice always produces an identical trace.

---

## 10. How to read and write a test

A short preparation for Project Kickoff — the first exercise where you write a test yourself, not just
run one.

Every test in this course has the same simple shape — an informal **arrange / act / assert**:

```cpp
// arrange -- set up state
psm::WeightReading reading{psm::ReadingStatus::Ok, 750};

// act -- call the thing under test
auto result = psm::decideClassification(reading);

// assert -- check the result
psmCheck(result == psm::WeightClass::Heavy, "750g with a valid reading classifies as Heavy");
```

A test is an **executable contract** — instead of describing in words how a function should behave, a
test *demonstrates* it, in a form you can run, and which either passes or doesn't. That's why the course
treats a green `ctest -L misja-N` as the final criterion for finishing a mission: the contract is
executable, not just described.

The most important question to ask about your own test: **does it actually detect something, or does
it just pass?** A test that passes regardless of whether the code is correct proves nothing. A simple
way to check: deliberately break the code under test and confirm your test **actually** turns red.
You'll walk through this exact exercise in `final_project/00_project_kickoff.md`.

This is not a chapter about testing frameworks — the course deliberately uses one minimal function
(`psmCheck`) throughout, so your attention stays on *what* and *why* you're testing, not the tool.

---

## 11. The final project — how to approach a new requirement

The Final Project is the first time no one has written the architecture for you. You're given a domain
requirement (parcels can arrive faster than the line can accept them) and a shared contract to satisfy
(`final_project/01_final_project_brief.md`) — you design the rest yourself.

This chapter **does not tell you how to solve** the project's task — it's a methodology for approaching
a new requirement in an existing, established system, useful far beyond this one case:

1. **Start from the domain contract, not the code.** Before you write a line of C++, write down in
   words what must be true before and after every operation you add.
2. **Write invariants down explicitly.** Which ones are entirely new? Which existing system invariants
   must remain true unchanged? Which ones need to be deliberately extended?
3. **Decide ownership of the new state** before you implement it. Who holds it? Does it only make sense
   in one mode of driving the system, or in every mode?
4. **Protect invariants through encapsulation**, not convention. If something must always hold, it
   should be impossible to break through the public API, not merely "not recommended."
5. **Keep existing contracts unless you have a deliberate reason to change them.** Extending the system
   doesn't mean rewriting it from scratch, or bypassing what already works with a parallel
   implementation.
6. **Write your own tests** — by behavior category, not by random examples. Check that each test
   actually detects something (chapter 10).
7. **Build a `Scenario`** that demonstrates your solution end to end — exactly the way `Scenario`s
   demonstrated finished behavior in Module 9.
8. **Justify your design decisions** — briefly, in writing. This becomes the starting point for the
   defense.

Full requirements, acceptance criteria, and the list of decisions left to you:
[`final_project/01_final_project_brief.md`](https://github.com/michalmaj/mechatronic-systems-programming/blob/final-project-start/final_project/01_final_project_brief.md)
(available from the `final-project-start` tag).

---

## Appendix A — glossary

| EN | PL | From the code |
|---|---|---|
| tick | tick | `Tick` |
| simulation step | krok symulacji | `Engine::step()` |
| tick result / trace | wynik ticku / ślad | `TickResult`, `std::vector<TickResult>` |
| zone | strefa | `Zone` |
| item / parcel | paczka | `Item`, `ItemId` |
| command | polecenie | `DiverterCommand`, `BeltMotorCommand` |
| actual state | stan rzeczywisty | `DiverterPosition`, `BeltMotorState` |
| operating mode | tryb pracy | `Mode` |
| latch (E-Stop) | zatrzask | `EStopLatchState` |
| sensor reading | odczyt czujnika | `PresenceReading`, `WeightReading` |
| reading status | status odczytu | `ReadingStatus` |
| (injected) fault | usterka (wstrzyknięta) | `SensorFaultKind`, `DiverterFaultKind` |
| system event | zdarzenie systemowe | `SystemEventKind` |
| scenario | scenariusz | `Scenario` |
| scenario replayer | odtwarzacz scenariusza | `runScenario` |
| static failure | błąd statyczny | `isValidScenario(...) == false` |
| dynamic failure | błąd dynamiczny | `runScenario(...) == std::nullopt` |
| mission | misja | `ctest -L misja-N` |
| module starting point | punkt startowy modułu | tag `module-XX-start` |
| reference solution | rozwiązanie referencyjne | tag `module-XX-solution` |

## Appendix B — commands

**CMake / CTest**

```bash
cmake --preset dev                 # configure (once, or after CMakeLists.txt changes)
cmake --build --preset dev         # build
ctest --preset test                # all tests
ctest --preset test -L misja-5     # only that mission's test
ctest --preset test -R test_name   # test by name (regex match)
ctest --preset test --output-on-failure   # show output of failing tests
```

**Git**

```bash
git fetch --tags                          # get the latest course tags
git switch -c my-work module-01-start     # your own branch from the module's starting point
git add <file>
git commit -m "short, specific description"
git log --oneline                         # your commit history
```

## Appendix C — type / API map

A short map of the most important Core types (not full API documentation — for details, go to the
headers in `include/psm/` and the module's own material):

| Type | Where | Role |
|---|---|---|
| `Plant` | `plant.hpp` | the line's physical state — parcel slots |
| `Item` / `ItemId` | `item.hpp` | a parcel and its identity |
| `Diverter` | `diverter.hpp` | the actuator that routes parcels to an output |
| `BeltMotor` | `belt_motor.hpp` | the belt drive, with ramping |
| `Mode` | `mode.hpp` | the whole system's operating mode |
| `EStopLatchState` | `estop_latch.hpp` | the safety latch |
| `PresenceSensor` / `WeightSensor` | `presence_sensor.hpp`, `weight_sensor.hpp` | sensors with reading uncertainty |
| `Engine` | `engine.hpp` | the one-tick orchestrator, the system's public API |
| `TickResult` | `tick_result.hpp` | the full, unambiguous result of one tick |
| `Scenario` / `runScenario` | `scenario.hpp` | declarative experiment description and replay |
