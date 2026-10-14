🇵🇱 Polski | [🇬🇧 English](07_pierwszy_ctest.en.md)

# 0.7 Pierwsze uruchomienie testów

Projekt już się buduje i wiesz, jak czytać błąd kompilacji. Teraz uruchomisz testy — to druga,
równie ważna metoda sprawdzania, czy kod działa poprawnie. Od Modułu 1 każda misja kończy się
właśnie testem.

## Czym są testy w tym projekcie

W katalogu `tests/` znajdują się małe programy sprawdzające, czy poszczególne części symulatora
(np. `Diverter`, `BeltMotor`, `Engine`) zachowują się tak, jak powinny. `CTest`, dołączony do
CMake, uruchamia je wszystkie i podsumowuje wynik.

## Uruchom testy z terminala

W folderze repozytorium wykonaj (ta sama komenda na Windows, Linux i macOS):

```bash
ctest --preset test
```

Zobaczysz listę testów przelatujących jeden po drugim, a na końcu podsumowanie podobne do:

```
100% tests passed, 0 tests failed out of 22

Total Test time (real) =   5.82 sec
```

Dokładna liczba testów może się różnić w zależności od wersji repozytorium — liczy się
`100% tests passed` i `0 tests failed`.

## Co to sprawdza

Te testy chronią już istniejący rdzeń symulatora — ten, który będziesz rozwijać przez cały semestr.
Jeśli kiedyś coś zepsujesz, część testów się zaczerwieni (`Failed`) i wskaże, co konkretnie nie
działa, zamiast zmuszać Cię do szukania błędu "na oko".

## Jeśli coś poszło nie tak

- **"ctest: command not found":** CTest instaluje się razem z CMake; sprawdź `cmake --version` i
  wróć do kroku instalacji, jeśli go nie ma.
- **"Preset 'test' is not defined":** jesteś w innym folderze niż główny folder repozytorium (ten z
  `CMakePresets.json`); przejdź do niego (`cd`) i spróbuj ponownie.
- **Niektóre testy są "Failed":** zanim zaczniesz szukać błędu we własnym kodzie, sprawdź, czy
  wcześniej budowanie (`cmake --build --preset dev`) zakończyło się bez błędów; testy uruchamiają
  się na ostatnio zbudowanym kodzie.

## Od Modułu 1

Będziesz uruchamiać testy pojedynczej misji, np.:

```bash
ctest --preset test -L misja-1
```

Flaga `-L misja-1` ogranicza uruchomienie do testów oznaczonych tą etykietą — zamiast czekać na
cały zestaw, sprawdzasz tylko to, nad czym właśnie pracujesz.

**Dalej:** [podgląd symulatora](./08_podglad_symulatora.md).
