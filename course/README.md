🇵🇱 Polski | [🇬🇧 English](README.en.md)

# Materiał kursu

← [README](../README.md) · [Plan pracy](../docs/roadmap.md) · [Podręcznik](../docs/handbook.md)

## Zanim zaczniesz czytać dowolny moduł

**Materiały z katalogu `course/` czytaj na `main`, gdzie znajduje się ich najnowsza wersja.** Kod
pisz na własnej gałęzi utworzonej z punktu startowego danego modułu (`module-XX-start`), zgodnie z
[planem pracy](../docs/roadmap.md#skąd-czytać-skąd-brać-kod):

```bash
git fetch --tags
git switch -c my-work module-XX-start
```

Odsyłacze do kodu źródłowego w opisach misji (np. `[Plant](.../src/plant.cpp)`) otwierają plik z
odpowiedniego tagu na GitHubie, a nie z Twojego lokalnego katalogu. Plik do edycji znajdziesz u siebie
pod tą samą ścieżką (`src/...`, `include/psm/...`).

Plik Markdown w tagu `module-XX-start` jest kopią materiału z chwili publikacji tagu i może nie
zawierać późniejszych poprawek redakcyjnych. Jeśli coś w treści wygląda na niespójne z kodem albo z
tym, co pamiętasz z zajęć, sprawdź aktualną wersję tego pliku na `main`, zanim zgłosisz błąd.
