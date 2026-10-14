[🇵🇱 Polski](01_terminal_i_polecenia_linux.md) | 🇬🇧 English

# 0.1 Terminal and basic commands: Linux

Before installing the course tools, get comfortable with a terminal. Most of Module 0 happens in
the IDE, but starting with later modules you will need the terminal every week.

## Open a terminal

Depends on your desktop environment: look for "Terminal" in the application menu, or use a keyboard
shortcut (often Ctrl+Alt+T). A window should open with a prompt similar to:

```
yourname@computer:~$
```

## Where am I? What is here?

```bash
pwd
ls
```

`pwd` (print working directory) shows the current folder. `ls` lists its contents.

## Move into a folder

```bash
cd Documents
```

`cd ..` moves up. `cd ~` returns to your home folder.

## Practice in a sandbox folder

```bash
cd ~
mkdir terminal-practice
cd terminal-practice
pwd
touch note.txt
ls
rm note.txt
ls
cd ..
```

## What is `sudo`?

In the installation step you'll see `sudo apt install ...`. `sudo` is short for "run this as
administrator." The system will ask for your password — that's normal, it's your own account's
password, not some new root password. While typing a password in the terminal, **nothing is shown
on screen**, not even asterisks — that's intentional, not a bug. Type it and press Enter.

## Common problems

- **"command not found":** check for typos.
- **Password after `sudo` "doesn't work":** check Caps Lock; remember the screen shows nothing while
  you type, not even asterisks.
- **"Permission denied" without `sudo`:** some operations (like installing system packages) need
  `sudo`; moving around your own files does not.

**Next:** [installation](./02_instalacja_linux.en.md).
