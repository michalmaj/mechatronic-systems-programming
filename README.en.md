[🇵🇱 Polski](README.md) | 🇬🇧 English

# Mechatronic Systems Programming

[![CI](https://github.com/michalmaj/mechatronic-systems-programming/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/michalmaj/mechatronic-systems-programming/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/github/license/michalmaj/mechatronic-systems-programming)](LICENSE)

A hands-on C++ course for mechatronics. Across 35 missions you build, test, and extend one
deterministic industrial sorting-cell simulator — from your first `enum class` to your own final
project.

## Who it's for

Mechatronics students, and anyone in a related engineering field, who want to learn modern C++ on a
real, growing system instead of disconnected exercises. You don't need prior C++ experience — some
programming background in any language is enough to get started.

## What you'll build

A sorting-cell simulator: parcels arrive on a conveyor, get weighed, classified, and routed to the
correct output. Along the way you add your own actuator class, an orchestrating `Engine`, operating
modes, an emergency stop, sensors with unreliable readings, fault handling, multiple parcels in
flight at once, and declarative test scenarios. It all runs on a regular computer, no hardware
required.

## How the course works

- The course core is Modules 0–9, made up of 35 missions.
- Every module has its own starting point as a Git tag, `module-XX-start`, and a matching reference
  solution, `module-XX-solution`. Use the solution freely when you're stuck — but you get the most
  out of the course by reaching it yourself first.
- The simulation is deterministic and sequential: the same scenario always produces the same result,
  with no threads and no randomness involved.
- CMake and CTest are provided infrastructure — you configure and run them, not build them from
  scratch. Each mission has its own test (`ctest -L misja-N`). A green test confirms the required
  behavior and finishes the mission, but it isn't proof that you understand it — that's what the
  checkpoints check, see the Roadmap.
- The course ends with an independent project, built on top of what you already have.

## Quick start

```bash
git clone https://github.com/michalmaj/mechatronic-systems-programming.git
cd mechatronic-systems-programming
git fetch --tags

git switch -c my-work module-01-start

cmake --preset dev
cmake --build --preset dev
ctest --preset test
```

Works on Windows, Linux, and macOS. For each following module you branch off the matching
`module-XX-start` — the Roadmap has the exact steps.

## Where to go next

- **[Roadmap](docs/roadmap.en.md)** — the exact path from Module 0 to the final project defense.
- **[Handbook](docs/handbook.en.md)** — coherent teaching material for every module.
- **[Module material](course/README.md)** *(Polish only for now)* — before clicking a source-code
  link inside a mission, read the short note on when those links actually resolve correctly.
- **[Final project](https://github.com/michalmaj/mechatronic-systems-programming/blob/final-project-start/final_project/README.md)** *(available from the `final-project-start` tag — this link won't resolve on `main`)* — an
  input buffer for the sorting line.
- **[Issues](../../issues)** — bugs in code or tests, problems in the material, proposals.

## Status

The course is in a pilot phase. Modules 0–9 are closed and stable; if anything in the code, tests, or
material looks inconsistent, report it via Issues.

## License

This project is released under the MIT License. See [LICENSE](LICENSE) for details.
