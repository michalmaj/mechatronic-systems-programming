🇵🇱 Polski | [🇬🇧 English](00_project_kickoff.en.md)

# Przed projektem: pierwszy własny test

## Cel

W modułach 0–9 korzystasz z gotowych testów. Przed projektem końcowym napiszesz pierwszy własny test
dla znanej już funkcji.

To krótkie ćwiczenie przed projektem, a nie moduł 10. Obejmuje jedną funkcję, kilka przypadków
testowych i gotowy fragment CMake do skopiowania. CMake nadal jest dostarczoną częścią infrastruktury,
nie tematem zadania.

## 1. Anatomia istniejącego testu

Otwórz `tests/controller_test.cpp`. To jeden z najkrótszych testów w rdzeniu kursu:

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

Zwróć uwagę na cztery rzeczy:

- Test jest zwykłym plikiem `.cpp` z funkcją `main()`. Nie korzysta z zewnętrznej biblioteki
  testowej ani rozbudowanych makr. `ctest` uruchamia skompilowany plik wykonywalny.
- `psmCheck(warunek, opis)` (`tests/support/check.hpp`) sprawdza warunek; jeśli jest fałszywy, wypisuje
  `opis` na `stderr` i **kończy proces przez `std::exit(1)`**. Pierwsze niespełnione sprawdzenie
  natychmiast przerywa test, więc pozostałe już się nie wykonają.
- `return 0;` na końcu `main()` informuje `ctest` o powodzeniu. Test przechodzi, jeśli program dotrze
  do końca bez wywołania `std::exit(1)`.
- Przypadki obejmują wartości z obu zakresów oraz granicę: `100` i `499` należą do Light, `500`
  sprawdza sam próg, a `999` leży w zakresie Heavy. Test wartości granicznej wykrywa typowy błąd w
  operatorze porównania.

## 2. Zadanie: test dla `decideClassification`

`controller_test.cpp` sprawdza `classify(Grams mass) -> WeightClass`, czyli czystą funkcję, której
jedynym przypadkiem brzegowym jest próg masy. W tym samym pliku `include/psm/controller.hpp` znajduje
się używana od modułu 6 funkcja o bardziej rozbudowanych zasadach działania:

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

W przeciwieństwie do `classify` ta funkcja ma **dwie** ścieżki do sprawdzenia. Dla poprawnego odczytu
korzysta z `classify`. Dla odczytu `Missing` lub `Stale` zawsze zwraca `std::nullopt`, niezależnie od
wartości zapisanej w `grams`.

**Napisz test w pliku `tests/support_your_first_test.cpp`**. Możesz wybrać inną nazwę, ponieważ nie
jest ona narzucona przez kurs. Test powinien obejmować co najmniej te przypadki:

1. `status == Ok`, masa w zakresie Light → wynik to `WeightClass::Light`.
2. `status == Ok`, masa w zakresie Heavy (włącznie z progiem) → wynik to `WeightClass::Heavy`.
3. `status == Missing` → wynik to `std::nullopt`, **niezależnie od wartości `grams`**. Użyj masy,
   która zostałaby sklasyfikowana jako Heavy, aby sprawdzić, czy status ma pierwszeństwo.
4. `status == Stale` → jak wyżej.

Wzoruj strukturę pliku na `controller_test.cpp`: dodaj `#include "support/check.hpp"` i `#include
<psm/controller.hpp>`, następnie serię wywołań `psmCheck(...)` oraz `return 0;`.

## 3. Rejestracja w CMake

`tests/CMakeLists.txt` rejestruje każdy test trzema liniami. Dla nowego pliku
`tests/support_your_first_test.cpp` dopisz na końcu pliku:

```cmake
add_executable(support_your_first_test support_your_first_test.cpp)
target_link_libraries(support_your_first_test PRIVATE psm_core)
add_test(NAME support_your_first_test COMMAND support_your_first_test)
```

Każdy test od `zone_test` po `engine_routing_deadline_test` jest rejestrowany za pomocą tych samych
trzech poleceń. Skopiuj je i zmień nazwę. Na tym etapie nie musisz poznawać pozostałych możliwości
CMake.

`add_executable` i `add_test` rejestrują **jeden plik `.cpp` jako jeden wykonywalny test CTest**. Ten
plik może jednak zawierać kilka niezależnych wywołań `psmCheck`, tak jak `controller_test.cpp`.
W terminologii CTest cały program jest testem, natomiast pojedyncze wywołanie `psmCheck` odpowiada
przypadkowi testowemu. To rozróżnienie będzie potrzebne w sekcji 6.

## 4. Uruchomienie przez CTest

```bash
cmake --preset dev
cmake --build --preset dev
ctest --test-dir build/dev -R support_your_first_test --output-on-failure
```

Jeśli test się kompiluje i przechodzi, wszystkie cztery warunki przekazane do `psmCheck` są
spełnione, a `main()` dochodzi do `return 0;`.

## 5. Porównanie z istniejącym testem kursowym

Porównaj swój plik z `controller_test.cpp`. Konstrukcja obu testów będzie podobna, ale powinny
sprawdzać inne zachowanie. `controller_test.cpp` nie korzysta ze `status`, ponieważ `classify` nie ma
takiego parametru. Nowy test jest potrzebny właśnie ze względu na dodatkową ścieżkę w
`decideClassification`. Jeśli różni się od istniejącego testu tylko nazwą funkcji, zapewne nie
sprawdza jeszcze przypadków `Missing` i `Stale`.

## 6. „Test przechodzi” a „test faktycznie coś wykrywa”

Sam fakt, że test przechodzi, nie wystarcza. Trzeba jeszcze sprawdzić, czy test **wykrywa błąd**.
Zrób to w kontrolowany sposób:

1. Tymczasowo wprowadź błąd do `decideClassification` w `src/controller.cpp`. Możesz na przykład
   usunąć warunek `status` i zawsze wywoływać `classify(weight.grams)`, niezależnie od statusu.
2. Przebuduj i uruchom ponownie swój test.
3. Jeśli test **nadal przechodzi** mimo błędnego kodu, nie sprawdzał ścieżki `status`. Wróć do punktu
   2 i dodaj przypadek, który wykrywa ten błąd.
4. Cofnij zmianę (`git restore src/controller.cpp` albo ręcznie) i upewnij się, że test znowu
   przechodzi na poprawnym kodzie.

Tak samo oceniaj każdy test napisany w projekcie końcowym: powinien nie tylko przechodzić dla
poprawnego kodu, lecz także odrzucać błędną implementację.

## Co dalej

W nowym pliku umieść **co najmniej dwa różne przypadki testowe (`psmCheck`)**. Przynajmniej jeden z
nich musi wykrywać błąd wprowadzony zgodnie z opisem w kroku 6. Nie twórz dwóch osobnych programów
testowych. Wystarczy jeden plik z kilkoma sprawdzeniami, zgodnie z wyjaśnieniem w punkcie 3.

Po wykonaniu ćwiczenia przejdź do `01_final_project_brief.md`.
