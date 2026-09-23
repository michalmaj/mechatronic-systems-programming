[🇵🇱 Polski](roadmap.md) | 🇬🇧 English

# Roadmap

← [README](../README.en.md) · [Handbook](handbook.en.md)

This is the operational map: what to do, in what order, and how you'll know you're ready for the next
step. Full explanations of the concepts live in the [Handbook](handbook.en.md) — this is just the
route.

## The path

```
Setup / Module 0
      ↓
Module 1
      ↓
Module 2
      ↓
Module 3
      ↓
Checkpoint 1
      ↓
Module 4
      ↓
Module 5
      ↓
Module 6
      ↓
Module 7
      ↓
Checkpoint 2
      ↓
Module 8
      ↓
Module 9
      ↓
Checkpoint 3
      ↓
Write your first test
      ↓
Final project
      ↓
Defense
```

The whole required path, from setup to the final project defense, is budgeted at roughly 30 hours of
work with the repository. Treat that as a planning target, not a hard limit — actual time varies
between students, and it'll be refined once we have pilot data.

## Working through a module

The same pattern repeats for Modules 1–9:

1. `git fetch --tags` to get the latest tags.
2. Branch off the module's starting point: `git switch -c my-work module-XX-start`.
3. Read the module's chapter in the [Handbook](handbook.en.md) and the introduction in
   `course/module_XX_.../00_wprowadzenie.md`. If you follow the source-code links inside that
   material, check [`course/README.md`](../course/README.md) first (Polish only for now).
4. Work through the module's missions in order. Each one is its own file,
   `course/module_XX_.../NN_*.md`, with a concrete task.
5. Run each mission's test as you finish it: `ctest --preset test -L misja-N`.
6. Commit often, in small steps — one meaningful change per commit rather than one big commit at the
   end of the module.
7. Once the module is done, run the full `ctest --preset test` and confirm everything is green.

`module-XX-solution` is that module's reference solution. Use it whenever you're stuck, or to compare
against your own approach afterward — but reaching the solution on your own is where the real
learning happens.

## Modules

| Module | Problem | You'll learn (C++) | What you build | Start | Missions | Done when |
|---|---|---|---|---|---|---|
| **0 — Orientation and tooling** | Getting your environment ready | CMake/CTest as tools you use, not build | a working build and CLI | *(no tag — `main`)* | — | `cmake --build` and `ctest` run locally |
| **1 — Control basics** | One parcel, one zone at a time | `enum class`, `struct`, `std::optional`, functions, a first control loop | `Plant`, `Item`, classification, a control loop | `module-01-start` | 1–6 | tests `misja-1`…`misja-6` are green |
| **2 — The Diverter class** | A command isn't the same thing as physical state | your first class, encapsulation, command vs. actual state | `Diverter` | `module-02-start` | 7–9 | tests `misja-7`…`misja-9` are green |
| **3 — Engine** | Something has to orchestrate a full simulation step | composition and ownership, free functions as logic (`Controller`), `Tick`/`TickResult` | `Engine::step()` | `module-03-start` | 10–12 | tests `misja-10`…`misja-12` are green |
| **4 — Belt and operating mode** | The line needs an operating mode, and an actuator that ramps up rather than switching instantly | another class, several state machines working together, gating | `BeltMotor`, `Mode` | `module-04-start` | 13–15 | tests `misja-13`…`misja-15` are green |
| **5 — Emergency stop** | Safety needs its own decision path, not one more `if` | a dedicated latch state, priorities, two independent paths | `EStopLatchState`, `Mode::EStopped` | `module-05-start` | 16–19 | tests `misja-16`…`misja-19` are green |
| **6 — Sensors** | Sensor data isn't always trustworthy | reading-status types, keeping the last trusted value, real state vs. what a sensor reports | `PresenceSensor`, `WeightSensor`, fault-tolerant classification | `module-06-start` | 20–24 | tests `misja-20`…`misja-24` are green |
| **7 — Fault mode** | Something breaks mechanically — how does the system notice? | external input vs. an event the system infers itself, `Mode::Fault`, recovery | `SystemEventKind`, fault detection and handling | `module-07-start` | 25–28 | tests `misja-25`…`misja-28` are green |
| **8 — Multiple parcels** | More than one parcel on the line at once | fixed named slots instead of a single object, output-to-input processing order, `ItemId` as a correlation key | a multi-parcel `Plant` and `Engine` | `module-08-start` | 29–32 | tests `misja-29`…`misja-32` are green |
| **9 — Scripted scenarios** | Running repeatable experiments without driving the system by hand | a declarative data type, validation, `runScenario` as a replayer | `Scenario`, `isValidScenario`, `runScenario` | `module-09-start` | 33–35 | tests `misja-33`…`misja-35` are green |

Module 8: multiple parcels at once doesn't mean multithreading. The simulator's core stays fully
sequential and deterministic — there's just more than one parcel in flight at a time.

Module 5: this is a simplified, pedagogical model of an emergency stop, meant to show the pattern of
an independent safety path in code. It is not a real safety-rated system design, and it isn't a
substitute for a functional-safety course.

## Checkpoints

After Modules 3, 7, and 9 there's a short, individual checkpoint with the instructor. Green tests are
necessary but not sufficient — the checkpoint also checks whether you can explain your own decisions
and apply what you've learned to a small, previously unseen problem. The instructor sets the exact
format.

## Write your first test

A short exercise before the final project (the source tree calls it Project Kickoff) — the first time
you write a test yourself, from scratch, for a function you already know from the course. It isn't
another module. Material: `final_project/00_project_kickoff.md`, available from the
`final-project-start` tag.

## Final project

```bash
git fetch --tags
git switch -c my-final-project final-project-start
```

Full description: [`final_project/README.md`](https://github.com/michalmaj/mechatronic-systems-programming/blob/final-project-start/final_project/README.md)
— available only once you've switched to `final-project-start`, since that directory doesn't exist
on `main`. The project builds on the exact same simulator you've been working on all course; you're
not starting over. Its reference solution isn't published — this one you solve on your own, with room
for your own design decisions wherever the brief explicitly leaves them open.

## Defense

After you submit the project, there's a short, individual defense: you explain your own decisions and
work through a small, previously unseen change or question based on your own code. Details are in the
"Defense" section of `final_project/01_final_project_brief.md`.
