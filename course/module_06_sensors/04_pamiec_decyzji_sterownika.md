# 6.4 Pamięć decyzji sterownika

To najważniejsza i najtrudniejsza misja tego modułu. Warto przeczytać ją uważnie od początku do
końca, zanim zaczniesz pisać kod.

## Problem

`PresenceCheck` i `Weighing` to dwie różne, **sekwencyjne** strefy. Paczka nigdy nie jest w obu
naraz — najpierw mija `PresenceCheck`, dopiero potem, kilka ticków później, dociera do `Weighing`.
To oznacza coś ważnego: **żaden pojedynczy tick nie może jednocześnie potwierdzić obecności i
zważyć paczki.** Odczyt z jednego ticku nigdy nie wystarczy, żeby ufać klasyfikacji — potrzebujemy
połączyć **dwa potwierdzenia z różnych chwil w czasie**.

## Nowy element C++

```cpp
struct ControllerState {
    bool presenceConfirmed = false;
    std::optional<WeightClass> classification;
};

void updateControllerState(ControllerState& state, const std::optional<Item>& item,
                            PresenceReading presence, WeightReading weight);
```

`ControllerState` to zwykły `struct`, bez metod — ten sam wybór co `Plant` (Moduł 1) czy
`TickResult` (Moduł 3): nic tu nie trzeba chronić enkapsulacją, wywołujący (docelowo `Engine`) po
prostu przechowuje tę wartość i przekazuje ją dalej, tick po ticku.

**Dlaczego dwa pola, nie jedno.** `presenceConfirmed` potwierdza się **wcześniej** (gdy paczka mija
`PresenceCheck`), `classification` **później** (gdy paczka mija `Weighing`) — i `classification`
ma sens ustawiać tylko wtedy, gdy `presenceConfirmed` jest już prawdziwe. To dwa niezależne
potwierdzenia z dwóch niezależnych, osobno mogących się zepsuć czujników, potwierdzające się
nawzajem w czasie — ten sam duch co dwie niezależne ścieżki bezpieczeństwa z Modułu 5, tym razem
zastosowany do **danych**, nie aktuatorów.

## Dokładna reguła aktualizacji

Wywoływana co tick, z aktualną paczką i świeżymi odczytami obu czujników:

1. **Jeśli paczki nie ma, albo jest w `Zone::Infeed`:** zresetuj — `state = ControllerState{}`.
   Nowa paczka zaczyna z czystą kartą; nic z poprzedniej nie może się przenieść. (Nieszkodliwe, jeśli
   paczka czeka w `Infeed` kilka ticków, zanim ruszy pas — reset powtarza się, zawsze do tego samego,
   pustego stanu.)
2. **W przeciwnym razie, jeśli paczka jest w `Zone::PresenceCheck`** i `presence.status == Ok` i
   `presence.occupied`: ustaw `state.presenceConfirmed = true`.
3. **W przeciwnym razie, jeśli paczka jest w `Zone::Weighing`**, `state.presenceConfirmed` jest już
   prawdziwe, i `weight.status == Ok`: ustaw
   `state.classification = decideClassification(weight)`.
4. **W przeciwnym razie:** bez zmian.

Zwróć uwagę na warunek w punkcie 3: `classification` ustawia się **tylko** jeśli `presenceConfirmed`
już jest prawdziwe. Jeśli akurat w ticku, w którym paczka mijała `PresenceCheck`, czujnik obecności
był zepsuty — `presenceConfirmed` nigdy nie zostanie ustawione dla tej paczki, i nawet idealny
odczyt wagi później **nie wyprodukuje klasyfikacji**.

## Co już masz gotowe

[`include/psm/controller_state.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/include/psm/controller_state.hpp) — deklaracje
kompletne, jak wyżej.

[`src/controller_state.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-06-start/src/controller_state.cpp) — pusty szkielet z komentarzem
`// TODO`, opisującym dokładnie te cztery kroki.

## Co masz napisać

Uzupełnij ciało `updateControllerState` zgodnie z dokładną regułą powyżej, w tej kolejności.

## Sprawdź się

```bash
ctest --preset test -L misja-23
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test przeprowadza paczkę przez
`Infeed→PresenceCheck→Weighing` i sprawdza, że `presenceConfirmed`/`classification` ustawiają się we
właściwych momentach — a także dwa przypadki awarii: wiarygodny odczyt wagi bez wcześniejszego
potwierdzenia obecności (klasyfikacja się nie pojawia), i potwierdzona obecność ale zepsuty odczyt
wagi (też się nie pojawia).

## Częste błędy

- **Ustawianie `classification` bez sprawdzenia `presenceConfirmed`** — to dokładnie problem, który
  ta misja rozwiązuje; bez tego warunku klasyfikacja opierałaby się wyłącznie na wadze, ignorując
  całkowicie czujnik obecności.
- **Reset tylko przy `!item.has_value()`, bez `Zone::Infeed`** — jeśli paczka jest już w systemie
  (np. świeżo dodana przez `spawnItem`, wciąż w `Infeed`), stary stan z poprzedniej paczki musi
  zniknąć, zanim ta zacznie się przemieszczać.
- **Kolejność sprawdzeń** — reset musi być sprawdzony jako pierwszy; w przeciwnym razie świeża
  paczka w `Infeed` mogłaby przypadkiem "odziedziczyć" stan poprzedniej.

## Pytanie do zastanowienia

Wyobraź sobie, że usterka czujnika obecności trwa dokładnie jeden tick, akurat wtedy, gdy paczka
mija `PresenceCheck` — a potem czujnik znowu działa poprawnie. Czy ta konkretna paczka kiedykolwiek
zostanie sklasyfikowana, nawet jeśli waga zadziała bez zarzutu? Prześledź regułę krok po kroku, żeby
się upewnić.

**Dalej:** [Misja 24: silnik z czujnikami](./05_silnik_z_czujnikami.md).
