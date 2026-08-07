# 0.5 Podgląd: dokąd zmierzamy

Ostatni krok tego modułu nie uczy niczego nowego technicznie — to samo budowanie i uruchamianie,
które już znasz z `toolchain_check`. Chcemy Ci tylko pokazać, dokąd zmierzamy.

## Uruchom `simulator_cli`

Tak samo jak poprzednio: wybierz target `simulator_cli` z listy w swoim IDE (zamiast
`toolchain_check`) i uruchom go.

## Czego się spodziewać

Zobaczysz kilkanaście linii tekstu, coś w rodzaju:

```
tick 0: mode=Running, item 1 in zone Infeed
tick 1: mode=Running, item 1 in zone Infeed
...
```

To jest symulator małej stacji sortującej paczki — system, który będziesz rozwijał krok po kroku
przez cały semestr. Na tym etapie **nie musisz rozumieć ani jednej linijki tego, co się dzieje**.
Zobaczysz w tym wyjściu wzmianki o trybach pracy, strefach, czujnikach — do każdego z tych pojęć
wrócimy osobno, w swoim czasie, kiedy będą miały znaczenie.

To, co ważne teraz, to jedno spostrzeżenie: uruchomiłeś właśnie ten sam projekt, w którym przed
chwilą naprawiałeś brakujący średnik. Jeden projekt C++ — od najmniejszego możliwego programu
(`toolchain_check`) po coś, co wygląda już jak prawdziwy system. Cały kurs to właśnie to: ten sam
projekt, rosnący krok po kroku.

## Koniec modułu 0

Jeśli dotarłeś tutaj i widziałeś działające wyjście obu programów — masz gotowe środowisko i
wiesz, jak wygląda podstawowy cykl pracy. To wszystko, czego potrzebujesz, żeby zacząć.
