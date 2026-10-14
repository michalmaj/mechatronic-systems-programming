[🇵🇱 Polski](03_instalacja_git_chocolatey_windows.md) | 🇬🇧 English

# 0.3 Installation (optional): Git via Chocolatey

This step is **optional**. If you already have Git from the previous step (the installer from
git-scm.com), you can skip this page and move on. If you'd like to practice installing tools from
the terminal — useful later as a developer — stay here.

## What is a package manager?

A package manager installs and updates software with a single command, instead of hunting for an
installer in a browser and clicking through setup windows. On Windows, a popular one is
**Chocolatey**.

## Normal PowerShell vs. PowerShell as Administrator

So far you've opened regular PowerShell. Installing Chocolatey requires **PowerShell as
Administrator** — elevated permissions to change the system.

How to open PowerShell as Administrator:

1. Open the Start menu.
2. Type `PowerShell`.
3. Right-click **Windows PowerShell**.
4. Choose **Run as administrator**.
5. If a system (UAC) dialog appears asking for confirmation, click **Yes**.

The window looks similar to regular PowerShell, but has higher permissions — often shown by an
"Administrator" label in the window title. **Only use Administrator mode when a guide explicitly
tells you to** — not as your everyday way of working.

## Installing Chocolatey

In a PowerShell window **as Administrator**, paste:

```powershell
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
```

What this command does:

1. Temporarily allows this one PowerShell session to run installation scripts.
2. Enables the required connection security protocol.
3. Downloads the official Chocolatey install script from `community.chocolatey.org`.
4. Runs that script.

**Safety note:** this command downloads and runs a script from the internet. Only run a command
like this from official documentation (`docs.chocolatey.org`), never from a site or message you
don't recognize.

Wait for the command to finish without errors.

## Close and reopen PowerShell

Close the Administrator window. Open a **regular** PowerShell window from the Start menu (it no
longer needs to be Administrator).

## Verify

```powershell
choco --version
```

Should print a version number, e.g. `2.x.x`. If PowerShell doesn't recognize `choco`, close the
window and open a new one — the session sometimes needs refreshing.

## Installing Git via Chocolatey

In a regular (non-Administrator) PowerShell window:

```powershell
choco install git -y
```

The `-y` flag means "automatically agree to all prompts" — Chocolatey usually asks for confirmation
during installation.

Close and reopen PowerShell, then check:

```powershell
git --version
```

Should print Git's version number.

## Common problems

- **"Scripts is disabled on this system":** you didn't use PowerShell as Administrator, or skipped
  the first part of the install command (`Set-ExecutionPolicy ...`). Open PowerShell as
  Administrator and try this page again from the start.
- **"Access is denied":** same cause — you need PowerShell as Administrator.
- **`choco` isn't recognized after installing:** close PowerShell completely and open a new window.
  If that doesn't help, restart your computer.
- **A school/university computer blocks the install:** some organizations block software
  installation. Use the Git installer from the previous step instead, or ask IT support for help.

**Next:** [downloading the repository](./04_pobranie_repozytorium.en.md).
