🇵🇱 Polski | [🇬🇧 English](01_terminal_i_polecenia_linux.en.md)

# 0.1 Terminal i podstawowe polecenia: Linux

Zanim zainstalujesz narzędzia kursu, oswój się z terminalem. Przez większość Modułu 0 pracujesz w
IDE, ale od kolejnych modułów terminal będzie potrzebny co tydzień.

## Otwórz terminal

Zależnie od środowiska graficznego: poszukaj w menu aplikacji frazy "Terminal", albo użyj skrótu
klawiszowego (często Ctrl+Alt+T). Powinno otworzyć się okno z promptem podobnym do:

```
twojanazwa@komputer:~$
```

## Gdzie jestem? Co tu jest?

```bash
pwd
ls
```

`pwd` (print working directory) pokazuje aktualny folder. `ls` listuje jego zawartość.

## Przejdź do folderu

```bash
cd Documents
```

`cd ..` wraca wyżej. `cd ~` wraca do folderu domowego.

## Ćwiczenie w piaskownicy

```bash
cd ~
mkdir terminal-cwiczenia
cd terminal-cwiczenia
pwd
touch notatka.txt
ls
rm notatka.txt
ls
cd ..
```

## Czym jest `sudo`?

W kroku instalacji zobaczysz polecenie `sudo apt install ...`. `sudo` to skrót od "wykonaj jako
administrator". System poprosi o Twoje hasło. To normalne, to hasło Twojego zwykłego konta, nie
jakieś nowe hasło roota. Podczas wpisywania hasła w terminalu **nic się nie wyświetla**, nawet
gwiazdki. To zamierzone zabezpieczenie, nie błąd. Wpisz hasło i naciśnij Enter.

## Jeśli coś poszło nie tak

- **"command not found":** sprawdź literówki.
- **Hasło po `sudo` "nie działa":** sprawdź Caps Lock; pamiętaj, że ekran nie pokazuje wpisywanych
  znaków, nawet gwiazdek.
- **"Permission denied" bez `sudo`:** niektóre operacje (np. instalacja pakietów systemowych)
  wymagają `sudo`; samo poruszanie się po swoich plikach nie.

**Dalej:** [instalacja](./02_instalacja_linux.md).
