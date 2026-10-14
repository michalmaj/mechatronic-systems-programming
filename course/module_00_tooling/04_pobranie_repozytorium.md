🇵🇱 Polski | [🇬🇧 English](04_pobranie_repozytorium.en.md)

# 0.4 Pobranie repozytorium

Na razie Git posłuży do pobrania plików kursu i do jednej próby generalnej schematu, który będzie
wracał w każdym module. Do prawdziwych commitów i branchy wrócimy w Module 1, kiedy pojawi się
pierwsza własna zmiana do zapisania.

## Ścieżka podstawowa: git clone

Otwórz terminal (na Windows możesz użyć programu **Git Bash**, zainstalowanego razem z Git for
Windows, albo PowerShell, jeśli zainstalowałeś/aś Gita przez Chocolatey) i wykonaj, w miejscu na
dysku, gdzie chcesz trzymać materiały kursu:

```bash
git clone https://github.com/michalmaj/mechatronic-systems-programming.git
```

Powstanie folder `mechatronic-systems-programming` z pełną zawartością repozytorium.

## Ścieżka awaryjna: Download ZIP

Jeśli instalacja Gita się nie powiodła, możesz tymczasowo pobrać pliki jako archiwum. Na stronie
repozytorium w GitHubie kliknij zielony przycisk **Code**, wybierz **Download ZIP** i rozpakuj
archiwum w wybranym miejscu.

**To rozwiązanie tymczasowe.** Folder z rozpakowanego ZIP-a nie jest repozytorium Git. Nie da się
w nim aktualizować zmian ani ich zapisywać w historii. Wystarczy do przejścia przez ten moduł;
zanim zaczniesz zapisywać własne zmiany w kolejnym module, wrócimy do `git clone`.

## Sprawdzenie

Niezależnie od wybranej ścieżki, powinieneś/aś mieć teraz folder `mechatronic-systems-programming`
zawierający m.in. plik `CMakeLists.txt` w katalogu głównym. To sygnał, że masz kompletne
repozytorium, gotowe do otwarcia w IDE.

## Jak będziesz pracować w każdym module

Od Modułu 1 każdy moduł zaczynasz tym samym schematem. Poćwiczmy go teraz na sucho, na nazwie
brancha, której nie będziesz używać na serio (`cwiczenie-00`), żebyś nie pomylił/a tego z prawdziwym
startem Modułu 1 (tam nazwa brancha będzie `my-work`).

Wejdź do folderu repozytorium:

```bash
cd mechatronic-systems-programming
```

Ściągnij najnowsze tagi (oznaczone punkty startowe i rozwiązania modułów):

```bash
git fetch --tags
```

Załóż własną, ćwiczebną gałąź z punktu startowego Modułu 1:

```bash
git switch -c cwiczenie-00 module-01-start
```

Sprawdź, na jakiej gałęzi jesteś:

```bash
git status
```

Powinieneś/aś zobaczyć w pierwszej linii `On branch cwiczenie-00`.

To wszystko — na razie nic z tą gałęzią nie robimy, to tylko próba generalna. Wróć na `main`:

```bash
git switch main
```

Dokładnie ten schemat (`git fetch --tags` → `git switch -c my-work module-XX-start`) powtórzy się w
każdym module, tylko z Twoją prawdziwą nazwą gałęzi (`my-work`) i właściwym numerem modułu. Pełny
opis: [Plan pracy studenta](../../docs/roadmap.md#jak-pracować-w-każdym-module).

## Jeśli coś poszło nie tak

- **"fatal: destination path already exists":** folder `mechatronic-systems-programming` już
  istnieje z wcześniejszej próby; usuń go albo pracuj w innym miejscu na dysku.
- **"error: pathspec 'module-01-start' did not match any file(s) known to git":**
  `git fetch --tags` się nie wykonał albo nie masz połączenia z internetem; spróbuj ponownie.
- **Nie wiesz, na jakiej gałęzi jesteś:** `git status` zawsze powie Ci to w pierwszej linii.

**Dalej:** [pierwsze budowanie projektu](./05_pierwszy_build.md).
