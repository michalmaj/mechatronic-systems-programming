🇵🇱 Polski | [🇬🇧 English](01_instalacja_linux.en.md)

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

1. Pobierz CLion ze strony jetbrains.com/clion. CLion jest dziś darmowy do użytku niekomercyjnego
   (nauka, projekty hobbystyczne, open source) — do kursu wystarczy zwykłe konto JetBrains, bez
   żadnej rejestracji uczelnianej. Alternatywą jest darmowa licencja z programu edukacyjnego
   JetBrains (rejestracja przez adres e-mail Twojej uczelni) — przydatna, jeśli wolisz mieć licencję
   związaną ze statusem studenta, ale niepotrzebna, żeby zacząć.
2. Zainstaluj według instrukcji dla swojej dystrybucji (JetBrains Toolbox App jest najwygodniejszą
   opcją — pozwala też później łatwo aktualizować CLion).

CLion domyślnie wykrywa zainstalowany kompilator i CMake automatycznie przy pierwszym uruchomieniu.

**Dalej:** [pobranie repozytorium](./02_pobranie_repozytorium.md).
