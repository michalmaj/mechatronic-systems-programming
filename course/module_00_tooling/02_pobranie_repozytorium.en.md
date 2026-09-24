[🇵🇱 Polski](02_pobranie_repozytorium.md) | 🇬🇧 English

# 0.2 Downloading the repository

In this step, Git serves purely as a way to download the course files to disk. We're not dealing
with commits, branches, or anything else yet — we'll come back to that in a later module, once you
have your own change worth saving.

## Primary path: git clone

Open a terminal (on Windows: you can use "Git Bash", installed together with Git for Windows) and
run, in the location on disk where you want to keep the course materials:

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

**Next:** [the first build](./03_pierwszy_build.en.md).
