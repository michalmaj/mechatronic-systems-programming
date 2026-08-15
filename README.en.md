[🇵🇱 Polski](README.md) | 🇬🇧 English

# Mechatronic Systems Programming

[![CI](https://github.com/michalmaj/mechatronic-systems-programming/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/michalmaj/mechatronic-systems-programming/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/github/license/michalmaj/mechatronic-systems-programming)](LICENSE)

A hands-on C++ course for mechatronics students: across 35 missions you build, test, and extend
**one deterministic industrial sorting-cell simulator** — from your first `enum class` to your own
final project.

## Who it's for

Mechatronics and related-engineering students who want to learn modern C++ on a real, growing system —
not on disconnected exercises. First contact with C++ is assumed; basic programming experience in any
language is enough to start.

## What you'll build

A sorting-cell simulator: parcels arrive on a conveyor, get weighed, classified, and routed to the
correct output. Along the way you add: your own actuator class, an orchestrating `Engine`, operating
modes, an E-Stop, sensors with reading uncertainty, fault handling, multiple parcels at once, and
declarative test scenarios. Everything runs on a regular computer — **no hardware required**.

## How the course works

- The course is **Course Core (Modules 0–9)**, made up of **35 missions**.
- Every module has its own starting point as a Git tag: `module-XX-start`. The reference solution is
  `module-XX-solution` — open-book material, there when you're stuck, but reaching the solution
  yourself is where the real value is.
- The simulation is **deterministic and sequential** — the same scenario always produces the same
  result. No threads, no randomness.
- `cmake`/`ctest` are provided infrastructure — you configure and run them, you don't build them from
  scratch. Each mission has its own test (`ctest -L misja-N`), which is its final contract: a mission
  is done when its test is green.
- The course ends with an independent **final project**, built on top of what you've already made.

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

Works on Windows, Linux, and macOS. For each following module you create a new branch from the
matching `module-XX-start` (exact workflow: see the Roadmap).

## Where to go next

- **[Roadmap / Student's plan of work](docs/roadmap.en.md)** — the exact path from Module 0 to the
  final project defense.
- **[Handbook](docs/handbook.en.md)** — coherent teaching material for every module.
- **[Final Project](https://github.com/michalmaj/mechatronic-systems-programming/blob/final-project-start/final_project/README.md)** *(available from the `final-project-start` tag — this link won't resolve on `main`)* — the
  final project: an input buffer for the sorting line.
- **[Issues](../../issues)** — bugs in code/tests, problems in the material, proposals.

## Status

The course is in a pilot phase. Course Core 0–9 material is closed and stable; if anything in the
code, tests, or material looks inconsistent, report it via Issues.

## License

This project is released under the MIT License. See [LICENSE](LICENSE) for details.
