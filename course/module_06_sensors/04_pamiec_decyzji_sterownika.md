🇵🇱 Polski | [🇬🇧 English](04_pamiec_decyzji_sterownika.en.md)

# 6.4 Pamięć decyzji sterownika

Ta misja łączy informacje z dwóch poprzednich czujników. Przeczytaj cały opis przed rozpoczęciem
pisania kodu.

## Problem

`PresenceCheck` i `Weighing` to dwie kolejne strefy. Paczka najpierw mija `PresenceCheck`, a dopiero
kilka ticków później dociera do `Weighing`. **W jednym ticku nie da się jednocześnie potwierdzić
obecności paczki i jej zważyć.** Sterownik musi połączyć odczyty wykonane w dwóch różnych chwilach.

## Nowy element C++

```cpp
struct ControllerState {
    bool presenceConfirmed = false;
    std::optional<WeightClass> classification;
};

void updateControllerState(ControllerState& state, const std::optional<Item>& item,
                            PresenceReading presence, WeightReading weight);
```

`ControllerState` to zwykły `struct` bez metod, podobnie jak `Plant` z modułu 1 i `TickResult` z
modułu 3. Stan nie wymaga tu ochrony przez enkapsulację. `Engine` będzie przechowywać tę wartość
między kolejnymi tickami.

**Dlaczego potrzebne są dwa pola.** `presenceConfirmed` jest ustawiane wcześniej, gdy paczka mija
`PresenceCheck`. `classification` może zostać ustawione dopiero później, w strefie `Weighing`, i
tylko wtedy, gdy obecność została już potwierdzona. Każda informacja pochodzi z innego czujnika, a
każdy z tych czujników może ulec osobnej usterce.

## Dokładna reguła aktualizacji

Funkcja jest wywoływana w każdym ticku z bieżącą paczką i nowymi odczytami obu czujników:

1. **Jeśli paczki nie ma albo jest w `Zone::Infeed`:** wykonaj `state = ControllerState{}`. Nowa
   paczka nie może odziedziczyć informacji po poprzedniej. Jeśli czeka w `Infeed` przez kilka
   ticków, stan będzie po prostu ponownie zerowany.
2. **W przeciwnym razie, jeśli paczka jest w `Zone::PresenceCheck`** i `presence.status == Ok` i
   `presence.occupied`: ustaw `state.presenceConfirmed = true`.
3. **W przeciwnym razie, jeśli paczka jest w `Zone::Weighing`**, `state.presenceConfirmed` jest już
   prawdziwe, i `weight.status == Ok`: ustaw
   `state.classification = decideClassification(weight)`.
4. **W przeciwnym razie:** bez zmian.

Zwróć uwagę na warunek w punkcie 3: `classification` ustawia się **tylko** jeśli `presenceConfirmed`
już jest prawdziwe. Jeśli akurat w ticku, w którym paczka mijała `PresenceCheck`, czujnik obecności
był uszkodzony, `presenceConfirmed` nie zostanie ustawione dla tej paczki. Nawet poprawny późniejszy
odczyt wagi **nie spowoduje wtedy zapisania klasyfikacji**.

## Co już masz gotowe

[`include/psm/controller_state.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/include/psm/controller_state.hpp)
zawiera kompletne deklaracje pokazane wyżej.

W pliku
[`src/controller_state.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/src/controller_state.cpp)
znajdziesz pusty szkielet funkcji z komentarzem `// TODO`, który opisuje te cztery kroki.

## Co masz napisać

Uzupełnij ciało `updateControllerState` zgodnie z dokładną regułą powyżej, w tej kolejności.

## Sprawdź się

```bash
ctest --preset test -L misja-23
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test przeprowadza paczkę przez
`Infeed` → `PresenceCheck` → `Weighing` i sprawdza, czy `presenceConfirmed` oraz `classification`
ustawiają się we właściwych momentach. Obejmuje też dwa przypadki usterek: poprawny odczyt wagi bez
wcześniejszego potwierdzenia obecności oraz potwierdzoną obecność przy błędnym odczycie wagi. W obu
przypadkach klasyfikacja nie powinna się pojawić.

## Częste błędy

- **Ustawianie `classification` bez sprawdzenia `presenceConfirmed`:** wtedy klasyfikacja
  opierałaby się wyłącznie na wadze i pomijała czujnik obecności.
- **Reset tylko przy `!item.has_value()`, bez `Zone::Infeed`:** jeśli paczka jest już w systemie
  (np. świeżo dodana przez `spawnItem`, wciąż w `Infeed`), stary stan z poprzedniej paczki musi
  zniknąć, zanim ta zacznie się przemieszczać.
- **Nieprawidłowa kolejność sprawdzeń:** reset musi być wykonany jako pierwszy. W przeciwnym razie
  paczka w `Infeed` mogłaby odziedziczyć stan poprzedniej.

## Pytanie do zastanowienia

Wyobraź sobie, że usterka czujnika obecności trwa jeden tick, akurat wtedy, gdy paczka
mija `PresenceCheck`, a potem czujnik znowu działa poprawnie. Czy ta konkretna paczka kiedykolwiek
zostanie sklasyfikowana, nawet jeśli waga zadziała bez zarzutu? Prześledź regułę krok po kroku, żeby
się upewnić.

**Dalej:** [Misja 24: Engine z czujnikami](./05_silnik_z_czujnikami.md).
