# 0.4 Zmiana i błąd

Twój program działa. Teraz nauczymy się dwóch rzeczy naraz: jak wygląda zwykła zmiana kodu i jak
wygląda błąd kompilatora — bo prędzej czy później go zobaczysz, i lepiej oswoić się z nim teraz,
na programie, w którym nic nie możesz zepsuć na trwałe.

## Krok 1: nieszkodliwa zmiana

Otwórz plik `apps/toolchain_check/main.cpp` w swoim IDE. Znajdziesz w nim jedną linijkę
odpowiedzialną za wypisywany tekst. Zmień treść napisu na dowolną inną (np. dopisz swoje imię).
Zapisz plik i uruchom program ponownie (tak jak w poprzednim kroku).

Zwróć uwagę: nie musiałeś nic "przebudowywać" ręcznie osobnym poleceniem — kliknięcie Run w IDE
samo zauważa, że plik źródłowy się zmienił, i przed uruchomieniem najpierw wywołuje kompilator
ponownie. To jest dokładnie ta sama sekwencja, o której mówiliśmy w kroku 0.0: zmiana pliku
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

1. **Nazwę pliku i numer linii** — dokładnie wskazuje, gdzie kompilator napotkał problem. Czasem
   to nie jest dokładnie ta linia, którą zepsułeś — brakujący średnik często ujawnia się dopiero w
   *następnej* linii, bo dopiero tam kompilator "gubi wątek".
2. **Pierwszy komunikat błędu** — jeśli widzisz kilkanaście linii błędów naraz, skup się na
   pierwszym. Brakujący średnik potrafi wywołać lawinę kolejnych, pozornie niezwiązanych błędów —
   napraw pierwszy, a reszta często zniknie sama.
3. **Treść komunikatu** — kompilatory C++ bywają rozwlekłe, ale zwykle da się z nich wyłuskać
   sedno (np. `expected ';'` — "oczekiwano średnika").

## Krok 3: naprawa

Wróć do zepsutej linii, przywróć średnik, zapisz plik, uruchom ponownie. Powinieneś znowu zobaczyć
swój napis z Kroku 1.

To jest cały cykl, który będziesz powtarzać setki razy w tym kursie: zmiana → (czasem) błąd →
odczytanie komunikatu → naprawa → sukces. Nie ma w nim nic wstydliwego — błędy kompilacji są
normalną, codzienną częścią pisania kodu, nie oznaką, że coś jest z Tobą nie tak.

**Dalej:** [podgląd symulatora](./05_podglad_symulatora.md).
