[🇵🇱 Polski](roadmap.md) | 🇬🇧 English

# Roadmap — student's plan of work

← [README](../README.en.md) · [Handbook](handbook.en.md)

This document is the operational map: what you do, in what order, and how you know you're ready for
the next step. Full explanations of the concepts live in the [Handbook](handbook.en.md) — this is just
the route.

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
Project Kickoff
      ↓
Final Project
      ↓
Defense
```

**Rough time budget for the whole required path (Setup → final project defense): about 30 hours of
work with the repository.** This is a planning target, not a hard ceiling — actual time varies between
students and will be calibrated from pilot data.

## Workflow for every module

The same pattern repeats for Modules 1–9:

1. `git fetch --tags` to get the latest tags.
2. Create your own branch from the module's starting point: `git switch -c my-work module-XX-start`.
3. Read the module's material in the [Handbook](handbook.en.md) (matching chapter) and the module
   introduction in `course/module_XX_.../00_wprowadzenie.md` — see
   [`course/README.md`](../course/README.md) (Polish only for now) regarding source-code links inside
   that material.
4. Work through the module's missions in order — each mission is a file
   `course/module_XX_.../NN_*.md` with a concrete task.
5. After each mission, run its test: `ctest --preset test -L misja-N`.
6. Commit small and often — one meaningful step per commit, not one giant commit at the end of the
   module.
7. At the end of the module, run the full `ctest --preset test` and make sure everything is green.

`module-XX-solution` is the module's reference solution — **open-book** material: there when you're
stuck, or to compare your own approach afterward. The real learning value, though, is in reaching the
solution yourself, not copying it right away.

## Modules

| Module | Problem | You learn (C++) | What gets built | Start | Missions | "Done" |
|---|---|---|---|---|---|---|
| **0 — Orientation and tooling** | Getting your environment ready | CMake/CTest as tools (you don't build them) | working build + CLI | *(no tag — `main`)* | — | `cmake --build` and `ctest` work locally |
| **1 — Control basics** | One parcel, one zone at a time | `enum class`, `struct`, `std::optional`, functions, a first control loop | `Plant`, `Item`, classification, first control loop | `module-01-start` | 1–6 | tests `misja-1`…`misja-6` green |
| **2 — The Diverter class** | A command isn't the same as physical state | your first own class, encapsulation, command vs. actual | `Diverter` | `module-02-start` | 7–9 | tests `misja-7`…`misja-9` green |
| **3 — Engine** | Who orchestrates a whole simulation step? | composition/ownership, free functions as logic (`Controller`), `Tick`/`TickResult` | `Engine::step()` | `module-03-start` | 10–12 | tests `misja-10`…`misja-12` green |
| **4 — Belt and operating mode** | The line needs an operating mode and an actuator that ramps up | another class, several cooperating state machines, gating | `BeltMotor`, `Mode` | `module-04-start` | 13–15 | tests `misja-13`…`misja-15` green |
| **5 — E-Stop** | Safety as an independent decision path, not another `if` | a dedicated latch state, priorities, two independent paths | `EStopLatchState`, `Mode::EStopped` | `module-05-start` | 16–19 | tests `misja-16`…`misja-19` green |
| **6 — Sensors** | Sensor data isn't always trustworthy | reading-status enums, last-known-good, ground truth vs. observation | `PresenceSensor`, `WeightSensor`, fault-tolerant classification | `module-06-start` | 20–24 | tests `misja-20`…`misja-24` green |
| **7 — Fault mode** | Something goes physically wrong — how does the system notice? | external input vs. emergent event, `Mode::Fault`, recovery | `SystemEventKind`, fault detection and handling | `module-07-start` | 25–28 | tests `misja-25`…`misja-28` green |
| **8 — Multiple parcels** | More than one parcel on the line at once | fixed named slots instead of one object, downstream→upstream processing, `ItemId` as the correlation key | multi-parcel `Plant`, `Engine` | `module-08-start` | 29–32 | tests `misja-29`…`misja-32` green |
| **9 — Scripted scenarios** | Repeatable experiments without driving the system step by step by hand | a declarative type, validation, `runScenario` as a replayer | `Scenario`, `isValidScenario`, `runScenario` | `module-09-start` | 33–35 | tests `misja-33`…`misja-35` green |

**A note on Module 8:** multiple parcels at once does not mean multithreading — the simulator's core
stays sequential and deterministic; there's just more than one parcel "in flight" at a time.

**A note on Module 5:** this is a simplified, pedagogical E-Stop model, built to show the pattern of an
independent safety path in code — **it is not a real safety-rated system design**, nor a full
functional-safety course.

## Checkpoints

After Modules 3, 7, and 9 there's a short, individual checkpoint with the instructor. Green tests are
necessary but not sufficient — the checkpoint also checks whether you can explain your own decisions
and apply what you've learned to a small, previously-unseen situation. The exact format is set by the
instructor.

## Project Kickoff

A short bridging exercise before the final project — the first time you write your own test from
scratch for a known course function. It is not another course module. Material:
`final_project/00_project_kickoff.md` (available from the `final-project-start` tag).

## Final Project

```bash
git fetch --tags
git switch -c my-final-project final-project-start
```

Full description: [`final_project/README.md`](https://github.com/michalmaj/mechatronic-systems-programming/blob/final-project-start/final_project/README.md)
(available only after `git switch` to `final-project-start` — this directory doesn't exist on `main`
yet). The project extends the
exact same simulator you built through Course Core — you are not starting from scratch. The project's
reference solution **is not published** — this is a task to solve yourself, with room for your own
design decisions exactly where the brief explicitly allows it.

## Defense

After submitting the project there's a short, individual defense — explaining your own decisions, plus
a small, previously-unseen change or analysis based on your own code. Details: the "Defense" section in
`final_project/01_final_project_brief.md`.
