🇵🇱 Polski *(materiał kursu — wersja angielska jeszcze nie istnieje)*

# Materiał kursu

← [README](../README.md) · [Roadmap](../docs/roadmap.md) · [Handbook](../docs/handbook.md)

## Zanim zaczniesz czytać dowolny moduł

**Ten katalog (`course/`) czytasz na `main` — to kanoniczne, aktualne źródło instrukcji.** Kod, który
piszesz, żyje gdzie indziej: na własnej gałęzi, założonej z punktu startowego danego modułu
(`module-XX-start`), dokładnie tak, jak opisuje
[Roadmap](../docs/roadmap.md#skąd-czytać-skąd-brać-kod):

```bash
git fetch --tags
git switch -c my-work module-XX-start
```

Linki do kodu źródłowego wewnątrz misji (np. `[Plant](.../src/plant.cpp)`) prowadzą wprost do pliku
na GitHubie, przypiętego do właściwego tagu tej misji — nie do Twojego lokalnego checkoutu. Kliknięcie
takiego linku zawsze pokaże Ci dokładnie ten kod, o którym mówi tekst, niezależnie od tego, na jakiej
gałęzi akurat jesteś. Żeby edytować ten sam plik u siebie, znajdź go pod tą samą ścieżką (`src/...`,
`include/psm/...`) we własnym checkoucie z `module-XX-start`.

**Jedna rzecz, o której warto pamiętać:** jeśli czytasz materiał misji z poziomu własnej gałęzi
(lokalny plik `course/module_XX_.../NN_*.md`, nie widok na GitHubie), pamiętaj że ten plik to zrzut z
chwili, gdy `module-XX-start` powstał — może nie zawierać późniejszych poprawek redakcyjnych. Jeśli
coś w treści wygląda na niespójne z kodem albo z tym, co pamiętasz z zajęć, sprawdź aktualną wersję
tego pliku na `main`, zanim zgłosisz błąd.
