🇵🇱 Polski | [🇬🇧 English](03_pierwszy_build.en.md)

# 0.3 Pierwsze budowanie projektu

Masz zainstalowane narzędzia i pobrane repozytorium. Czas zbudować pierwszy program.

## Otwórz projekt

**Visual Studio 2022:** z ekranu startowego wybierz **Open a local folder** i wskaż folder
`mechatronic-systems-programming`. Visual Studio rozpozna plik `CMakeLists.txt` i samo skonfiguruje
projekt. Przy pierwszym otwarciu może to potrwać chwilę; postęp widać na dole okna.

**CLion:** z ekranu startowego wybierz **Open** i wskaż folder `mechatronic-systems-programming`.
CLion rozpozna plik `CMakeLists.txt` i automatycznie uruchomi konfigurację CMake. Poczekaj, aż
pasek postępu na dole zniknie.

## Znajdź i uruchom `toolchain_check`

To jeden z kilku programów (targetów) zdefiniowanych w tym projekcie. Jest najmniejszy,
stworzony właśnie do tego ćwiczenia.

**Visual Studio 2022:** na górnym pasku, obok przycisku uruchamiania (zielony trójkąt), znajduje
się lista rozwijana z dostępnymi targetami. Wybierz `toolchain_check.exe`, a następnie kliknij
zielony trójkąt (albo naciśnij F5).

**CLion:** w prawym górnym rogu znajduje się lista rozwijana z konfiguracjami uruchomieniowymi.
Wybierz `toolchain_check`, a następnie kliknij zielony trójkąt obok (albo naciśnij Shift+F10).

## Czego się spodziewać

Na dole ekranu, w oknie **Output** lub **Run**, powinieneś zobaczyć:

```
toolchain check: compiler, CMake, and IDE are wired together correctly.
```

Jeśli widzisz ten komunikat, kompilator, CMake i IDE poprawnie ze sobą współpracują. W ten sam sposób
będziesz budować wszystkie kolejne programy w kursie, w tym symulator sortowni.

## Jeśli coś poszło nie tak

Najczęstsze przyczyny problemów:
- **IDE nie rozpoznaje projektu jako CMake:** upewnij się, że otworzyłeś *folder*
  `mechatronic-systems-programming` (ten zawierający `CMakeLists.txt`), a nie jakiś folder
  wewnątrz niego.
- **Lista targetów jest pusta albo nie ma `toolchain_check`:** poczekaj, aż konfiguracja CMake
  się zakończy (pasek postępu na dole okna) i odśwież listę.
- **Błąd podczas konfiguracji CMake:** sprawdź, czy kompilator faktycznie się zainstalował
  (wróć do kroku instalacji dla swojego systemu i uruchom komendy sprawdzające wersję).

**Dalej:** [zmiana i błąd](./04_zmiana_i_blad.md).
