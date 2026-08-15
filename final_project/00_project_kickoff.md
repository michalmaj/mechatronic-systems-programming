# Project Kickoff: writing your first test

## Cel

Przez cały Course Core 0–9 testy istniały dla ciebie — pisane przez kurs, uruchamiane przez `ctest`,
czytane jako kontrakt do zrozumienia. Zanim zaczniesz Final Project, potrzebujesz jednej nowej
umiejętności: **napisania własnego testu, od zera, dla znanej ci już funkcji.**

To nie jest Moduł 10. To krótkie, ograniczone ćwiczenie pomostowe — jedna funkcja, jeden wzorzec CMake
do skopiowania, dwa-trzy przypadki testowe. CMake pozostaje mechaniką dostarczoną przez kurs, dokładnie
tak jak przez cały Core — nie jest tu nowym celem nauki, tylko czymś, co powtarzasz wg gotowego wzorca.

## 1. Anatomia istniejącego testu

Otwórz `tests/controller_test.cpp` — jeden z najkrótszych testów w Core:

```cpp
#include "support/check.hpp"
#include <psm/controller.hpp>

int main() {
    psmCheck(psm::classify(100) == psm::WeightClass::Light, "100g classifies as Light");
    psmCheck(psm::classify(499) == psm::WeightClass::Light, "499g classifies as Light");
    psmCheck(psm::classify(500) == psm::WeightClass::Heavy, "500g (at the threshold) classifies as Heavy");
    psmCheck(psm::classify(999) == psm::WeightClass::Heavy, "999g classifies as Heavy");
    return 0;
}
```

Cztery rzeczy, które warto tu zobaczyć jawnie:

- Test to zwykły plik `.cpp` z `main()` — żaden framework, żadna makro-magia. To, co uruchamia
  `ctest`, to zwykły plik wykonywalny.
- `psmCheck(warunek, opis)` (`tests/support/check.hpp`) sprawdza warunek; jeśli jest fałszywy, wypisuje
  `opis` na `stderr` i **kończy proces przez `std::exit(1)`** — pierwsze niespełnione sprawdzenie
  przerywa test natychmiast, reszta się nie wykonuje.
- `return 0;` na końcu `main()` to sygnał sukcesu dla `ctest` — test, który dotarł do końca bez
  wywołania `std::exit(1)`, zaliczył się.
- Przypadki są dobrane celowo, nie losowo: `100` i `499` (środek zakresu Light), `500` (dokładnie na
  granicy — to *tutaj* najłatwiej o błąd o jeden), `999` (środek zakresu Heavy). Granica progu jest
  przetestowana wprost, nie tylko "typowe" wartości.

## 2. Twoje zadanie: test dla `decideClassification`

`controller_test.cpp` testuje `classify(Grams mass) -> WeightClass` — czystą funkcję bez żadnego
warunku brzegowego poza samym progiem. Obok niej, w tym samym pliku `include/psm/controller.hpp`,
mieszka funkcja o bogatszym kontrakcie, stabilna od Modułu 6:

```cpp
std::optional<WeightClass> decideClassification(WeightReading weight);
```

```cpp
// include/psm/sensor_snapshot.hpp
struct WeightReading {
    ReadingStatus status;   // Ok, Missing, Stale
    Grams grams;
};
```

Zaimplementowana w `src/controller.cpp` tak:

```cpp
std::optional<WeightClass> decideClassification(WeightReading weight) {
    if (weight.status != ReadingStatus::Ok) {
        return std::nullopt;
    }
    return classify(weight.grams);
}
```

W przeciwieństwie do `classify`, ta funkcja ma **dwie** ścieżki do sprawdzenia: poprawny odczyt
(deleguje do `classify`) i niepoprawny odczyt (`Missing` lub `Stale` — zawsze `std::nullopt`,
niezależnie od tego, co akurat siedzi w `grams`).

**Napisz `tests/support_your_first_test.cpp`** (albo dowolną nazwę pliku — to nie jest misja z
ustaloną nazwą) sprawdzający co najmniej:

