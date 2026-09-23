# 9.2 Odtwarzacz

## Problem

Zwalidowany `Scenario` to tylko dane. Coś musi zamienić go w ślad `TickResult`, prowadząc prawdziwy
`Engine`, we właściwej kolejności, na każdym ticku.

## Nowy element C++

```cpp
std::optional<std::vector<TickResult>> runScenario(const Scenario& scenario);
```

Buduje **świeży `Engine` wewnątrz** -- nie przyjmuje `Engine&`. To właśnie czyni `Scenario` naprawdę
powtarzalnym eksperymentem: ta sama wartość `Scenario` zawsze zaczyna od tego samego, czystego stanu.

`activeSensorFault`/`activeDiverterFault` -- funkcje sprawdzające, czy dana usterka jest aktywna na
danym ticku -- są prywatnymi szczegółami implementacji `runScenario`, żyjącymi w anonimowej przestrzeni
nazw w `src/scenario.cpp`. Nie są deklarowane w `scenario.hpp`: upublicznienie ich wymagałoby
udokumentowania i utrzymania osobnego kontraktu ("działa poprawnie tylko dla już zwalidowanych,
nienakładających się harmonogramów"), którego obecnie nic nie potrzebuje.

## Ustalona kolejność ticka

**arrivals → operator inputs → stan usterek → `Engine::step()` → zbierz wynik.**

Stan usterek jest przeliczany od zera na każdym ticku i aplikowany bezwarunkowo przez `injectSensorFault`/
`clearSensorFault`/`injectDiverterFault`/`clearDiverterFault` -- nie trzeba śledzić "czy było aktywne
poprzednio", bo te cztery metody `Engine` są już idempotentne.

## Sygnalizacja błędu -- przez typ zwracany, nie `assert`

`assert()` znika w buildach Release (`NDEBUG`), a nieprawidłowy `Scenario` to prawdziwy, obsługiwalny
na poziomie API warunek, nie błąd programisty do pominięcia w Release. Dwa przypadki, oba
`std::nullopt`:
- `Scenario` jest statycznie nieprawidłowy (`isValidScenario` zwraca `false`) -- sprawdzone raz, przed
  pierwszym tickiem.
- Zaplanowane przybycie nie udaje się w trakcie odtwarzania (`Plant::infeed` wciąż zajęty przez
  wcześniejszą, jeszcze nieodjechaną paczkę). To **nie** jest wykrywalne statycznie w ogólności
  (zależy od stanu pasa/trybu wynikającego z `operatorInputs`) i **nie** jest ponawiane -- to błąd
  scenariusza, zgłaszany tak samo jak nieprawidłowość statyczna.

## Kontrakt odtwarzalności

**Dla dowolnego poprawnego `Scenario`, który da się odtworzyć do końca, dwa osobne wywołania
`runScenario(scenario)` dają sekwencje `TickResult` semantycznie identyczne, pole po polu.** ("Pole po
polu", nie "bajt po bajcie" -- to twierdzenie o wartościach, nie o reprezentacji pamięciowej obiektów
C++.) Wynika to wprost z tego, że `runScenario` buduje świeży `Engine` przy każdym wywołaniu, oraz z
fundamentalnej, projektowej gwarancji determinizmu (brak zegara systemowego, brak losowości, brak
ukrytego stanu globalnego -- prawda od Modułu 0).

## Dlaczego `{EmergencyStopReleased, Reset}` na tym samym ticku jest bezpieczne

Wynika to z zachowania prawdziwego, niezmienionego `nextEStopLatchState` (Moduł 5):
ustawienie `released` i `resetRequested` jednocześnie, startując z `Engaged`, trafia w gałąź
`previous == Engaged` (sprawdzaną jako pierwszą) -- `resetRequested` nie jest w ogóle konsultowane tym
wywołaniem. Zatrzask trafia do `Armed`, nie `Released`. Para jest dozwolona, bo jest bezpieczna i dobrze
zdefiniowana -- nie dlatego, że skraca dwuetapowe odzyskiwanie, które znasz z Modułu 5.

## Co już masz gotowe

[`include/psm/scenario.hpp`](../../include/psm/scenario.hpp) -- sygnatura `runScenario` gotowa.
`isValidScenario` z Misji 33 jest już Twoje.

## Co masz napisać

Uzupełnij ciało `runScenario` w [`src/scenario.cpp`](../../src/scenario.cpp), łącznie z prywatnymi
`activeSensorFault`/`activeDiverterFault`, zgodnie z ustaloną kolejnością powyżej.

## Sprawdź się

```bash
ctest --preset test -L misja-34
```

Oczekiwany wynik: `100% tests passed`. Test buduje tę samą, krótką sekwencję dwa razy -- raz
imperatywnie, raz przez `Scenario`+`runScenario` -- i porównuje wyniki pole po polu, zamiast zgadywać
nowy ślad ręcznie. Sprawdza też: `{EmergencyStopReleased, Reset}` na tym samym ticku; przybycie na
zajęty `Infeed` zwracające `std::nullopt`; statycznie nieprawidłowy scenariusz zwracający `std::nullopt`
bez uruchamiania czegokolwiek; `duration == 0` zwracające pusty, ale obecny ślad; oraz niezmienniczość
względem kolejności elementów w wektorach.

## Częste błędy

- **Śledzenie "czy usterka była aktywna poprzednio"** -- niepotrzebne, `inject*`/`clear*` są
  idempotentne; przelicz stan na nowo na każdym ticku.
- **Automatyczne ponawianie nieudanego przybycia** -- to dokładnie to, czego ta misja świadomie nie
  robi.
- **Upublicznienie `activeSensorFault`/`activeDiverterFault`** -- zostają prywatne w `scenario.cpp`.

## Pytanie do zastanowienia

`runScenario` sprawdza `isValidScenario` raz, na samym początku. Co konkretnie mogłoby pójść źle,
gdyby zamiast tego sprawdzać poszczególne reguły "w locie", w trakcie odtwarzania, zamiast z góry?

**Dalej:** [Misja 35: integracja w CLI](./03_integracja_w_cli.md).
