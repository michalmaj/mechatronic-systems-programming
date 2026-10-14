🇵🇱 Polski | [🇬🇧 English](03_instalacja_git_chocolatey_windows.en.md)

# 0.3 Instalacja (opcjonalnie): Git przez Chocolatey

Ten krok jest **opcjonalny**. Jeśli masz już Gita z poprzedniego kroku (instalator ze strony
git-scm.com), możesz tę stronę pominąć i przejść dalej. Jeśli chcesz poćwiczyć instalowanie narzędzi
z terminala — przyda się to w pracy programisty — zostań tutaj.

## Co to menedżer pakietów?

Menedżer pakietów instaluje i aktualizuje programy jedną komendą, zamiast szukania instalatora w
przeglądarce i przeklikiwania okien. Na Windows popularnym menedżerem jest **Chocolatey**.

## Normalny PowerShell a PowerShell jako administrator

Do teraz otwierałeś/aś zwykły PowerShell. Instalacja Chocolatey wymaga **PowerShell jako
administrator** — czyli z podniesionymi uprawnieniami do zmiany systemu.

Jak otworzyć PowerShell jako administrator:

1. Otwórz menu Start.
2. Wpisz `PowerShell`.
3. Kliknij prawym przyciskiem myszy na **Windows PowerShell**.
4. Wybierz **Uruchom jako administrator** (**Run as administrator**).
5. Jeśli pojawi się systemowe okno (UAC) z pytaniem o zgodę, kliknij **Tak**.

Okno wygląda podobnie do zwykłego PowerShell, ale ma wyższe uprawnienia — stąd np. w tytule okna
często widać dopisek "Administrator". **Używaj administratora tylko wtedy, gdy instrukcja tego
wymaga** — nie jako stały sposób pracy.

## Instalacja Chocolatey

W oknie PowerShell **jako administrator** wklej:

```powershell
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
```

Co robi ta komenda:

1. Tymczasowo zezwala na uruchamianie skryptów instalacyjnych — tylko w tej jednej sesji PowerShell.
2. Włącza wymagany protokół bezpieczeństwa połączenia.
3. Pobiera oficjalny skrypt instalacyjny Chocolatey ze strony `community.chocolatey.org`.
4. Uruchamia ten skrypt.

**Uwaga bezpieczeństwa:** ta komenda pobiera i wykonuje skrypt z internetu. Uruchamiaj taką komendę
tylko z oficjalnej dokumentacji (`docs.chocolatey.org`), nigdy ze stron czy wiadomości, których nie
znasz.

Poczekaj, aż polecenie się zakończy bez błędów.

## Zamknij i otwórz nowy PowerShell

Zamknij okno administratora. Otwórz **zwykły** PowerShell z menu Start (nie musi już być jako
administrator).

## Sprawdzenie

```powershell
choco --version
```

Powinien wypisać numer wersji, np. `2.x.x`. Jeśli PowerShell nie rozpoznaje `choco`, zamknij okno i
otwórz nowe — czasem trzeba odświeżyć sesję.

## Instalacja Gita przez Chocolatey

W zwykłym (nie administratorskim) PowerShell:

```powershell
choco install git -y
```

Flaga `-y` oznacza "zgadzam się na wszystkie pytania automatycznie" — Chocolatey w trakcie
instalacji zwykle o to pyta.

Zamknij i otwórz nowy PowerShell, a potem sprawdź:

```powershell
git --version
```

Powinien wypisać numer wersji Gita.

## Jeśli coś poszło nie tak

- **"Scripts is disabled on this system":** nie użyłeś/aś PowerShell jako administrator, albo
  pominąłeś/aś pierwszą część komendy instalacyjnej (`Set-ExecutionPolicy ...`). Otwórz PowerShell
  jako administrator i spróbuj ponownie od początku tej strony.
- **"Access is denied":** to samo — potrzebujesz PowerShell jako administrator.
- **`choco` nie jest rozpoznawane po instalacji:** zamknij całkowicie PowerShell i otwórz nowe okno.
  Jeśli to nie pomoże, zrestartuj komputer.
- **Komputer szkolny/uczelniany blokuje instalację:** niektóre organizacje blokują instalację
  oprogramowania. Użyj wtedy instalatora Gita z poprzedniego kroku albo poproś o pomoc dział IT.

**Dalej:** [pobranie repozytorium](./04_pobranie_repozytorium.md).
