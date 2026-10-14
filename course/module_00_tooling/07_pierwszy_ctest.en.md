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

In the repository folder (the same commands on Windows, Linux, and macOS). If you've only built
through the IDE so far (clicking Run), CTest first needs the same configured and built `build/dev`
folder, so run these in order:

```bash
cmake --preset dev
cmake --build --preset dev
ctest --preset test
```

The first two commands do what the IDE was doing for you with one click of Run. If you already
built in this repository folder (e.g. in the previous step, in the IDE), you can skip straight to
`ctest --preset test`.

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

- **"No tests were found!!!" (the command doesn't report an error, it just tests nothing):** the
  `build/dev` folder hasn't been configured and built from the terminal yet. Run `cmake --preset
  dev`, then `cmake --build --preset dev`, then go back to `ctest --preset test`.
- **"ctest: command not found" (on Windows, in a regular PowerShell):** CTest is installed together
  with CMake, but Visual Studio's CMake component ("C++ CMake tools for Windows") isn't added to
  regular PowerShell. Open **Developer PowerShell for VS 2022** from the Start menu — a special
  PowerShell window Visual Studio adds to the system, where `cmake`/`ctest` are already available —
  and try again there.
- **"ctest: command not found" (on Linux/macOS):** check `cmake --version` and go back to the
  installation step if it's missing.
- **"CMake Error: Could not read presets from ...: File not found: .../CMakePresets.json":** you're
  in a different folder than the repository root (the one that actually contains
  `CMakePresets.json`); `cd` there and try again.
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
