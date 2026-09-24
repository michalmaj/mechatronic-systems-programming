[🇵🇱 Polski](01_instalacja_linux.md) | 🇬🇧 English

# 0.1 Installation: Linux

You need: a compiler and build tools, CMake, Git, and CLion.

## Compiler and build tools

On Debian/Ubuntu-based distributions:

```bash
sudo apt update
sudo apt install build-essential cmake git
```

`build-essential` installs the GCC compiler along with basic build tools (including `make`). If
you use a different distribution (Fedora, Arch, ...), look for the equivalent of
`build-essential` in your package manager (e.g. on Fedora: `sudo dnf groupinstall
"Development Tools"`) — the package name may differ, but you need the same thing: a C++ compiler
and `make`.

Check that it worked:

```bash
g++ --version
cmake --version
git --version
```

Each of these three commands should print a version number, not a "command not found" error.

## CLion

1. Download CLion from jetbrains.com/clion. CLion is now free for non-commercial use (learning,
   hobby projects, open source) — for this course, a regular JetBrains account is enough, no
   university registration required. An alternative is the free license from the JetBrains
   educational program (registration via your university email address) — useful if you'd rather
   have a license tied to your student status, but not needed to get started.
2. Install it following the instructions for your distribution (the JetBrains Toolbox App is the
   most convenient option — it also makes it easy to update CLion later).

CLion detects the installed compiler and CMake automatically on first launch.

**Next:** [downloading the repository](./02_pobranie_repozytorium.en.md).
