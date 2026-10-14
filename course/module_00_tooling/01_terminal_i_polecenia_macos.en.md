[🇵🇱 Polski](01_terminal_i_polecenia_macos.md) | 🇬🇧 English

# 0.1 Terminal and basic commands: macOS

Before installing the course tools, get comfortable with a terminal. Most of Module 0 happens in
the IDE, but starting with later modules you will need the terminal every week.

## Open Terminal

Applications → Utilities → Terminal (or Cmd+Space, type "Terminal"). The prompt will look similar
to:

```
yourname@MacBook-Pro ~ %
```

## Where am I? What is here?

```bash
pwd
ls
```

## Move into a folder

```bash
cd Documents
cd ..
```

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

## What is that admin-password prompt?

Some installations (like `xcode-select --install` in the next step) will ask for your password or
show a system dialog asking for confirmation. That's normal — the system wants to confirm that it's
you installing software, not some other program without your knowledge.

## Common problems

- **"command not found: ...":** check for typos.
- **Terminal's prompt looks different from the example:** that's normal, it depends on your macOS
  version and settings. The command matters, not the look.
- **Terminal asks for a password and you don't know what it is:** it's your macOS account password
  (the one you log in with), not something new to set up.

**Next:** [installation](./02_instalacja_macos.en.md).
