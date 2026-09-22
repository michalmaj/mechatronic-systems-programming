🇵🇱 Polski *(materiał kursu — wersja angielska jeszcze nie istnieje)*

# Materiał kursu

← [README](../README.md) · [Roadmap](../docs/roadmap.md) · [Handbook](../docs/handbook.md)

## Zanim zaczniesz czytać dowolny moduł

Pliki w tym katalogu linkują do kodu źródłowego ścieżkami względnymi, np.
`[Plant](../../src/plant.cpp)`. Te linki są poprawne wtedy, gdy czytasz materiał **na własnej gałęzi,
utworzonej z punktu startowego tego modułu** — dokładnie tak, jak opisuje
[Roadmap](../docs/roadmap.md#workflow-każdego-modułu):

```bash
git fetch --tags
git switch -c my-work module-XX-start
```

Wtedy `../../src/plant.cpp` wskazuje na Twój własny, lokalny kod tego modułu — dokładnie ten, o
którym mówi tekst.

**Jeśli zamiast tego przeglądasz ten plik na GitHubie w kontekście gałęzi `main`,** te same linki
prowadzą do zupełnie innego kodu, albo do pliku, który jeszcze nie istnieje na tym etapie. To nie
błąd w materiale — to konsekwencja struktury tego repozytorium: materiał kursu (`course/`) żyje na
`main`, ale kod, który budujesz krok po kroku, żyje na tagach `module-XX-start`/`module-XX-solution`,
nie na `main`. `main` sam w sobie zawiera osobny, niezwiązany z Course Core projekt referencyjny —
podobieństwo nazw plików jest przypadkowe.

Zasada jest prosta: **materiał czytaj skądkolwiek, ale linki do kodu klikaj (albo otwieraj lokalnie)
dopiero mając wyewidencjonowany właściwy tag.**
