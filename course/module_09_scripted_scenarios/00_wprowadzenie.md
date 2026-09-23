# Moduł 9: scenariusze skryptowane

## Gdzie jesteśmy

Od Modułu 1 każdy ślad `TickResult` powstawał z ręcznie splecionych wywołań `request*()`/`inject*()`/
`clear*()`/`spawnItem()`, wplecionych między kolejne `step()`. Ten moduł dodaje deklaratywną
alternatywę: wartość `Scenario`, opisującą cały eksperyment z góry (wejścia operatora, przybycia
paczek, okna usterek czujników/dywertera, czas trwania), oraz `runScenario()`, która odtwarza go na
świeżym `Engine` i zwraca pełny ślad.

## Co się zmienia -- i co się NIE zmienia

`Engine` pozostaje **całkowicie niezmieniony** -- żadnego nowego konstruktora, żadnych nowych pól.
Skryptowanie to warstwa NAD publicznym API `Engine`, która woła dokładnie te same metody, których
każdy dotychczasowy test i CLI już używa. To świadomie inny wybór niż w referencyjnym symulatorze,
gdzie `Engine` w ogóle nie ma `request*()`/`inject*()`/`clear*()` -- tam skrypt *zastępuje*
imperatywne API. Tutaj skrypt jest opcjonalną nakładką, nie zamiennikiem.

`Scenario` to jedna nazwana wartość: `operatorInputs`, `arrivals`, `sensorFaults`, `diverterFaults`,
`duration`. `isValidScenario` sprawdza ją statycznie, przed uruchomieniem czegokolwiek. `runScenario`
buduje świeży `Engine` i odtwarza scenariusz tick po ticku, zwracając `std::optional<std::vector<TickResult>>`
-- `std::nullopt` dla scenariusza, który jest nieprawidłowy albo którego nie da się dokończyć.

## Trzy misje

- **Misja 33 — model i walidacja.** Kształt `Scenario` i jego pięciu składowych typów;
  `isValidScenario`.
- **Misja 34 — odtwarzacz.** `runScenario`: świeży `Engine`, ustalona kolejność ticka, kontrakt
  odtwarzalności.
- **Misja 35 — integracja w CLI.** Dwie demonstracje w `apps/simulator_cli`: odtworzenie mechanizmu i
  osi czasu ze śladu odzyskiwania z Modułu 7, oraz nowy scenariusz z zaplanowanymi przybyciami wielu
  paczek.

## Zanim zaczniesz

Ten moduł niczego istniejącego nie modyfikuje na poziomie domeny -- `Item`, `Plant`, `Engine`,
`TickResult` zostają dokładnie takie, jakie są. `apps/simulator_cli/main.cpp` jest jednak w tym module
całkiem nowy (nie kontynuacja CLI z Modułu 8) i **nie jest** żadną z Twoich misji -- to gotowy,
mechaniczny harness, który buduje dwa scenariusze, uruchamia je i sam sprawdza, czy wyszło coś
sensownego. Twoja praca to `isValidScenario`, `runScenario`, i zawartość dwóch funkcji budujących
konkretne demonstracyjne scenariusze.

**Dalej:** [Misja 33: model i walidacja](./01_model_i_walidacja.md).
