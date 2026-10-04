🇵🇱 Polski | [🇬🇧 English](00_wprowadzenie.en.md)

# Moduł 8: wiele paczek jednocześnie

## Gdzie jesteśmy

Od modułu 1 `Plant` przechowywał jedną paczkę w polu `std::optional<Item> item`. Na prawdziwej linii
sortującej kilka paczek może znajdować się jednocześnie w różnych strefach. W tym module dostosujesz
do tego nasz model.

## Co się zmienia

Z `Item` znika pole `zone`. `Plant` będzie mieć osobne pole dla każdej strefy, więc położenie paczki
wynika bezpośrednio z pola, w którym została zapisana. Dodatkowy znacznik powielałby tę informację i
mógłby wskazywać inną strefę niż rzeczywiste położenie paczki.

Każdy `Item` otrzymuje za to własny stan przetwarzania: `presenceConfirmed`, `classification` i
`divertingWaitTicks`. Dotychczas dane te znajdowały się we wspólnym `ControllerState`. Przy kilku
paczkach odczyty czujników wykonane w jednym ticku mogą jednak dotyczyć różnych `ItemId`, dlatego
stan musi być przechowywany razem z właściwą paczką.

`Plant` otrzymuje cztery pola: `infeed`, `presenceCheck`, `weighing` i `diverting`. Każde może
przechowywać najwyżej jedną paczkę. Paczka nie może więc wejść do zajętej strefy, a przy dywerterze
nigdy nie znajdą się dwie paczki naraz.

Funkcja `advance()` będzie obsługiwać cztery przejścia w stałej kolejności, od wyjścia w stronę
wejścia: `Diverting` → `Weighing` → `PresenceCheck` → `Infeed`. Dzięki temu kilka paczek może
przesunąć się w jednym ticku, również do stref zwolnionych chwilę wcześniej. Żadna pojedyncza paczka
nie przesunie się przy tym więcej niż raz.

Strefy `OutputLight` i `OutputHeavy` nie będą już polami `Plant`. Paczka opuszczająca linię zniknie z
`Plant`, a `advance()` zwróci informację o jej odjeździe w `ItemDeparture`.

## Cztery misje

- **Misja 29: model wielu paczek.** Poznasz nową strukturę `Item` i `Plant` oraz uzupełnisz
  `spawnItem(id, mass)`.
- **Misja 30: przesuwanie paczek.** Zaimplementujesz pełny algorytm `advance()`, który obsługuje
  strefy od wyjścia do wejścia.
- **Misja 31: powiązanie odczytów z paczkami.** Zastąpisz `ControllerState` dwiema funkcjami, które
  zapisują wyniki pomiarów w odpowiednich paczkach.
- **Misja 32: `Engine` z wieloma paczkami.** Połączysz wszystkie elementy w `Engine::step()`,
  rozszerzysz `TickResult` i `describe()` oraz przygotujesz demonstrację w programie terminalowym.

## Zanim zaczniesz

Tego modułu nie da się budować przyrostowo w taki sam sposób jak poprzednich. `Engine::step()` z
modułu 7 korzysta z pól `Plant::item` i `Item::zone`, które zostały usunięte. Kod startowy się
kompiluje, ale do misji 32 metoda `Engine::step()` nie przesuwa paczek. Jest to zamierzone zachowanie,
a nie błąd przygotowanego projektu.

**Dalej:** [Misja 29: model wielu paczek](./01_partie_i_paczki.md).
