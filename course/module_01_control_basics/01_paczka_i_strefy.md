🇵🇱 Polski | [🇬🇧 English](01_paczka_i_strefy.en.md)

# 1.1 Paczka i strefy

## Problem

Komórka sortująca składa się z przenośnika podzielonego na kilka odcinków, które nazwiemy
**strefami**. Paczka wjeżdża na przenośnik, przechodzi kolejno przez punkt kontrolny obecności,
wagę, rozjazd (dywerter) i kończy w jednym z dwóch miejsc wyjściowych, zależnie od tego, ile waży.

Zanim cokolwiek ruszymy, potrzebujemy dwóch rzeczy:
- sposobu, żeby **nazwać** te miejsca w kodzie,
- sposobu, żeby **opisać jedną konkretną paczkę**, jej położenie i masę.

## Nowe elementy C++

**`enum class`** opisuje zamknięty zbiór nazwanych wartości. Moglibyśmy strefy zapisać jako zwykłe
liczby (`0`, `1`, `2`...), ale wtedy nic nie chroni nas przed pomyłką w rodzaju „strefa 7”, która nie
istnieje. `enum class` pozwala napisać `Zone::Infeed` zamiast `0`. Kompilator zna wszystkie
dozwolone wartości i nie pozwoli przez pomyłkę wpisać czegoś spoza tego zbioru.

```cpp
enum class Zone { Infeed, PresenceCheck, Weighing, Diverting, OutputLight, OutputHeavy };
```

**`struct`** pozwala zgrupować kilka powiązanych wartości. Paczka ma tożsamość (`id`), bieżące
położenie (`zone`) i wagę (`mass`). Są to trzy różne informacje, ale wszystkie opisują tę samą
paczkę. `struct` pozwala trzymać je razem, zamiast przekazywać trzy osobne zmienne.

```cpp
struct Item {
    ItemId id;
    Zone zone;
    Grams mass;
};
```

## Co już masz gotowe

Otwórz [`include/psm/zone.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/include/psm/zone.hpp) i
[`include/psm/item.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/include/psm/item.hpp). Oba typy, `enum class Zone` i `struct Item`,
są już w pełni zdefiniowane, tak jak w przykładzie wyżej. Nie zmieniaj ich.

To, czego brakuje, to **zachowanie**: sposób zamiany wartości `Zone` na czytelny dla człowieka
napis. Zobacz [`src/zone.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-01-start/src/zone.cpp):

```cpp
std::string_view toString(Zone zone) {
    // TODO (Misja 1: paczka_i_strefy): każda strefa powinna dać inny napis.
    (void)zone;
    return "TODO";
}
```

## Co masz napisać

Uzupełnij `toString` tak, żeby dla każdej wartości `Zone` zwracał odpowiadający jej napis, czyli
nazwę enumeratora jako tekst: `Zone::Infeed` → `"Infeed"`, `Zone::PresenceCheck` →
`"PresenceCheck"`, i tak dalej dla wszystkich sześciu stref.

Do wyboru między sześcioma wartościami dobrze nadaje się instrukcja `switch`:

```cpp
switch (zone) {
    case Zone::Infeed: return "Infeed";
    // ...
}
```

Usuń linię `(void)zone;`. Była potrzebna tylko po to, żeby kompilator nie zgłaszał nieużywanego
parametru, zanim go faktycznie użyjesz.

## Sprawdź się

```bash
ctest --preset test -L misja-1
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`.

## Częste błędy

- **Brak `return` w którymś `case`:** wykonanie przechodzi do następnego przypadku (tzw.
  fall-through) i zwraca zły napis. Każdy `case` w tej funkcji powinien kończyć się swoim `return`.
- **Literówka w napisie:** test porównuje napis znak w znak (`"PresenceCheck"`, nie
  `"presence_check"` ani `"Presence Check"`).
- **Pominięta wartość `Zone`:** jeśli zapomnisz `case` dla którejś strefy, kompilator prawdopodobnie
  wypisze ostrzeżenie o niewyczerpanym `switch`. Warto je od razu naprawić, nie ignorować.

## Pytanie do zastanowienia

Dlaczego `enum class Zone` jest tu lepszym wyborem niż zwykły `int`, skoro i tak w środku komputera
to tylko liczba?

**Dalej:** [Misja 2: ruch paczki](./02_ruch_paczki.md).
