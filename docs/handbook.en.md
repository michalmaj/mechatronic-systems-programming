[🇵🇱 Polski](handbook.md) | 🇬🇧 English

# Course Handbook

← [README](../README.en.md) · [Roadmap](roadmap.en.md)

This handbook is the coherent teaching material for the course core (Modules 0–9), plus an
introduction to the final project. Each chapter's source of truth is the actual code and tests at
the matching tag (`module-XX-start` / `module-XX-solution`). If anything here looks different from
the code you're working with, the code wins — please report it via Issues.

**Contents:** [0](#0-how-to-use-this-handbook) · [1](#1-modeling-a-simple-process) ·
[2](#2-a-command-is-not-physical-state) · [3](#3-orchestrating-the-system) ·
[4](#4-an-actuator-and-an-operating-mode) · [5](#5-an-independent-emergency-stop-path) ·
[6](#6-sensors-and-data-quality) · [7](#7-faults-and-system-events) ·
[8](#8-multiple-parcels-and-invariants) · [9](#9-scenarios-as-data) ·
[10](#10-how-to-read-and-write-a-test) ·
[11](#11-the-final-project--approaching-a-new-requirement) ·
[Appendix A](#appendix-a--glossary) · [Appendix B](#appendix-b--commands) ·
[Appendix C](#appendix-c--type--api-map)

---

## 0. How to use this handbook

The course teaches through three steps that repeat throughout: code, experiment, test. Write a piece
of implementation, run the simulator through the CLI or a test, look at the result, and only then
check it against the provided test — not the other way around. The test is there to confirm what
you've already seen happen, not to tell you what to build.

A handful of things hold true across the whole course, worth knowing before you start:

- The simulation runs in discrete, deterministic steps called ticks. Every call to `Engine::step()`
  is one tick: read the current state, decide what changes, return the result as a `TickResult`. Feed
  it the same sequence of inputs twice and you get the same sequence of results twice.
- Tests come with the course. You won't write your own until the exercise before the final project
  (Chapter 10) — until then your job is to understand and run them, not design them. A test is a
  mission's executable contract: going green confirms the required behavior and finishes the mission,
  but it doesn't prove you understand the material. That's what the checkpoints after Modules 3, 7,
  and 9 are for.
- CMake and CTest are tools here, not something you're learning. Configure a preset, build, run the
  tests — that's the whole CMake surface you need for this course (Appendix B has the commands).
- You work on tags. Every module starts with `git switch -c <your-branch> module-XX-start`. Commit on
  your own branch and leave the course tags alone; they're your reference point throughout.

---

## 1. Modeling a simple process

**Module 1 · `module-01-start` · missions 1–6**

You start with the smallest version of the problem imaginable: one parcel, one zone at a time. A
parcel arrives, gets classified, and "moves" through zones — no actuator yet, no reaction delay, no
more than one parcel around.

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

It's deliberately as plain as it can be — a single `std::optional<Item>`, no container involved. This
is also where you pick up the C++ vocabulary the rest of the course leans on: `enum class` for a
closed set of states, `struct` for plain data, `std::optional` for "there might be nothing here"
instead of a pointer or a sentinel value, and free functions (`spawnItem`, `advance`, `classify`) as
the basic unit of logic, before classes enter the picture at all.

The first control loop in the CLI, simplified as it is, already shows the rhythm the whole course
follows: decide a command from the current state, take a step, print what happened. That rhythm —
decide, step, observe — comes back in every module after this one, just with progressively richer
state behind it.

---

## 2. A command is not physical state

**Module 2 · `module-02-start` · missions 7–9**

The lesson here: sending a command to a device doesn't mean the device has already carried it out. A
diverter told to move to `Diverted` needs time to get there, and code that assumes otherwise is wrong
— even when it compiles and looks reasonable.

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

This is your first class in the course, and it makes encapsulation concrete rather than abstract:
`Diverter` keeps a hard line between what you commanded (`command`) and what's actually happening
(`actualPosition`) — nothing outside the class can overwrite the actual state directly. This
command-versus-actual pattern resurfaces with `BeltMotor` in Module 4, and it's one of the most useful
design habits the course teaches.

`Plant::advance()` now has to wait for the diverter to settle (`isSettled()`) before a parcel can
leave — the first place where a duration, a number of ticks, becomes part of the logic instead of
just something you count in a loop.

---

## 3. Orchestrating the system

**Module 3 · `module-03-start` · missions 10–12**

Until now, `main()` in the CLI did all the orchestration by hand. This module moves that job to one
place: `Engine`.

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

`Engine` owns `Plant` and `Diverter` as members — it doesn't inherit from either, and it isn't either
of them. That has-a/is-a distinction keeps mattering throughout the course, and mixing composition up
with inheritance is one of the more common wrong turns: `Engine` assembles the domain objects and
calls into the control logic (the free functions in `Controller`), but that logic itself stays a set
of free functions rather than becoming `Engine` methods.

`TickResult` introduces another pattern worth noticing: instead of a pile of separate getters,
`Engine::step()` returns one complete value describing everything that happened during that tick.
Tests and the CLI only ever look at that value — they never need to interrogate `Engine`'s internal
state mid-tick.

This module also nails down the order operations happen in within a single step — implicit back in
Module 1, and from here on an explicit, tested contract.

---

## 4. An actuator and an operating mode

**Module 4 · `module-04-start` · missions 13–15**

A production line can't just "run" — it needs an operating mode, and its main drive needs to ramp up
and down rather than switch on and off instantly.

```cpp
enum class BeltMotorCommand { Stop, Run };
enum class BeltMotorState { Stopped, RampingUp, Running, RampingDown };
enum class Mode { Idle, Running };
```

(`Mode` grows over the next few modules: `EStopped` shows up in Module 5, `Fault` in Module 7. Each
state arrives together with the mechanism that actually needs it, not before.)

`BeltMotor` repeats the command/actual pattern from Module 2, with one addition that matters:
transitioning between states takes more than a single tick (`Stopped → RampingUp → Running`). That
one tick of delay isn't cosmetic — later in the course, and in the final project, it decides exactly
when something is allowed to start moving on the line.

`Mode` is the first state machine that gates the rest of the system's behavior: physical movement
(`advance()`) only happens while the belt is actually `Running`. That establishes a habit the course
keeps coming back to — rather than scattering `if (mode == something)` checks through the codebase,
one well-tested decision point guards one specific operation.

From here on, several independent state machines run side by side (`Mode`, `DiverterPosition`,
`BeltMotorState`), each responsible for its own slice of the system and indifferent to how the others
work internally.

---

## 5. An independent emergency-stop path

**Module 5 · `module-05-start` · missions 16–19**

> What you're building in this module is a simplified, pedagogical model of the "independent safety
> path" pattern in code. It is not the design of a real safety-rated system, and it doesn't stand in
> for a functional-safety course — you're learning an engineering pattern here, not a safety
> standard.

The core idea: safety shouldn't be one more `if` tangled into ordinary business logic. It should be a
separate path with its own priority, able to override everything else no matter what the rest of the
system happens to be doing at that moment.

```cpp
enum class EStopLatchState { Released, Engaged, Armed };
```

`EStopLatchState` is a latch — a state that doesn't revert by itself. Pressing the button drives it to
`Engaged`; releasing the button alone isn't enough to get back to `Released`, an explicit `Reset` is
still required, and only once the latch has passed through the intermediate `Armed` state.

That clears the latch, but it doesn't restart the line on its own. Release plus reset unlock the
system and bring `Mode` back to `Idle` — not `Running` yet. Actually moving the line again takes a
separate, later `StartRequested`. Send `Reset` and `StartRequested` on the same tick and the system
still won't restart right away: that tick's `modeStep` only gets you to `Idle`; reaching `Running`
needs its own start call once `Mode` is already sitting at `Idle`. That's deliberate — resuming the
line should always be a distinct, explicit operator decision, never a side effect of clearing the
lock.

`Mode::EStopped` outranks everything else, which is exactly what "independent path" is supposed to
mean: the rest of the system doesn't need to know the emergency stop exists for the emergency stop to
override it.

---

## 6. Sensors and data quality

**Module 6 · `module-06-start` · missions 20–24**

Up to this point the system always knew exactly what was going on. In a real system, a sensor can
fail, return a stale reading, or simply not answer.

```cpp
enum class ReadingStatus { Ok, Missing, Stale };

struct PresenceReading { ReadingStatus status; bool occupied; };
struct WeightReading { ReadingStatus status; Grams grams; };
```

The distinction this module is built around: what's actually on the scale — which the simulator
always knows, since we're the ones building it — versus what the sensor actually reported, tagged
with its own `ReadingStatus`. Control logic should never reach past the sensor for the real value; it
works only with what the sensor reports, exactly as a real controller would have to.

When a reading comes back `Stale`, the sensor falls back to the last trusted value, but only if one
already exists — otherwise `Stale` behaves exactly like `Missing`. `Missing` never falls back to a
previous value at all; it's an explicit "nothing is known" signal, and that's a different claim from
"known, but out of date." Presence and weight readings follow the same rule. That's what
fault-tolerant classification comes down to: a decision that degrades in a predictable way instead of
failing at random.

Classification uses these readings, but it also needs to remember something of its own from tick to
tick: presence confirmation (`PresenceCheck`) and the weight reading (`Weighing`) happen in two
separate, sequential zones — a parcel passes through them one at a time, on different ticks. To trust
the weight later on, the system has to remember that presence was already confirmed earlier. That's
the job of `ControllerState`, a small record (`presenceConfirmed`, `classification`) kept across ticks
for whichever parcel is currently being processed, updated every tick as it moves along.

---

## 7. Faults and system events

**Module 7 · `module-07-start` · missions 25–28**

This module teaches a distinction that keeps coming back: external input versus an event the system
works out on its own.

```cpp
enum class DiverterFaultKind { Blocked };
enum class SystemEventKind { DiverterNotReady, RoutingDeadlineMissed };
```

`DiverterFaultKind::Blocked` is external input — something you inject into the system from outside,
say in a test or a scenario, to simulate a mechanical failure. `SystemEventKind` is something else
entirely: an event the system inferred for itself from what it observed. For instance, that the
diverter hasn't reached position within a reasonable time (`DiverterNotReady`), and if that drags on
too long, `RoutingDeadlineMissed`.

The distinction carries real architectural weight: external input is something you hand to the
system, a system event is something the system produces on its own as a result of its own logic.
Mixing the two up is a common design mistake, and this module trains you to keep them apart in the
types, not just in what you happen to call them.

`Mode::Fault` reacts to `RoutingDeadlineMissed` and blocks further movement until the operator resets
the system — the same recovery shape as Module 5, triggered by a completely different cause.

---

## 8. Multiple parcels and invariants

**Module 8 · `module-08-start` · missions 29–32**

The single biggest architectural jump in the course core: the system now has to handle more than one
parcel at a time.

> Multiple parcels at once does not mean concurrent programming. The simulator's core stays fully
> sequential and deterministic — one thread, one call to `Engine::step()` at a time. What changes is
> how much data `Plant` holds simultaneously, not the execution model underneath it.

`Plant` stops holding a single `std::optional<Item>` and gets four named slots instead:

```cpp
struct Plant {
    std::optional<Item> infeed;
    std::optional<Item> presenceCheck;
    std::optional<Item> weighing;
    std::optional<Item> diverting;
};
```

That's a deliberate choice: named slots, not a container. Each physical zone on the line can hold
exactly one parcel — a physical constraint the type enforces directly (`std::optional`, not
`std::vector`) rather than one you'd have to describe in a comment and hope people read.

`advance()` walks through its transitions in one fixed direction, output to input: the zone nearest
the exit first, Infeed last. That lets a parcel that just vacated a zone be followed into it within
the same tick — a "chain shift" — while still preserving the invariant that no parcel moves more than
once per tick. That's not incidental; it falls directly out of every transition being evaluated
exactly once, in a fixed order.

`ItemId` becomes the correlation key for each parcel from here on, wherever the mere fact that
"something is present" used to be enough. The trace (`TickResult`) has to say unambiguously which
parcel a given observation or event belongs to, even with several on the line at once.

---

## 9. Scenarios as data

**Module 9 · `module-09-start` · missions 33–35**

Every experiment so far has meant calling `Engine` methods by hand, one step at a time. This module
adds a declarative way to describe a whole experiment as a single piece of data:

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

The property worth holding onto: `Scenario` is a plain layer sitting on top of `Engine`, not a new way
of controlling the system. `runScenario` just calls the same public `Engine` methods you already know
from Module 3, at whatever ticks the data says to call them.

Fault time windows (`from`, `until`) are half-open — `[from, until)` — so a fault is active on tick
`from` but no longer active on tick `until`. That's the standard, predictable way intervals are
written throughout the course, not a one-off convention for this particular type.

It's worth separating two kinds of failure here: a static failure, where `isValidScenario` returns
`false` and the scenario never runs at all because its own description contradicts itself, and a
dynamic failure, where the scenario is statically fine but something can't actually be carried out
while running, so `runScenario` returns `std::nullopt` after it's already started. Whether a problem
is visible before you run anything, or only once you do, is a distinction that comes back in the
final project.

`runScenario` builds a fresh `Engine` for every call, which is what makes it reproducible: run the
same valid `Scenario` twice and you get an identical trace both times.

---

## 10. How to read and write a test

A short warm-up for the exercise before the final project — the first time you write a test yourself
instead of just running one.

Every test in this course follows the same simple shape, an informal arrange/act/assert:

```cpp
// arrange -- set up state
psm::WeightReading reading{psm::ReadingStatus::Ok, 750};

// act -- call the thing under test
auto result = psm::decideClassification(reading);

// assert -- check the result
psmCheck(result == psm::WeightClass::Heavy, "750g with a valid reading classifies as Heavy");
```

A test is an executable contract. Rather than describing in words how a function should behave, it
demonstrates that behavior in a form you can actually run, and which either passes or doesn't. That's
why the course treats a green `ctest -L misja-N` as the final word on whether a mission is finished —
the contract runs, it isn't just written down somewhere.

The question worth asking about any test you write: does it actually catch something, or does it just
pass? A test that stays green no matter what the code does proves nothing. The simplest way to check:
break the code on purpose and confirm your test actually turns red. You'll do exactly this in
`final_project/00_project_kickoff.md`.

This isn't a chapter about testing frameworks — the course sticks to one minimal function
(`psmCheck`) throughout on purpose, so the focus stays on what you're testing and why, not on the
tooling.

---

## 11. The final project — approaching a new requirement

The final project is the first time nobody has handed you an architecture to fill in. You get a
domain requirement — parcels can arrive faster than the line can accept them — and a shared contract
to satisfy (`final_project/01_final_project_brief.md`). Everything else is your call.

This chapter doesn't tell you how to solve the project. It's a way of approaching a new requirement in
an existing, working system, and it's useful well beyond this one case:

1. Start from the domain contract, not the code. Before writing a line of C++, write down in plain
   words what has to be true before and after every operation you're adding.
2. Write your invariants down explicitly. Which ones are brand new? Which of the system's existing
   invariants have to keep holding unchanged? Which ones need a deliberate extension?
3. Decide who owns the new state before you implement it. Who's responsible for holding it? Does it
   only make sense in one way of driving the system, or in every mode?
4. Protect invariants with encapsulation, not a gentleman's agreement. If something has to hold
   always, the public API should make it impossible to break, not merely discouraged.
5. Keep existing contracts unless you have a real reason to change them. Extending the system doesn't
   mean rewriting it from scratch, or working around what already works with a parallel
   implementation.
6. Write your own tests, organized by behavior, not by whatever examples come to mind first. Check
   that each one actually catches something (Chapter 10).
7. Build a `Scenario` that demonstrates your solution end to end, the same way `Scenario`s
   demonstrated finished behavior back in Module 9.
8. Write down why you made the decisions you made. Keep it short — this becomes your starting point
   for the defense.

Full requirements, acceptance criteria, and the list of decisions left up to you:
[`final_project/01_final_project_brief.md`](https://github.com/michalmaj/mechatronic-systems-programming/blob/final-project-start/final_project/01_final_project_brief.md),
available from the `final-project-start` tag.

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
| latch (emergency stop) | zatrzask | `EStopLatchState` |
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

A short map of the most important types in the course core — not full API documentation. For
details, go to the headers in `include/psm/` and the relevant module's own material.

| Type | Where | Role |
|---|---|---|
| `Plant` | `plant.hpp` | the line's physical state — parcel slots |
| `Item` / `ItemId` | `item.hpp` | a parcel and its identity |
| `Diverter` | `diverter.hpp` | the actuator that routes parcels to an output |
| `BeltMotor` | `belt_motor.hpp` | the belt drive, with ramping |
| `Mode` | `mode.hpp` | the whole system's operating mode |
| `EStopLatchState` | `estop_latch.hpp` | the safety latch |
| `PresenceSensor` / `WeightSensor` | `presence_sensor.hpp`, `weight_sensor.hpp` | sensors with unreliable readings |
| `Engine` | `engine.hpp` | the one-tick orchestrator, the system's public API |
| `TickResult` | `tick_result.hpp` | the full, unambiguous result of one tick |
| `Scenario` / `runScenario` | `scenario.hpp` | declarative experiment description and replay |
