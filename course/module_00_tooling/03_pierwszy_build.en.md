[🇵🇱 Polski](03_pierwszy_build.md) | 🇬🇧 English

# 0.3 The first build

You have the tools installed and the repository downloaded. Time to build the first program.

## Open the project

**Visual Studio 2022:** from the start screen, choose **"Open a local folder"** and point it at
the `mechatronic-systems-programming` folder. Visual Studio will recognize the `CMakeLists.txt`
file and configure the project on its own — this can take a moment on first open (a progress bar
at the bottom of the window).

**CLion:** from the start screen, choose **Open** and point it at the
`mechatronic-systems-programming` folder. CLion will recognize the `CMakeLists.txt` file and
automatically run CMake configuration — wait until the progress bar at the bottom disappears.

## Find and run `toolchain_check`

This is one of several programs (targets) defined in this project — the smallest one, created
specifically for this exercise.

**Visual Studio 2022:** on the top toolbar, next to the run button (green triangle), there's a
dropdown list of available targets. Select `toolchain_check.exe`, then click the green triangle
(or press F5).

**CLion:** in the top-right corner there's a dropdown list of run configurations. Select
`toolchain_check`, then click the green triangle next to it (or press Shift+F10).

## What to expect

At the bottom of the screen (in the "Output" or "Run" window) you should see:

```
toolchain check: compiler, CMake, and IDE are wired together correctly.
```

If you see this — congratulations. Your compiler, CMake, and IDE are correctly working together.
This is exactly the same mechanism you'll use to build every subsequent program in this course, all
the way to the sorting-cell simulator you'll see at the end of this module.

## If something went wrong

Most common causes:
- **The IDE doesn't recognize the project as CMake** — make sure you opened the
  `mechatronic-systems-programming` *folder* (the one containing `CMakeLists.txt`), not some
  folder inside it.
- **The target list is empty or doesn't include `toolchain_check`** — wait for CMake configuration
  to finish (the progress bar at the bottom of the window) and refresh the list.
- **An error during CMake configuration** — check whether the compiler actually got installed (go
  back to the installation step for your system and run the version-check commands).

**Next:** [a change and an error](./04_zmiana_i_blad.en.md).
