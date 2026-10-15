🇵🇱 Polski | [🇬🇧 English](07_pierwszy_ctest.en.md)

# 0.7 Pierwsze uruchomienie testów

Projekt już się buduje i wiesz, jak czytać błąd kompilacji. Teraz uruchomisz testy: to druga,
równie ważna metoda sprawdzania, czy kod działa poprawnie. Od Modułu 1 każda misja kończy się
właśnie testem.

## Czym są testy w tym projekcie

W katalogu `tests/` znajdują się małe programy sprawdzające, czy poszczególne części symulatora
(np. `Diverter`, `BeltMotor`, `Engine`) zachowują się tak, jak powinny. `CTest`, dołączony do
CMake, uruchamia je wszystkie i podsumowuje wynik.

## Uruchom testy z terminala

W folderze repozytorium (ta sama komenda na Windows, Linux i macOS). Jeśli wcześniej budowałeś
tylko przez IDE (przyciskiem Run), CTest potrzebuje najpierw tego samego skonfigurowanego i
zbudowanego folderu `build/dev`, więc wykonaj po kolei:

```bash
cmake --preset dev
cmake --build --preset dev
ctest --preset test
```

Pierwsze dwie komendy robią to samo, co IDE robiło dotąd za Ciebie jednym kliknięciem Run. Jeśli
już budowałeś w tym folderze repozytorium (np. w poprzednim kroku, w IDE), możesz od razu
przejść do samego `ctest --preset test`.

Zobaczysz listę testów przelatujących jeden po drugim, a na końcu podsumowanie podobne do:

```
100% tests passed, 0 tests failed out of 22

Total Test time (real) =   5.82 sec
```

Dokładna liczba testów może się różnić w zależności od wersji repozytorium, liczy się
`100% tests passed` i `0 tests failed`.

## Co to sprawdza

Te testy chronią już istniejący rdzeń symulatora: ten, który będziesz rozwijać przez cały semestr.
Jeśli kiedyś coś zepsujesz, część testów się zaczerwieni (`Failed`) i wskaże, co konkretnie nie
działa, zamiast zmuszać Cię do szukania błędu „na oko”.

## Jeśli coś poszło nie tak

- **"No tests were found!!!" (polecenie nie zgłasza błędu, po prostu nic nie testuje):** folder
  `build/dev` nie został jeszcze skonfigurowany i zbudowany z terminala. Wykonaj
  `cmake --preset dev`, potem `cmake --build --preset dev`, a następnie wróć do `ctest --preset test`.
- **"ctest: command not found" (na Windows, w zwykłym PowerShell):** CTest instaluje się razem
  z CMake, ale komponent CMake z Visual Studio („C++ CMake tools for Windows") nie trafia do
  zwykłego PowerShell. Otwórz z menu Start **Developer PowerShell for VS 2022**: to specjalne
  okno PowerShell, które Visual Studio dodaje do systemu i w którym `cmake`/`ctest` są już
  widoczne, i spróbuj w nim ponownie.
- **"ctest: command not found" (na Linux/macOS):** sprawdź `cmake --version` i wróć do kroku
  instalacji, jeśli go nie ma.
- **"CMake Error: Could not read presets from ...: File not found: .../CMakePresets.json":**
  jesteś w innym folderze niż główny folder repozytorium (ten, w którym faktycznie leży plik
  `CMakePresets.json`); przejdź do niego (`cd`) i spróbuj ponownie.
- **Niektóre testy są "Failed":** zanim zaczniesz szukać błędu we własnym kodzie, sprawdź, czy
  wcześniej budowanie (`cmake --build --preset dev`) zakończyło się bez błędów; testy uruchamiają
  się na ostatnio zbudowanym kodzie.

## Od Modułu 1

Będziesz uruchamiać testy pojedynczej misji, np.:

```bash
ctest --preset test -L misja-1
```

Flaga `-L misja-1` ogranicza uruchomienie do testów oznaczonych tą etykietą. Zamiast czekać na
cały zestaw, sprawdzasz tylko to, nad czym właśnie pracujesz.

**Dalej:** [podgląd symulatora](./08_podglad_symulatora.md).
