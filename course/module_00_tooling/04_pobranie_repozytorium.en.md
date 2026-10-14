[🇵🇱 Polski](04_pobranie_repozytorium.md) | 🇬🇧 English

# 0.4 Downloading the repository

For now, Git will download the course files and let you rehearse, once, the pattern that will come
back in every module. We'll come back to real commits and branches in Module 1, once you have your
own change worth saving.

## Primary path: git clone

Open a terminal (on Windows: you can use "Git Bash", installed together with Git for Windows, or
PowerShell if you installed Git via Chocolatey) and run, in the location on disk where you want to
keep the course materials:

```bash
git clone https://github.com/michalmaj/mechatronic-systems-programming.git
```

This creates a `mechatronic-systems-programming` folder with the repository's full contents.

## Fallback path: Download ZIP

If installing Git didn't work out, or you'd rather not deal with it right now — don't let that
block you. Go to the repository page on GitHub, click the green **Code** button, then
**Download ZIP**. Extract the archive to the location you want on disk.

**This is a temporary solution.** A folder from an extracted ZIP is not a Git repository — you
can't pull updates into it or save changes in its history. It's enough to get through this module;
before you start saving your own changes in the next module, we'll switch back to `git clone`.

## Check

Whichever path you chose, you should now have a `mechatronic-systems-programming` folder
containing, among other things, a `CMakeLists.txt` file in the root directory. That's the signal
that you have the complete repository, ready to open in an IDE.

## How you'll work in every module

Starting with Module 1, every module begins with the same pattern. Let's rehearse it now, as a dry
run, on a branch name you won't actually use for real (`cwiczenie-00`), so you don't confuse it with
the real start of Module 1 (where the branch name will be `my-work`).

Move into the repository folder:

```bash
cd mechatronic-systems-programming
```

Fetch the latest tags (the marked starting points and solutions for each module):

```bash
git fetch --tags
```

Create your own, throwaway practice branch from Module 1's starting point:

```bash
git switch -c cwiczenie-00 module-01-start
```

Check which branch you're on:

```bash
git status
```

The first line should read `On branch cwiczenie-00`.

That's it — we're not doing anything with this branch yet, it was just a dry run. Switch back to
`main`:

```bash
git switch main
```

This exact pattern (`git fetch --tags` → `git switch -c my-work module-XX-start`) will repeat in
every module, just with your real branch name (`my-work`) and the correct module number. Full
details: [Student Roadmap](../../docs/roadmap.en.md#working-through-a-module).

## Common problems

- **"fatal: destination path already exists":** the `mechatronic-systems-programming` folder
  already exists from an earlier attempt; delete it or work in a different disk location.
- **"error: pathspec 'module-01-start' did not match any file(s) known to git":**
  `git fetch --tags` didn't run, or you have no internet connection; try again.
- **Not sure which branch you're on:** `git status` always tells you in its first line.

**Next:** [the first build](./05_pierwszy_build.en.md).
