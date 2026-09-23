🇵🇱 Polski *(materiał kursu — wersja angielska jeszcze nie istnieje)*

# Materiał kursu

← [README](../README.md) · [Roadmap](../docs/roadmap.md) · [Handbook](../docs/handbook.md)

## Zanim zaczniesz czytać dowolny moduł

Pliki w tym katalogu linkują do kodu źródłowego ścieżkami względnymi, np.
`[Plant](../../src/plant.cpp)`. Te linki są poprawne wtedy, gdy czytasz materiał **na własnej gałęzi,
utworzonej z punktu startowego tego modułu** — dokładnie tak, jak opisuje
[Roadmap](../docs/roadmap.md#jak-pracować-w-każdym-module):

```bash
git fetch --tags
git switch -c my-work module-XX-start
```

Wtedy `../../src/plant.cpp` wskazuje na Twój własny, lokalny kod tego modułu — dokładnie ten, o
którym mówi tekst.

**Jeśli zamiast tego przeglądasz ten plik na GitHubie w kontekście gałęzi `main`,** te same linki
mogą prowadzić do innego kodu niż ten, o którym mówi tekst, albo do pliku, który na tym etapie
jeszcze nie istnieje. To nie błąd w materiale — to konsekwencja struktury tego repozytorium: `main`
zawiera materiał kursu (`course/`, `docs/`) oraz zrzut demonstracyjny z Modułu 0 — ten sam
`simulator_cli`, który uruchamiasz w misji 0.5 jako podgląd tego, dokąd zmierzasz. Właściwa,
budowana krok po kroku implementacja, którą rozwijasz przez cały kurs, żyje na tagach
`module-XX-start`/`module-XX-solution`, nie na `main`. Dlatego linki do kodu źródłowego wewnątrz
materiałów misji trzeba śledzić w kontekście właściwego taga, nie na `main`.

Zasada jest prosta: **materiał czytaj skądkolwiek, ale linki do kodu klikaj (albo otwieraj lokalnie)
dopiero mając wyewidencjonowany właściwy tag.**
