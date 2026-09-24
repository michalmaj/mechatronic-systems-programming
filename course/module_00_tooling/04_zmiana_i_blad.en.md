[🇵🇱 Polski](04_zmiana_i_blad.md) | 🇬🇧 English

# 0.4 A change and an error

Your program works. Now let's learn two things at once: what an ordinary code change looks like,
and what a compiler error looks like — because sooner or later you'll see one, and it's better to
get used to it now, on a program where you can't break anything permanently.

## Step 1: a harmless change

Open the file `apps/toolchain_check/main.cpp` in your IDE. You'll find one line responsible for the
printed text. Change the text of the message to anything else (e.g. add your name). Save the file
and run the program again (the same way as in the previous step).

Notice: you didn't have to "rebuild" anything by hand with a separate command — clicking Run in the
IDE notices on its own that the source file changed, and calls the compiler again before running.
This is exactly the same sequence we talked about in step 0.0: source file change → compiler → new
executable file → new process.

You should see your changed message. If so — it works.

## Step 2: a deliberate error

Now let's break something on purpose. In the same file, remove one semicolon (`;`) at the end of
any line of code. Save the file and try running the program again.

This time the program won't run. Instead, you'll see a compiler error message — usually in red
text, in the "Output"/"Build" window (Visual Studio) or the compiler messages window (CLion).

## How to read a message like this

A typical error message contains three things, in the order we look for them:

1. **The file name and line number** — points to where the compiler ran into the problem.
   Sometimes it isn't exactly the line you broke — a missing semicolon often only surfaces on the
   *next* line, because that's where the compiler "loses the thread".
2. **The first error message** — if you see a dozen or so error lines at once, focus on the first
   one. A missing semicolon can trigger a cascade of further, seemingly unrelated errors — fix the
   first one, and the rest often disappear on their own.
3. **The message text** — C++ compilers can be verbose, but you can usually pick out the gist (e.g.
   `expected ';'`).

## Step 3: the fix

Go back to the broken line, restore the semicolon, save the file, run again. You should see your
message from Step 1 again.

This is the whole cycle you'll repeat hundreds of times in this course: change → (sometimes) error
→ reading the message → fix → success. There's nothing embarrassing about it — compile errors are
a normal, everyday part of writing code, not a sign that something's wrong with you.

**Next:** [a preview of the simulator](./05_podglad_symulatora.en.md).
