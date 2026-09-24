[🇵🇱 Polski](00_project_kickoff.md) | 🇬🇧 English

# Project Kickoff: writing your first test

## Goal

Throughout the course core (Modules 0–9), tests were already there for you — written by the course,
run through `ctest`, read as a contract to understand. Before the final project, you need one new
skill: **writing your own test, from scratch, for a function you already know.**

This isn't Module 10. It's a short, bounded bridging exercise — one function, one CMake pattern to
copy, two or three test cases. CMake stays the same provided mechanics it's been throughout the core —
it isn't a new learning goal here, just something you repeat using an established pattern.

## 1. Anatomy of an existing test

Open `tests/controller_test.cpp` — one of the shortest tests in the course core:

```cpp
#include "support/check.hpp"
#include <psm/controller.hpp>

int main() {
    psmCheck(psm::classify(100) == psm::WeightClass::Light, "100g classifies as Light");
    psmCheck(psm::classify(499) == psm::WeightClass::Light, "499g classifies as Light");
    psmCheck(psm::classify(500) == psm::WeightClass::Heavy, "500g (at the threshold) classifies as Heavy");
    psmCheck(psm::classify(999) == psm::WeightClass::Heavy, "999g classifies as Heavy");
    return 0;
}
```

Four things worth noticing explicitly here:

- A test is a plain `.cpp` file with `main()` — no framework, no macro magic. What `ctest` runs is
  just a regular executable.
- `psmCheck(condition, description)` (`tests/support/check.hpp`) checks the condition; if it's false,
  it prints `description` to `stderr` and **terminates the process via `std::exit(1)`** — the first
  failing check stops the test immediately, the rest never runs.
- `return 0;` at the end of `main()` is the success signal for `ctest` — a test that reaches the end
  without calling `std::exit(1)` passes.
- The cases are chosen deliberately, not at random: `100` and `499` (mid-range Light), `500` (exactly
  at the boundary — the easiest place to get an off-by-one error), `999` (mid-range Heavy). The
  threshold itself is tested directly, not just "typical" values.

## 2. Your task: a test for `decideClassification`

`controller_test.cpp` tests `classify(Grams mass) -> WeightClass` — a pure function with no edge case
beyond the threshold itself. Right next to it, in the same `include/psm/controller.hpp`, lives a
function with a richer contract, stable since Module 6:

```cpp
std::optional<WeightClass> decideClassification(WeightReading weight);
```

```cpp
// include/psm/sensor_snapshot.hpp
struct WeightReading {
    ReadingStatus status;   // Ok, Missing, Stale
    Grams grams;
};
```

Implemented in `src/controller.cpp` like this:

```cpp
std::optional<WeightClass> decideClassification(WeightReading weight) {
    if (weight.status != ReadingStatus::Ok) {
        return std::nullopt;
    }
    return classify(weight.grams);
}
```

Unlike `classify`, this function has **two** paths to check: a valid reading (delegates to
`classify`) and an invalid one (`Missing` or `Stale` — always `std::nullopt`, regardless of whatever
happens to sit in `grams`).

**Write `tests/support_your_first_test.cpp`** (or any other file name — this isn't a mission with a
fixed name) checking at least:

1. `status == Ok`, mass in the Light range → the result is `WeightClass::Light`.
2. `status == Ok`, mass in the Heavy range (including the threshold) → the result is
   `WeightClass::Heavy`.
3. `status == Missing` → the result is `std::nullopt`, **regardless of the value of `grams`** — it's
   worth using a mass that "looks like" Heavy, to make sure the status genuinely wins.
4. `status == Stale` → same as above.

Model the file's structure on `controller_test.cpp` — `#include "support/check.hpp"`, `#include
<psm/controller.hpp>`, a series of `psmCheck(...)`, `return 0;`.

## 3. Registering it in CMake — a ready-made pattern to copy

`tests/CMakeLists.txt` registers every test with three lines. For the new file
`tests/support_your_first_test.cpp`, add at the end of the file:

```cmake
add_executable(support_your_first_test support_your_first_test.cpp)
target_link_libraries(support_your_first_test PRIVATE psm_core)
add_test(NAME support_your_first_test COMMAND support_your_first_test)
```

That's the **entire** pattern — the same three lines, only the name changes, for every test in that
file from `zone_test` to `engine_routing_deadline_test`. You don't need to understand CMake any more
deeply than this to copy it and swap the name — it's mechanics the course provides, not a new learning
goal.

One thing worth noting here: `add_executable`/`add_test` register **one `.cpp` file as one CTest
executable** — but nothing stops that one file from containing several independent `psmCheck` calls,
the same way `controller_test.cpp` already does today. A "test" in CTest's sense is an executable; a
"test case" is a single `psmCheck` inside it. That distinction comes back in the requirement in
section 6 below.

## 4. Running it through CTest

```bash
cmake --preset dev
cmake --build --preset dev
ctest --test-dir build/dev -R support_your_first_test --output-on-failure
```

If the test compiles and passes — all four `psmCheck` calls hold, `main()` reaches `return 0;`.

## 5. Compare against the existing course test

It's worth placing your file side by side with `controller_test.cpp`. The difference shouldn't be in
style (both are a series of `psmCheck`) — it should be in **what's being tested**:
`controller_test.cpp` never touches `status` at all, because `classify` doesn't take one. Your new
test exists precisely because `decideClassification` has an extra path that `classify` doesn't. If it
looks identical to `controller_test.cpp` with the function name swapped — the `Missing`/`Stale` path
probably hasn't been checked at all yet.

## 6. "The test passes" vs. "the test actually catches something"

A green test proves nothing on its own — it only proves something once paired with evidence that it
**would also turn red** if the code were broken. Worth checking that now, deliberately:

1. Temporarily break `decideClassification` in `src/controller.cpp` — e.g. remove the `status` check
   and always call `classify(weight.grams)`, regardless of status.
2. Rebuild and rerun your test.
3. If your test **stays green** despite the broken code — it wasn't testing the `status` path, it just
   happened not to notice it was missing. Go back to step 2 and add a case that actually catches it.
4. Undo the breakage (`git checkout -- src/controller.cpp` or by hand), confirm the test passes again
   on the correct code.

That's the difference between "the test passes" and "the test actually catches something" — and
exactly the same question will come up for every test you write in the final project (see the project
brief's section on your own tests).

## What's next

This exercise's minimum requirement: in your new test file, **at least two distinct cases
(`psmCheck` calls), at least one of which genuinely catches breakage**, verified the way you just did
in step 6 — not two separate executables; one file with a few checks is entirely sufficient (see
section 3). Once that's done, move on to `01_final_project_brief.md`.
