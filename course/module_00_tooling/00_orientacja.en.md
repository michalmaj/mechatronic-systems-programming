[🇵🇱 Polski](00_orientacja.md) | 🇬🇧 English

# 0.0 Orientation: before we install anything

Before we install anything, it's worth breaking down a few words we'll be using all semester. If
you already know them — great, skim through and move on. If not, this is exactly the moment to
slow down.

## Operating system

The operating system (Windows, Linux, macOS) is the program that manages the computer: files,
memory, devices, other programs. Everything else discussed below runs *on top of* it.

## Terminal (console, command line)

A terminal is a program where you issue commands as text instead of clicking with a mouse. It's
neither scary nor "for advanced users" — it's simply another way of talking to the computer. We
will barely use it in this module — we work mostly through the IDE. But the terminal will show up
more and more often, so let's get comfortable with its existence right away.

## Compiler

The compiler is a program that translates source code (the text you write — `.cpp` files) into
code the processor understands. Without a compiler, your `.cpp` file is just text — nothing "runs"
directly from the source file. The compiler is a separate program, installed on your computer,
independent of the editor you write code in.

## CMake

The code in this course's project consists of many `.cpp` files. CMake is a tool that describes
how those files combine into finished programs — which files belong to which program, which
compiler options to use. CMake doesn't compile code itself — it calls the compiler for you, with
the right commands.

## IDE (integrated development environment)

An IDE (Visual Studio, CLion) is a program where you write code and which, *for convenience*, can
call CMake and the compiler on its own, show you errors in a readable form, and run the finished
program with one click. An IDE is not a compiler — it's a layer that makes it easier to use the
compiler and CMake, so you don't have to type commands by hand in the terminal (for now).

## Process and executable file

When the compiler finishes its work, it produces an **executable file** (on Windows: `.exe`; on
Linux/macOS: a file with no extension, marked as executable) — a finished program sitting on disk.
When you run that file (through the IDE, a double-click, or from the terminal), the operating
system creates a **process** — a running, live instance of that program, with its own memory,
executing "now". You can run the same executable file many times — each run creates a separate
process.

## Why all of this matters

When something "doesn't work", the first question is: at which stage? Is it a compiler error (the
code doesn't translate)? Is it a CMake error (the files don't link together)? Did the program build
fine but do something wrong once it's running as a process? In the next steps we'll learn to tell
these apart on a live example.

**Next:** installing the tools for your system —
[Windows](./01_instalacja_windows.en.md) · [Linux](./01_instalacja_linux.en.md) ·
[macOS](./01_instalacja_macos.en.md).
