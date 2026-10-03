🇵🇱 Polski | [🇬🇧 English](01_instalacja_macos.en.md)

# 0.1 Instalacja: macOS

Potrzebujesz Xcode Command Line Tools (kompilator i Git), CMake oraz CLion.

## Xcode Command Line Tools

Otwórz Terminal (Aplikacje → Narzędzia → Terminal) i wpisz:

```bash
xcode-select --install
```

Potwierdź instalację w oknie systemowym. W ten sposób zainstalujesz Clang i Gita bez pobierania
pełnego Xcode z App Store.

Sprawdź, czy się udało:

```bash
clang++ --version
git --version
```

## CMake

Najprościej zainstalować CMake przez menedżer pakietów Homebrew. Jeśli jeszcze go nie masz:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Następnie:

```bash
brew install cmake
cmake --version
```

## CLion

1. Pobierz CLion ze strony jetbrains.com/clion. Do nauki i projektów niekomercyjnych wystarczy zwykłe
   konto JetBrains. Możesz też skorzystać z licencji edukacyjnej przypisanej do uczelnianego adresu
   e-mail.
2. Zainstaluj program. JetBrains Toolbox ułatwia zarówno instalację, jak i późniejsze aktualizacje.

Przy pierwszym uruchomieniu CLion powinien sam wykryć kompilator i CMake.

**Dalej:** [pobranie repozytorium](./02_pobranie_repozytorium.md).
