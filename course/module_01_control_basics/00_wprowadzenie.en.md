[🇵🇱 Polski](00_wprowadzenie.md) | 🇬🇧 English

# 1.0 Introduction

In Module 0 you ran two programs: `toolchain_check` and a ready-made preview of `simulator_cli`. You
didn't write a single line of logic — the point was purely to get your environment working.

Now the real work begins. In this module you'll build a small but complete system from scratch: a
parcel that moves through successive conveyor zones and, at the end, gets routed to one of two
outputs depending on its weight. By the end of the module, that same `simulator_cli` you only watched
before will be your own program, printing the result of your work to the console.

## Where you're starting from

The code you get at the start **compiles right away** — but doesn't do anything meaningful yet. Every
function you'll be filling in already exists in the project as an empty skeleton with a `// TODO`
comment. Your job in each mission is to fill in one such spot, until the matching test stops
reporting a failure.

Fetch the module's starting point:

```bash
git fetch --tags
git switch -c <your-branch-name> module-01-start
```

`module-01-start` is a **tag** — a fixed point in Git history, not a branch. `git switch -c` creates a
new branch of your own, starting exactly at that point. That gives you somewhere safe to save your
own work (`git add`, `git commit`) — the tag itself never changes, and you can always come back to it.

## What one mission looks like

The module consists of six missions. Each one:

1. poses a concrete problem tied to our sorting cell,
2. introduces exactly the C++ elements needed to solve that problem — nothing more,
3. tells you what's already in place, and what you need to add,
4. gives you a quick way to check your work: running **just that mission's test**,
5. at the end (in the last mission) asks you to run the **whole** test suite at once.

Tests for all six missions already exist in the project — the course wrote them, not you. Your job is
to make them pass. Writing your own tests is a topic for later.

## Module map

1. **Parcel and zones** — what a parcel is and where it can be.
2. **Parcel movement** — how to move a parcel one zone forward.
3. **Conveyor state** — what happens when the conveyor is empty.
4. **Sorting decision** — how to decide which way to route a parcel.
5. **Control loop** — how to repeat this cycle many times in a row.
6. **First run** — your own `main()`, printing the result to the console.

## Before you start

- You don't need to change anything in the `CMakeLists.txt` files — the whole build configuration is
  already in place.
- You don't need to write or modify tests — only run them.
- If you get stuck, every mission has a section covering the most common mistakes at that stage.

**Next:** [Mission 1: parcel and zones](./01_paczka_i_strefy.en.md).
