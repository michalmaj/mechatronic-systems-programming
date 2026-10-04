🇵🇱 Polski | [🇬🇧 English](00_wprowadzenie.en.md)

# 5.0 Wprowadzenie

Do tej pory bezpieczne zatrzymanie systemu zależało od poprawnego działania `Mode`, sterownika i
pozostałej logiki. W tym module dodasz **drugą, niezależną** ścieżkę reagowania na zatrzymanie
awaryjne. Nie będzie ona zależeć od decyzji sterownika ani od logiki wyboru trasy paczki.

Powstający tu mechanizm jest uproszczonym modelem dydaktycznym. Pokazuje trzy ważne cechy:
niezależną ścieżkę reakcji, bezwzględny priorytet zatrzymania oraz zakaz automatycznego wznowienia
pracy. To **nie jest** projekt układu bezpieczeństwa spełniającego normy dla prawdziwej maszyny.
Takie układy wymagają między innymi certyfikowanych podzespołów, odpowiednich obwodów sprzętowych i
analizy zgodności z normami. Ten kurs nie obejmuje ich projektowania.

## Skąd startujesz

```bash
git fetch --tags
git switch -c <nazwa-twojego-brancha> module-05-start
```

Do ostatniej misji pozostaw `Engine::step()` w wersji z modułu 4. Najpierw zbudujesz przycisk
awaryjny, rozszerzony `Mode` i funkcje bezpieczeństwa jako osobne elementy, które można testować
niezależnie. Dopiero w ostatniej misji połączysz je z `Engine`.

## Mapa modułu

Ten moduł ma **cztery** misje zamiast trzech.

1. **Przycisk awaryjny:** `EStopLatchState`, czyli kolejny niewielki automat stanów.
2. **Tryb zatrzymania awaryjnego:** rozszerzenie `Mode` o wartość `EStopped`.
3. **Dwie niezależne ścieżki:** osobne funkcje decyzyjne oraz operacja wymuszająca zatrzymanie w
   modelu silnika.
4. **Obsługa zatrzymania awaryjnego w `Engine`:** połączenie wszystkich elementów w
   `Engine::step()`.

## Zanim zaczniesz

- Testy modułów 1–4 (`misja-1`–`misja-4`, `misja-6`–`misja-15`) są już obecne i przechodzą.
- Tak jak zawsze: nie edytujesz plików testowych ani `CMakeLists.txt`.

**Dalej:** [Misja 16: przycisk awaryjny](./01_przycisk_awaryjny.md).
