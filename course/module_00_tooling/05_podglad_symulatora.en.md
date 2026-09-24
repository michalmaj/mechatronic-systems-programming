[🇵🇱 Polski](05_podglad_symulatora.md) | 🇬🇧 English

# 0.5 A preview: where we're heading

The last step of this module doesn't teach anything technically new — it's the same building and
running you already know from `toolchain_check`. We just want to show you where we're heading.

## Run `simulator_cli`

Same as before: select the `simulator_cli` target from the list in your IDE (instead of
`toolchain_check`) and run it.

## What to expect

You'll see a dozen or so lines of text, something like:

```
tick 0: mode=Running, item 1 in zone Infeed
tick 1: mode=Running, item 1 in zone Infeed
...
```

This is a simulator of a small sorting cell — the system you'll be developing step by
step throughout the whole semester. At this stage **you don't need to understand a single line of
what's happening**. You'll see mentions of operating modes, zones, sensors in this output — we'll
come back to each of these concepts separately, in its own time, when it becomes relevant.

What matters right now is one observation: you just ran the very same project in which you were
fixing a missing semicolon a moment ago. One C++ project — from the smallest possible program
(`toolchain_check`) to something that already looks like a real system. That's what the whole
course is: the same project, growing step by step.

## End of Module 0

If you made it here and saw both programs' output working — you have your environment set up and
you know what the basic work cycle looks like. That's everything you need to get started.
