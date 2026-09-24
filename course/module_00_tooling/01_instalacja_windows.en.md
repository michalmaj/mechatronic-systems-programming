[🇵🇱 Polski](01_instalacja_windows.md) | 🇬🇧 English

# 0.1 Installation: Windows

You need two things: Visual Studio 2022 (with the right set of components) and Git.

## Visual Studio 2022

1. The course deliberately sticks to **Visual Studio 2022** — not whatever the latest version
   happens to be. The general visualstudio.microsoft.com/downloads page defaults to proposing the
   newest edition (currently no longer 2022), so download the installer from the official VS2022
   release history page instead:
   [learn.microsoft.com/visualstudio/releases/2022/release-history](https://learn.microsoft.com/en-us/visualstudio/releases/2022/release-history)
   — the free **Community** edition is enough.
2. Run the installer. You'll see a list of **workloads** — large sets of components. Check
   **"Desktop development with C++"**.
3. On the right side, in the "Installation details" panel, make sure the **"C++ CMake tools for
   Windows"** component is checked — it's usually checked by default along with that workload, but
   it's worth verifying. This is what gives you CMake and Ninja, without a separate installation.
4. Click Install. This can take a while — several gigabytes of data.

You don't need any other workloads (e.g. for mobile or web development) — checking them will only
make the installation longer and use more disk space.

## Git for Windows

1. Download the installer from git-scm.com.
2. Run the installer — the default settings are fine, you can click "Next" through the whole
   process.

In this module, Git will be used for exactly one thing: downloading the course repository to disk.
We'll come back to Git itself more broadly in a later module.

## Check

Open Visual Studio 2022. From the start screen you should see the **"Open a local folder"**
option — we'll come back to that in the next step.

**Next:** [downloading the repository](./02_pobranie_repozytorium.en.md).
