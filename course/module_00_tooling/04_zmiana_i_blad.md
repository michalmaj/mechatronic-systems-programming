🇵🇱 Polski | [🇬🇧 English](04_zmiana_i_blad.en.md)

# 0.4 Zmiana i błąd

Program już działa. Teraz wprowadzisz prostą zmianę, a następnie świadomie wywołasz błąd kompilacji.
Najlepiej pierwszy raz zobaczyć taki błąd w małym programie, który łatwo naprawić.

## Krok 1: nieszkodliwa zmiana

Otwórz plik `apps/toolchain_check/main.cpp` w swoim IDE. Znajdziesz w nim jedną linijkę
odpowiedzialną za wypisywany tekst. Zmień treść napisu na dowolną inną (np. dopisz swoje imię).
Zapisz plik i uruchom program ponownie (tak jak w poprzednim kroku).

Nie trzeba osobno uruchamiać budowania. Po kliknięciu Run IDE zauważa zmianę pliku i ponownie
wywołuje kompilator przed startem programu. Zachodzi więc sekwencja opisana w kroku 0.0: zmiana pliku
źródłowego → kompilator → nowy plik wykonywalny → nowy proces.

Powinieneś zobaczyć swój zmieniony napis. Jeśli tak — działa.

## Krok 2: celowy błąd

Teraz zepsujemy coś celowo. W tym samym pliku usuń jeden średnik (`;`) na końcu dowolnej linii z
kodem. Zapisz plik i spróbuj uruchomić program ponownie.

Tym razem program się nie uruchomi. Zamiast tego zobaczysz komunikat błędu kompilatora — zwykle
czerwonym tekstem, w oknie "Output"/"Build" (Visual Studio) albo w oknie komunikatów kompilatora
(CLion).

## Jak czytać taki komunikat

Typowy komunikat błędu zawiera trzy rzeczy, których szukamy w tej kolejności:

1. **Nazwę pliku i numer linii** — wskazuje, gdzie kompilator napotkał problem. Czasem
   nie musi to być linia, w której powstał błąd — brakujący średnik często ujawnia się dopiero w
   *następnej* linii, bo dopiero tam kompilator "gubi wątek".
2. **Pierwszy komunikat błędu** — jeśli widzisz kilkanaście linii błędów naraz, skup się na
   pierwszym. Brakujący średnik potrafi wywołać lawinę kolejnych, pozornie niezwiązanych błędów —
   napraw pierwszy, a reszta często zniknie sama.
3. **Treść komunikatu** — kompilatory C++ bywają rozwlekłe, ale zwykle da się z nich wyłuskać
   sedno (np. `expected ';'` — "oczekiwano średnika").

## Krok 3: naprawa

Wróć do zepsutej linii, przywróć średnik, zapisz plik, uruchom ponownie. Powinieneś znowu zobaczyć
swój napis z Kroku 1.

Ten cykl będzie wracał przez cały kurs: zmiana → błąd → odczytanie komunikatu → poprawka → ponowne
uruchomienie. Błędy kompilacji są zwykłą częścią pracy nad kodem.

**Dalej:** [podgląd symulatora](./05_podglad_symulatora.md).