1. `status == Ok`, masa w zakresie Light → wynik to `WeightClass::Light`.
2. `status == Ok`, masa w zakresie Heavy (włącznie z progiem) → wynik to `WeightClass::Heavy`.
3. `status == Missing` → wynik to `std::nullopt`, **niezależnie od wartości `grams`** — spróbuj z
   masą, która "wyglądałaby" na Heavy, żeby upewnić się, że status faktycznie wygrywa.
4. `status == Stale` → jak wyżej.

Wzoruj strukturę pliku na `controller_test.cpp` — `#include "support/check.hpp"`, `#include
<psm/controller.hpp>`, seria `psmCheck(...)`, `return 0;`.

## 3. Rejestracja w CMake — gotowy wzorzec do skopiowania

`tests/CMakeLists.txt` rejestruje każdy test trzema liniami. Dla nowego pliku
`tests/support_your_first_test.cpp` dopisz na końcu pliku:

```cmake
add_executable(support_your_first_test support_your_first_test.cpp)
target_link_libraries(support_your_first_test PRIVATE psm_core)
add_test(NAME support_your_first_test COMMAND support_your_first_test)
```

To jest **cały** wzorzec — te same trzy linie, zmieniona tylko nazwa, dla każdego testu w tym pliku od
`zone_test` po `engine_routing_deadline_test`. Nie musisz rozumieć CMake głębiej niż to, żeby go
skopiować i podmienić nazwę — to dostarczona przez kurs mechanika, nie nowy cel nauki.

## 4. Uruchomienie przez CTest

```bash
cmake --preset dev
cmake --build --preset dev
ctest --test-dir build/dev -R support_your_first_test --output-on-failure
```

Jeśli test się kompiluje i przechodzi — wszystkie cztery `psmCheck` są spełnione, `main()` dochodzi do
`return 0;`.

## 5. Porównaj swój test z istniejącym testem kursowym

Postaw obok siebie swój plik i `controller_test.cpp`. Różnica nie powinna być w stylu (oba to seria
`psmCheck`) — powinna być w **tym, co jest testowane**: `controller_test.cpp` nie dotyka `status` w
ogóle, bo `classify` go nie przyjmuje. Twój test istnieje właśnie dlatego, że `decideClassification` ma
dodatkową ścieżkę, której `classify` nie ma. Jeśli twój test wygląda identycznie jak
`controller_test.cpp` z podmienioną nazwą funkcji — prawdopodobnie nie sprawdziłeś/aś ścieżki
`Missing`/`Stale` wcale.

## 6. „Test przechodzi” vs „test faktycznie coś wykrywa”

Zielony test niczego nie dowodzi sam z siebie — dowodzi dopiero razem z dowodem, że **umiałby też
zaczerwienić się**, gdyby kod był zepsuty. Sprawdź to teraz, celowo:

1. Tymczasowo zepsuj `decideClassification` w `src/controller.cpp` — np. usuń warunek `status` i
   zawsze wywołuj `classify(weight.grams)`, niezależnie od statusu.
2. Przebuduj i uruchom ponownie swój test.
3. Jeśli twój test **nadal jest zielony** mimo zepsutego kodu — nie testował ścieżki `status`, tylko
   przypadkiem nie zauważył jej braku. Wróć do punktu 2 i dodaj przypadek, który faktycznie to
   wykrywa.
4. Cofnij zepsucie (`git checkout -- src/controller.cpp` albo ręcznie), upewnij się, że test znowu
   przechodzi na poprawnym kodzie.

To jest różnica między „test przechodzi” a „test rzeczywiście coś wykrywa” — i dokładnie to samo
pytanie będzie towarzyszyć każdemu testowi, który napiszesz w Final Project (patrz Final Project Brief,
sekcja o własnych testach).

## Co dalej

Minimalne wymaganie Kickoff: **co najmniej 2 nowe testy, z których co najmniej jeden realnie wykrywa
zepsucie** (sprawdzone tak jak w kroku 6). Gdy to masz — przejdź do `01_final_project_brief.md`.
