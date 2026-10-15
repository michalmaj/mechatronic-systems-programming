[🇵🇱 Polski](01_terminal_i_polecenia_windows.md) | 🇬🇧 English

# 0.1 Terminal and basic commands: Windows

Before installing the course tools, get comfortable with a terminal. Most of Module 0 happens in
the IDE, but starting with later modules you will need the terminal every week — to fetch code and
run tests.

## Open PowerShell

1. Press the Windows key (or click the Start button).
2. Type `PowerShell`.
3. Click the **Windows PowerShell** (or **PowerShell**) application.

A window should open with text like:

```
PS C:\Users\YourName>
```

This is the **prompt** — it means PowerShell is waiting for a command. You do not type the prompt
itself.

## Where am I?

```powershell
Get-Location
```

Prints the folder PowerShell is currently in, e.g.:

```
Path
----
C:\Users\YourName
```

## What is here?

```powershell
Get-ChildItem
```

Lists files and folders in the current location.

## Move into a folder

```powershell
cd Documents
```

Check where you are with `Get-Location`. Go back up with `cd ..`.

## Practice in a sandbox folder

Run these one by one so the commands stop feeling scary:

```powershell
cd $HOME
mkdir terminal-practice
cd terminal-practice
Get-Location
New-Item note.txt -ItemType File
Get-ChildItem
Remove-Item note.txt
Get-ChildItem
cd ..
```

Each step does one thing: creates a practice folder, moves into it, creates a file, lists it,
removes the file, lists again (should be empty), moves back up.

## What are "administrator permissions"?

Windows sometimes asks whether to let a program change the system (a **UAC** — User Account
Control — window). That is a separate topic from the terminal itself; you do not need it for this
step. If such a window appears during the Visual Studio installation, click **Yes** — that is
normal. The full explanation (and PowerShell *as* administrator) comes in the optional Chocolatey
step.

## Common problems

- **I can't find PowerShell:** double-check the spelling; on newer Windows it may appear as
  **Terminal**, opening a PowerShell tab.
- **"Cannot find path":** you are in a different folder than you think — check `Get-Location`, then
  `Get-ChildItem` to see what is actually there.
- **Command not recognized:** check for typos; PowerShell is not case-sensitive for command names,
  but spaces and dashes matter.

**Next:** [installation](./02_instalacja_windows.en.md).
