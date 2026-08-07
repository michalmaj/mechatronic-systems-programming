# 0.2 Pobranie repozytorium

W tym kroku Git służy wyłącznie jako sposób pobrania plików kursu na dysk. Nie zajmujemy się
jeszcze commitami, branchami ani niczym innym — do tego wrócimy w późniejszym module, kiedy
będziesz mieć własną, wartą zapisania zmianę.

## Ścieżka podstawowa: git clone

Otwórz terminal (na Windows: możesz użyć "Git Bash", zainstalowanego razem z Git for Windows) i
wykonaj, w miejscu na dysku, gdzie chcesz trzymać materiały kursu:

```bash
git clone https://github.com/michalmaj/mechatronic-systems-programming.git
```

Powstanie folder `mechatronic-systems-programming` z pełną zawartością repozytorium.

## Ścieżka awaryjna: Download ZIP

Jeśli instalacja Gita się nie powiodła albo wolisz na razie jej nie rozwiązywać — nie blokuj się
na tym. Wejdź na stronę repozytorium na GitHubie, kliknij zielony przycisk **Code**, a następnie
**Download ZIP**. Rozpakuj archiwum w wybranym miejscu na dysku.

**To rozwiązanie tymczasowe.** Folder z rozpakowanego ZIP-a nie jest repozytorium Git — nie da się
w nim aktualizować zmian ani ich zapisywać w historii. Wystarczy do przejścia przez ten moduł;
zanim zaczniesz zapisywać własne zmiany w kolejnym module, wrócimy do `git clone`.

## Sprawdzenie

Niezależnie od wybranej ścieżki, powinieneś mieć teraz folder `mechatronic-systems-programming`
zawierający m.in. plik `CMakeLists.txt` w katalogu głównym. To sygnał, że masz kompletne
repozytorium, gotowe do otwarcia w IDE.

**Dalej:** [pierwszy build](./03_pierwszy_build.md).
