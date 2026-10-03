🇵🇱 Polski | [🇬🇧 English](01_instalacja_windows.en.md)

# 0.1 Instalacja: Windows

Potrzebujesz Visual Studio 2022 z narzędziami C++ oraz Gita.

## Visual Studio 2022

1. W tym kursie używamy **Visual Studio 2022**. Pobierz instalator z oficjalnej strony historii
   wydań tej wersji:
   [learn.microsoft.com/visualstudio/releases/2022/release-history](https://learn.microsoft.com/en-us/visualstudio/releases/2022/release-history)
   — wystarczy darmowa edycja **Community**.
2. Uruchom instalator i na liście zestawów składników zaznacz **Desktop development with C++**
   (**Programowanie aplikacji klasycznych w C++**).
3. W panelu **Installation details** (**Szczegóły instalacji**) sprawdź, czy jest zaznaczony składnik
   **C++ CMake tools for Windows**. Instaluje on CMake i Ninja, więc nie trzeba pobierać ich osobno.
4. Rozpocznij instalację. Do pobrania jest kilka gigabajtów danych.

Pozostałe zestawy, na przykład do aplikacji mobilnych lub internetowych, nie są potrzebne w tym
kursie.

## Git for Windows

1. Pobierz instalator z git-scm.com.
2. Uruchom instalator i pozostaw ustawienia domyślne.

W tym module Git posłuży wyłącznie do jednej rzeczy: pobrania repozytorium kursu na dysk. Do
samego Gita wrócimy szerzej w późniejszym module.

## Sprawdzenie

Otwórz Visual Studio 2022. Na ekranie startowym powinna być widoczna opcja **Open a local folder**
(**Otwórz folder lokalny**). Skorzystasz z niej w następnym kroku.

**Dalej:** [pobranie repozytorium](./02_pobranie_repozytorium.md).
