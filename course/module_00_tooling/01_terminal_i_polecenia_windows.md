🇵🇱 Polski | [🇬🇧 English](01_terminal_i_polecenia_windows.en.md)

# 0.1 Terminal i podstawowe polecenia: Windows

Zanim zainstalujesz narzędzia kursu, oswój się z terminalem. Przez większość Modułu 0 pracujesz w
IDE, ale od kolejnych modułów terminal będzie potrzebny co tydzień — do pobierania kodu i
uruchamiania testów.

## Otwórz PowerShell

1. Naciśnij klawisz Windows (albo kliknij przycisk Start).
2. Wpisz `PowerShell`.
3. Kliknij aplikację **Windows PowerShell** (albo **PowerShell**).

Powinno otworzyć się okno z tekstem podobnym do:

```
PS C:\Users\TwojaNazwa>
```

To **prompt** — znak, że PowerShell czeka na Twoją komendę. Nie wpisujesz go sam/a.

## Gdzie jestem?

```powershell
Get-Location
```

Wypisze folder, w którym aktualnie "stoi" terminal, np.:

```
Path
----
C:\Users\TwojaNazwa
```

## Co tu jest?

```powershell
Get-ChildItem
```

Wylistuje pliki i foldery w aktualnym miejscu.

## Przejdź do folderu

```powershell
cd Documents
```

Sprawdź, gdzie jesteś: `Get-Location`. Powrót wyżej: `cd ..`.

## Ćwiczenie w piaskownicy

Wykonaj po kolei, żeby przestać się bać poleceń:

```powershell
cd $HOME
mkdir terminal-cwiczenia
cd terminal-cwiczenia
Get-Location
New-Item notatka.txt -ItemType File
Get-ChildItem
Remove-Item notatka.txt
Get-ChildItem
cd ..
```

Każdy krok robi jedno: tworzy folder ćwiczeniowy, wchodzi do niego, tworzy plik, pokazuje listę,
usuwa plik, znowu pokazuje listę (powinna być pusta), wraca wyżej.

## Czym są "uprawnienia administratora"?

Czasem Windows pyta, czy zezwolić programowi na zmiany w systemie (okno **UAC** — User Account
Control). To osobny temat od samego terminala — w tym kroku go nie potrzebujesz. Jeśli pojawi się
takie okno podczas instalacji Visual Studio, kliknij **Tak**/**Yes** — to normalne. Do pełnego
wyjaśnienia (i do PowerShell *jako* administrator) wrócimy w opcjonalnym kroku o Chocolatey.

## Jeśli coś poszło nie tak

- **Nie widzę PowerShell na liście:** sprawdź literówkę; na nowszych wersjach Windows może się
  nazywać **Terminal** i otwierać zakładkę PowerShell.
- **"cd: Cannot find path":** jesteś w innym folderze niż myślisz — sprawdź `Get-Location`, a potem
  `Get-ChildItem`, żeby zobaczyć, co naprawdę jest dostępne.
- **Polecenie nie jest rozpoznane:** sprawdź literówki; PowerShell nie rozróżnia wielkości liter w
  nazwach komend, ale spacje i myślniki mają znaczenie.

**Dalej:** [instalacja](./02_instalacja_windows.md).
