# 0.1 Instalacja: macOS

Potrzebujesz: Xcode Command Line Tools (kompilator + Git), CMake oraz CLion.

## Xcode Command Line Tools

Otwórz Terminal (Aplikacje → Narzędzia → Terminal) i wpisz:

```bash
xcode-select --install
```

Pojawi się okno systemowe z prośbą o instalację — potwierdź. To zainstaluje kompilator Clang oraz
Git, bez potrzeby instalowania pełnego Xcode ze Sklepu App Store.

Sprawdź, czy się udało:

```bash
clang++ --version
git --version
```

## CMake

Najprościej przez Homebrew (menedżer pakietów dla macOS). Jeśli nie masz jeszcze Homebrew:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Następnie:

```bash
brew install cmake
cmake --version
```

## CLion

1. Pobierz CLion ze strony jetbrains.com/clion. CLion jest dziś darmowy do użytku niekomercyjnego
   (nauka, projekty hobbystyczne, open source) — do kursu wystarczy zwykłe konto JetBrains, bez
   żadnej rejestracji uczelnianej. Alternatywą jest darmowa licencja z programu edukacyjnego
   JetBrains (rejestracja przez adres e-mail Twojej uczelni) — przydatna, jeśli wolisz mieć licencję
   związaną ze statusem studenta, ale niepotrzebna, żeby zacząć.
2. Zainstaluj (JetBrains Toolbox App jest najwygodniejszą opcją do instalacji i późniejszych
   aktualizacji).

CLion domyślnie wykrywa zainstalowany kompilator i CMake automatycznie przy pierwszym uruchomieniu.

**Dalej:** [pobranie repozytorium](./02_pobranie_repozytorium.md).
