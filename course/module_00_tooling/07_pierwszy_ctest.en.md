[🇵🇱 Polski](07_pierwszy_ctest.md) | 🇬🇧 English

# 0.7 Running tests for the first time

The project already builds, and you know how to read a compiler error. Now you'll run tests — a
second, equally important way of checking that code behaves correctly. Starting in Module 1, every
mission ends with exactly this.

## What are tests in this project

The `tests/` directory contains small programs that check whether individual parts of the simulator
(e.g. `Diverter`, `BeltMotor`, `Engine`) behave the way they should. `CTest`, bundled with CMake,
runs all of them and summarizes the result.

## Run the tests from the terminal

In the repository folder, run (the same command on Windows, Linux, and macOS):

```bash
ctest --preset test
```

You'll see a list of tests passing one by one, ending in a summary like:

```
100% tests passed, 0 tests failed out of 22

Total Test time (real) =   5.82 sec
```

The exact test count may differ depending on the repository version — what matters is
`100% tests passed` and `0 tests failed`.

## What this checks

These tests protect the simulator core that already exists — the one you'll be developing all
semester. If you ever break something, some tests will turn red (`Failed`) and point to exactly
what stopped working, instead of forcing you to hunt for the bug by eye.

## Common problems

- **"ctest: command not found":** CTest is installed together with CMake; check `cmake --version`
  and go back to the installation step if it's missing.
- **"Preset 'test' is not defined":** you're in a different folder than the repository root (the
  one containing `CMakePresets.json`); `cd` there and try again.
- **Some tests show "Failed":** before looking for a bug in your own code, check whether the earlier
  build step (`cmake --build --preset dev`) finished without errors; tests run against the most
  recently built code.

## Starting in Module 1

You'll run a single mission's tests, e.g.:

```bash
ctest --preset test -L misja-1
```

The `-L misja-1` flag limits the run to tests tagged with that label — instead of waiting for the
whole suite, you check just what you're currently working on.

**Next:** [a preview of the simulator](./08_podglad_symulatora.en.md).
