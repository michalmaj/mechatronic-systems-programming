[🇵🇱 Polski](01_instalacja_macos.md) | 🇬🇧 English

# 0.1 Installation: macOS

You need: Xcode Command Line Tools (compiler + Git), CMake, and CLion.

## Xcode Command Line Tools

Open Terminal (Applications → Utilities → Terminal) and type:

```bash
xcode-select --install
```

A system dialog will pop up asking to install — confirm it. This installs the Clang compiler and
Git, without needing to install the full Xcode from the App Store.

Check that it worked:

```bash
clang++ --version
git --version
```

## CMake

The easiest way is via Homebrew (a package manager for macOS). If you don't have Homebrew yet:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Then:

```bash
brew install cmake
cmake --version
```

## CLion

1. Download CLion from jetbrains.com/clion. CLion is now free for non-commercial use (learning,
   hobby projects, open source) — for this course, a regular JetBrains account is enough, no
   university registration required. An alternative is the free license from the JetBrains
   educational program (registration via your university email address) — useful if you'd rather
   have a license tied to your student status, but not needed to get started.
2. Install it (the JetBrains Toolbox App is the most convenient option for installing and later
   updating it).

CLion detects the installed compiler and CMake automatically on first launch.

**Next:** [downloading the repository](./02_pobranie_repozytorium.en.md).
