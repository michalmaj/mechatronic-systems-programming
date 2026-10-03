🇵🇱 Polski | [🇬🇧 English](05_podglad_symulatora.en.md)

# 0.5 Podgląd: dokąd zmierzamy

W ostatnim kroku ponownie zbudujesz i uruchomisz program, tym razem po to, by zobaczyć efekt końcowy
kursu.

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

To symulator niewielkiej komórki sortującej, który będziesz rozwijać przez cały semestr. Nie próbuj
jeszcze analizować każdej linii. Tryby pracy, strefy i czujniki zostaną wyjaśnione w kolejnych
modułach.

Oba programy należą do tego samego projektu C++: prosty `toolchain_check` i znacznie większy
symulator. W następnych modułach będziesz stopniowo budować ten drugi.

## Koniec modułu 0

Jeśli dotarłeś tutaj i widziałeś działające wyjście obu programów — masz gotowe środowisko i
wiesz, jak wygląda podstawowy cykl pracy. To wszystko, czego potrzebujesz, żeby zacząć.
