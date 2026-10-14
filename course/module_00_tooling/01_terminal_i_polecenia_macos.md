🇵🇱 Polski | [🇬🇧 English](01_terminal_i_polecenia_macos.en.md)

# 0.1 Terminal i podstawowe polecenia: macOS

Zanim zainstalujesz narzędzia kursu, oswój się z terminalem. Przez większość Modułu 0 pracujesz w
IDE, ale od kolejnych modułów terminal będzie potrzebny co tydzień.

## Otwórz Terminal

Aplikacje → Narzędzia → Terminal (albo Cmd+Spacja, wpisz "Terminal"). Prompt będzie podobny do:

```
twojanazwa@MacBook-Pro ~ %
```

## Gdzie jestem? Co tu jest?

```bash
pwd
ls
```

## Przejdź do folderu

```bash
cd Documents
cd ..
```

## Ćwiczenie w piaskownicy

```bash
cd ~
mkdir terminal-cwiczenia
cd terminal-cwiczenia
pwd
touch notatka.txt
ls
rm notatka.txt
ls
cd ..
```

## Czym jest prośba o hasło administratora?

Niektóre instalacje (np. `xcode-select --install` w następnym kroku) poproszą o Twoje hasło albo
pokażą systemowe okno z prośbą o zgodę. To normalne — system chce potwierdzenia, że to Ty instalujesz
oprogramowanie, nie jakiś inny program bez Twojej wiedzy.

## Jeśli coś poszło nie tak

- **"command not found: ...":** sprawdź literówki.
- **Terminal ma inny prompt niż w przykładzie:** to normalne, zależy od wersji macOS i ustawień.
  Liczy się polecenie, nie wygląd.
- **Terminal pyta o hasło, a nie wiesz, co to:** to hasło Twojego konta macOS (to, którym się
  logujesz), nie coś nowego do ustawienia.

**Dalej:** [instalacja](./02_instalacja_macos.md).
