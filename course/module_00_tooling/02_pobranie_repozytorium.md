🇵🇱 Polski | [🇬🇧 English](02_pobranie_repozytorium.en.md)

# 0.2 Pobranie repozytorium

Na razie Git posłuży tylko do pobrania plików kursu. Do commitów i branchy wrócimy, gdy pojawią się
pierwsze własne zmiany do zapisania.

## Ścieżka podstawowa: git clone

Otwórz terminal (na Windows możesz użyć programu **Git Bash**, zainstalowanego razem z Git for
Windows) i wykonaj, w miejscu na dysku, gdzie chcesz trzymać materiały kursu:

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

Niezależnie od wybranej ścieżki, powinieneś mieć teraz folder `mechatronic-systems-programming`
zawierający m.in. plik `CMakeLists.txt` w katalogu głównym. To sygnał, że masz kompletne
repozytorium, gotowe do otwarcia w IDE.

**Dalej:** [pierwsze budowanie projektu](./03_pierwszy_build.md).
