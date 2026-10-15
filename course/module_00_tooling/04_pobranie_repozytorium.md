🇵🇱 Polski | [🇬🇧 English](04_pobranie_repozytorium.en.md)

# 0.4 Pobranie repozytorium

Na razie Git posłuży do pobrania plików kursu i do jednej próby generalnej schematu, który będzie
wracał w każdym module. Do prawdziwych commitów i branchy wrócimy w Module 1, kiedy pojawi się
pierwsza własna zmiana do zapisania.

Zanim zaczniesz, trzy krótkie pojęcia:

- **Repozytorium** to po prostu folder, którego historię zmian pamięta Git. Każda zmiana, którą
  kiedykolwiek zapisano, jest w nim nadal dostępna.
- **Tag** to nazwana, zamrożona chwila w tej historii, na przykład `module-01-start`. Tagi w tym
  kursie się nie przesuwają: to stały punkt odniesienia.
- **Gałąź (branch)** to Twoja własna linia pracy, zaczynająca się od wybranego punktu (najczęściej
  właśnie od tagu). Możesz na niej zmieniać kod bez wpływu na `main` i innych.

## Ścieżka podstawowa: git clone

Otwórz PowerShell (ten, którego uczyłeś się w poprzednim kroku) i przejdź do folderu, w którym
chcesz trzymać materiały kursu, na przykład do folderu domowego:

```powershell
cd $HOME
```

Następnie wykonaj:

```bash
git clone https://github.com/michalmaj/mechatronic-systems-programming.git
```

Powstanie folder `mechatronic-systems-programming` z pełną zawartością repozytorium.

Na Windows możesz też skorzystać z **Git Bash**, instalowanego razem z Git for Windows. To inny
terminal niż PowerShell, z inną rodziną poleceń (np. `pwd` i `ls` zamiast `Get-Location` i
`Get-ChildItem`). Dla tego kursu wystarczy PowerShell; Git Bash wspomnimy tu tylko jako alternatywę.

## Ścieżka awaryjna: Download ZIP

Jeśli instalacja Gita się nie powiodła, możesz tymczasowo pobrać pliki jako archiwum. Na stronie
repozytorium w GitHubie kliknij zielony przycisk **Code**, wybierz **Download ZIP** i rozpakuj
archiwum w wybranym miejscu.

**To rozwiązanie tymczasowe.** Folder z rozpakowanego ZIP-a nie jest repozytorium Git. Nie da się
w nim aktualizować zmian ani ich zapisywać w historii. Wystarczy do przejścia przez ten moduł.
Zanim zaczniesz zapisywać własne zmiany w kolejnym module, wrócimy do `git clone`.

**Jeśli jesteś na tej ścieżce, pomiń sekcję „Jak będziesz pracować w każdym module” poniżej.** Wymaga
ona prawdziwego repozytorium Git, którego folder z ZIP-a nie jest.

## Sprawdzenie

Niezależnie od wybranej ścieżki, powinieneś mieć teraz folder `mechatronic-systems-programming`
zawierający m.in. plik `CMakeLists.txt` w katalogu głównym. To sygnał, że masz kompletne
repozytorium, gotowe do otwarcia w IDE.

## Jak będziesz pracować w każdym module

Od Modułu 1 każdy moduł zaczynasz tym samym schematem. Poćwiczmy go teraz na sucho, na nazwie
brancha, której nie będziesz używać na serio (`cwiczenie-00`), żebyś nie pomylił tego z prawdziwym
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

Powinieneś zobaczyć w pierwszej linii `On branch cwiczenie-00`.

To wszystko, na razie nic z tą gałęzią nie robimy, to tylko próba generalna. Wróć na `main` i usuń
ćwiczebną gałąź, bo była tylko na chwilę:

```bash
git switch main
git branch -d cwiczenie-00
```

Usuwanie gałęzi w Gicie jest bezpieczne: `main` i tagi, z których gałąź powstała, zostają bez
zmian; usuwasz tylko etykietę Twojej tymczasowej linii pracy.

Dokładnie ten schemat (`git fetch --tags` → `git switch -c my-work module-XX-start`) powtórzy się w
każdym module, tylko z Twoją prawdziwą nazwą gałęzi (`my-work`) i właściwym numerem modułu. Pełny
opis: [Plan pracy studenta](../../docs/roadmap.md#jak-pracować-w-każdym-module).

## Jeśli coś poszło nie tak

- **"fatal: destination path already exists":** folder `mechatronic-systems-programming` już
  istnieje z wcześniejszej próby. Usuń go albo pracuj w innym miejscu na dysku.
- **"error: pathspec 'module-01-start' did not match any file(s) known to git":**
  `git fetch --tags` się nie wykonał albo nie masz połączenia z internetem. Spróbuj ponownie.
- **"fatal: not a git repository":** prawdopodobnie jesteś na ścieżce Download ZIP. Ta sekcja
  wymaga repozytorium z `git clone`; pomiń ją na razie.
- **Nie wiesz, na jakiej gałęzi jesteś:** `git status` zawsze powie Ci to w pierwszej linii.

**Dalej:** [pierwsze budowanie projektu](./05_pierwszy_build.md).
