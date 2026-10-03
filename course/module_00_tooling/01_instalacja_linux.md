🇵🇱 Polski | [🇬🇧 English](01_instalacja_linux.en.md)

# 0.1 Instalacja: Linux

Potrzebujesz kompilatora z narzędziami do budowania, CMake, Gita oraz CLion.

## Kompilator i narzędzia budowania

Na dystrybucjach opartych o Debian/Ubuntu:

```bash
sudo apt update
sudo apt install build-essential cmake git
```

`build-essential` instaluje kompilator GCC razem z podstawowymi narzędziami budowania (m.in.
`make`). W innej dystrybucji znajdź odpowiedni zestaw w jej menedżerze pakietów. Na Fedorze będzie
to na przykład `sudo dnf groupinstall "Development Tools"`. Nazwa pakietu może być inna, ale
potrzebne są kompilator C++ i `make`.

Sprawdź, czy się udało:

```bash
g++ --version
cmake --version
git --version
```

Każde z tych poleceń powinno wypisać numer wersji. Komunikat `command not found` oznacza, że danego
narzędzia nie udało się zainstalować albo nie ma go w zmiennej `PATH`.

## CLion

1. Pobierz CLion ze strony jetbrains.com/clion. Do nauki i projektów niekomercyjnych wystarczy zwykłe
   konto JetBrains. Możesz też skorzystać z licencji edukacyjnej przypisanej do uczelnianego adresu
   e-mail.
2. Zainstaluj program zgodnie z instrukcją dla swojej dystrybucji. JetBrains Toolbox ułatwia zarówno
   instalację, jak i późniejsze aktualizacje.

Przy pierwszym uruchomieniu CLion powinien sam wykryć kompilator i CMake.

**Dalej:** [pobranie repozytorium](./02_pobranie_repozytorium.md).
