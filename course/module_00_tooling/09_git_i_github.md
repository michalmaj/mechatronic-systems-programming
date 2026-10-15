🇵🇱 Polski | [🇬🇧 English](09_git_i_github.en.md)

# 0.9 Git i GitHub: szerszy obraz (opcjonalnie)

Ta strona jest **opcjonalna**. Do zaliczenia kursu nie musisz z niczego tu opisanego korzystać,
w tym kursie nie zakładasz forka, nie zgłaszasz issue ani nie otwierasz pull requesta. Ale te
pojęcia pojawiają się bardzo często w prawdziwej pracy programistycznej, warto więc je rozumieć,
żeby nie być zaskoczonym, gdy ktoś o nich wspomni.

## Git to nie GitHub

`Git` to narzędzie, którego już używasz: lokalnie zapisuje historię zmian na Twoim dysku (`git
clone`, `git status`, `git switch`). Działa bez internetu i bez żadnej konkretnej firmy.

`GitHub` to serwis internetowy, który hostuje repozytoria Git i dodaje do nich narzędzia
współpracy: strony projektów, issues, pull requesty, forki. GitHub nie jest jedynym takim
serwisem (są też GitLab, Bitbucket i inne), ale jest najpopularniejszy, a ten kurs też z niego
korzysta.

## Fork: Twoja kopia czyjegoś repozytorium

**Fork** to Twoja własna kopia czyjegoś repozytorium, na Twoim koncie GitHub. Możesz w niej robić
co chcesz, bez wpływu na oryginał, bo to osobna kopia. Forkując to repozytorium kursu, miałbyś
własną wersję pod adresem w stylu `github.com/TwojaNazwa/mechatronic-systems-programming`,
niezależną od `github.com/michalmaj/mechatronic-systems-programming`.

## Upstream i synchronizacja forka

Repozytorium, z którego powstał fork, nazywa się **upstream**. Fork nie aktualizuje się sam: jeśli
oryginał dostanie nowe commity, Twój fork zostaje przy starej wersji, dopóki go nie zsynchronizujesz.
Na stronie forka na GitHubie jest do tego przycisk **Sync fork**. (To samo da się zrobić przez
terminal, dodając `upstream` jako kolejne zdalne repozytorium i robiąc `fetch` plus `merge`, ale
przycisk na GitHubie jest prostszy.)

## Issue: jak zgłosić problem

**Issue** to zgłoszenie w repozytorium na GitHubie: błąd, pytanie albo propozycja zmiany. To nie
jest fragment kodu, to tekst, który czyta osoba prowadząca projekt.

Dobre issue zawiera:

1. co próbowałeś zrobić,
2. co się stało,
3. co powinno się stać,
4. jak to powtórzyć (dokładne kroki albo komenda).

Zgłoszenia dotyczące tego kursu (błędy w kodzie, testach albo materiale) przyjmujemy właśnie przez
zakładkę **Issues** tego repozytorium na GitHubie.

## Pull request: jak proponuje się zmianę

**Pull request** (PR) to prośba o wmergowanie Twoich zmian (najczęściej z forka albo z brancha) do
innego repozytorium. Autor projektu przegląda zmianę, może o nią zapytać albo zaproponować
poprawki, i dopiero potem ją scala.

W tym kursie PR-ów nie używasz: Twoje commity zostają lokalnie, na Twoim komputerze. Ale to właśnie
tak pracuje się w zdecydowanej większości projektów open source i w firmach, więc warto znać ten
mechanizm, nawet jeśli teraz go nie potrzebujesz.

## Spróbuj sam (opcjonalnie)

Jeśli chcesz zobaczyć to na żywo, a nie tylko w teorii:

1. Załóż darmowe konto na github.com, jeśli jeszcze go nie masz.
2. Wejdź na stronę tego repozytorium na GitHubie i kliknij przycisk **Fork** w prawym górnym rogu.
3. Zobacz swoją własną kopię repozytorium pod swoim kontem.

Nie musisz nic więcej robić: to tylko po to, żeby zobaczyć, jak fork wygląda w praktyce. Nie musisz
zgłaszać żadnego issue ani otwierać pull requesta, żeby zaliczyć ten moduł.

## Koniec modułu 0

To już naprawdę koniec Modułu 0. Masz środowisko gotowe do pracy, znasz podstawowy cykl
zmiana → kompilacja → test, i rozumiesz, czym różni się Git od GitHub. Czas na Moduł 1.
