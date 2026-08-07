# 0.1 Instalacja: Windows

Potrzebujesz dwóch rzeczy: Visual Studio 2022 (z odpowiednim zestawem komponentów) oraz Gita.

## Visual Studio 2022

1. Pobierz instalator ze strony visualstudio.microsoft.com — wystarczy darmowa edycja
   **Community**.
2. Uruchom instalator. Zobaczysz listę **obciążeń (workloads)** — dużych zestawów komponentów.
   Zaznacz **"Desktop development with C++"** ("Programowanie aplikacji klasycznych w C++").
3. Po prawej stronie, w panelu "Szczegóły instalacji" (Installation details), upewnij się, że
   zaznaczony jest komponent **"C++ CMake tools for Windows"** — zwykle jest zaznaczony domyślnie
   razem z tym obciążeniem, ale warto sprawdzić. To on daje Ci CMake i Ninja, bez osobnej
   instalacji.
4. Kliknij Zainstaluj. To może chwilę potrwać — kilka gigabajtów danych.

Nie potrzebujesz żadnych innych obciążeń (np. do tworzenia aplikacji mobilnych czy webowych) —
zaznaczenie ich tylko wydłuży instalację i zajmie więcej miejsca na dysku.

## Git for Windows

1. Pobierz instalator z git-scm.com.
2. Uruchom instalator — domyślne ustawienia są w porządku, możesz klikać "Next" przez cały
   proces.

W tym module Git posłuży wyłącznie do jednej rzeczy: pobrania repozytorium kursu na dysk. Do
samego Gita wrócimy szerzej w późniejszym module.

## Sprawdzenie

Otwórz Visual Studio 2022. Z ekranu startowego powinieneś zobaczyć opcję **"Open a local
folder"** — do tego wrócimy w następnym kroku.

**Dalej:** [pobranie repozytorium](./02_pobranie_repozytorium.md).
