[🇵🇱 Polski](README.md) | 🇬🇧 English

# Course material

← [README](../README.en.md) · [Roadmap](../docs/roadmap.en.md) · [Handbook](../docs/handbook.en.md)

## Before you read any module

**You read this directory (`course/`) on `main` — that's the canonical, current source of
instructions.** The code you write lives somewhere else: on your own branch, created from a module's
starting point (`module-XX-start`), exactly as described in the
[Roadmap](../docs/roadmap.en.md#where-to-read-where-to-get-code):

```bash
git fetch --tags
git switch -c my-work module-XX-start
```

Source-code links inside a mission (e.g. `[Plant](.../src/plant.cpp)`) point straight to the file on
GitHub, pinned to that mission's own tag — not to your local checkout. Clicking one always shows you
exactly the code the text is talking about, regardless of which branch you happen to be on. To edit
that same file yourself, find it under the same path (`src/...`, `include/psm/...`) in your own
checkout of `module-XX-start`.

**One thing worth remembering:** if you're reading mission material from your own branch (a local
`course/module_XX_.../NN_*.md` file, not the GitHub view), keep in mind that file is a snapshot from
whenever `module-XX-start` was cut — it may not include later editorial fixes. If something in the
text looks inconsistent with the code, or with what you remember from class, check the current version
of that file on `main` before reporting a bug.
