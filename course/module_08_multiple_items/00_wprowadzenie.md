# Moduł 8: wiele paczek naraz

## Gdzie jesteśmy

Od Modułu 1 `Plant` modelował dokładnie jedną paczkę na raz — `std::optional<Item> item`, jedno pole.
Prawdziwa taśma sortująca tak nie działa: kilka paczek jest fizycznie w locie jednocześnie, każda w
innej strefie, każda na innym etapie procesu. Ten moduł to zmienia.

## Co się zmienia

`Item` traci pole `zone` — skoro `Plant` będzie miał osobne pole na każdą strefę, przynależność paczki
do strefy to fakt wynikający z tego, w którym polu ona leży, nie osobny znacznik, który mógłby się
z tym faktem rozjechać. W zamian `Item` zyskuje własny stan przetwarzania: `presenceConfirmed`,
`classification`, `divertingWaitTicks` — to, co dotąd żyło w jednym, globalnym `ControllerState`,
teraz podróżuje razem z konkretną paczką, bo przy kilku paczkach naraz różne odczyty czujników mogą w
tym samym ticku dotyczyć zupełnie różnych `ItemId`.

`Plant` zyskuje cztery nazwane pola: `infeed`, `presenceCheck`, `weighing`, `diverting`. Każde mieści
najwyżej jedną paczkę — to naturalny, fizyczny backpressure: paczka nie może wejść do zajętej strefy.
Dzięki temu dywerter nigdy nie musi arbitrażować między dwiema paczkami naraz — w `diverting` zawsze
jest co najwyżej jedna.

`advance()` przesuwa teraz cztery przejścia zamiast jednego, w ustalonej kolejności downstream-to-
upstream (`Diverting` → `Weighing` → `PresenceCheck` → `Infeed`) — dzięki temu paczka może w tym samym
ticku wejść do strefy dopiero co zwolnionej przez inną paczkę, bez sztucznego opóźnienia, a mimo to
żadna paczka nigdy nie przesuwa się więcej niż raz w jednym ticku.

Wyjście (`OutputLight`/`OutputHeavy`) przestaje być polem `Plant` — staje się zlewem (sink):
paczka, która wyjeżdża, jest zgłaszana jako `ItemDeparture` i znika z `Plant` w tym samym wywołaniu
`advance()`, nigdy po cichu.

## Cztery misje

- **Misja 29 — partie i paczki.** Nowy kształt `Item`/`Plant`, `spawnItem(id, mass)`.
- **Misja 30 — przesuwanie partii.** Pełny algorytm `advance()`, downstream-to-upstream.
- **Misja 31 — korelacja per paczka.** `ControllerState` odchodzi; dwie nowe, jawnie rozdzielone
  funkcje korelacji zajmują jego miejsce.
- **Misja 32 — silnik z wieloma paczkami.** Pełna integracja w `Engine::step()`, rozszerzony
  `TickResult`/`describe()`, demonstracja w CLI.

## Zanim zaczniesz

Ten moduł nie da się zbudować przyrostowo tak, jak poprzednie — `Engine::step()` z Modułu 7 odwołuje
się wprost do `Plant::item` i `Item::zone`, które w ogóle już nie istnieją. Dlatego start tego modułu
wygląda inaczej: kompiluje się od razu, ale `Engine::step()` to świadomie tymczasowy placeholder — nie
przesuwa żadnej paczki, dopóki nie napiszesz Misji 32. To normalne i zamierzone, nie błąd w starcie.

**Dalej:** [Misja 29: partie i paczki](./01_partie_i_paczki.md).
