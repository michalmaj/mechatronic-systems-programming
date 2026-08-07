# 0.1 Instalacja: Linux

Potrzebujesz: kompilatora i narzędzi budowania, CMake, Gita oraz CLion.

## Kompilator i narzędzia budowania

Na dystrybucjach opartych o Debian/Ubuntu:

```bash
sudo apt update
sudo apt install build-essential cmake git
```

`build-essential` instaluje kompilator GCC razem z podstawowymi narzędziami budowania (m.in.
`make`). Jeśli używasz innej dystrybucji (Fedora, Arch, ...), poszukaj odpowiednika
`build-essential` w swoim menedżerze pakietów (np. na Fedorze: `sudo dnf groupinstall
"Development Tools"`) — nazwa pakietu bywa inna, ale potrzebujesz tego samego: kompilatora C++
oraz `make`.

Sprawdź, czy się udało:

```bash
g++ --version
cmake --version
git --version
```

Każda z tych trzech komend powinna wypisać numer wersji, nie błąd "command not found".

## CLion

1. Pobierz CLion ze strony jetbrains.com/clion — jako student masz prawo do darmowej licencji
   edukacyjnej (rejestracja przez adres e-mail Twojej uczelni).
2. Zainstaluj według instrukcji dla swojej dystrybucji (JetBrains Toolbox App jest najwygodniejszą
   opcją — pozwala też później łatwo aktualizować CLion).

CLion domyślnie wykrywa zainstalowany kompilator i CMake automatycznie przy pierwszym uruchomieniu.

**Dalej:** [pobranie repozytorium](./02_pobranie_repozytorium.md).
