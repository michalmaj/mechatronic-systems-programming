# 1.1 Paczka i strefy

## Problem

Nasza stacja sortująca to fizycznie jeden przenośnik podzielony na kilka odcinków — nazwijmy je
**strefami**. Paczka wjeżdża na początku, przechodzi kolejno przez punkt kontrolny obecności,
wagę, rozjazd (dywerter) i kończy w jednym z dwóch miejsc wyjściowych, zależnie od tego, ile waży.

Zanim cokolwiek ruszymy, potrzebujemy dwóch rzeczy:
- sposobu, żeby **nazwać** te miejsca w kodzie,
- sposobu, żeby **opisać jedną konkretną paczkę** — gdzie akurat jest i ile waży.

## Nowe elementy C++

**`enum class`** — zamknięty zbiór nazwanych wartości. Moglibyśmy strefy zapisać jako zwykłe liczby
(`0`, `1`, `2`...), ale wtedy nic nie chroni nas przed literówką w rodzaju "strefa 7", która nie
istnieje. `enum class` pozwala napisać `Zone::Infeed` zamiast `0` — kompilator zna dokładnie, jakie
wartości są dozwolone, i nie da się przez pomyłkę wpisać czegoś spoza tego zbioru.

```cpp
enum class Zone { Infeed, PresenceCheck, Weighing, Diverting, OutputLight, OutputHeavy };
```

**`struct`** — sposób na zgrupowanie kilku powiązanych ze sobą wartości w jedną całość. Paczka ma
tożsamość (`id`), aktualne położenie (`zone`) i wagę (`mass`) — trzy różne rzeczy, ale wszystkie
opisują tę samą paczkę. `struct` pozwala trzymać je razem, zamiast żonglować trzema osobnymi
zmiennymi.

```cpp
struct Item {
    ItemId id;
    Zone zone;
    Grams mass;
};
```

## Co już masz gotowe

Otwórz [`include/psm/zone.hpp`](../../include/psm/zone.hpp) i
[`include/psm/item.hpp`](../../include/psm/item.hpp). Oba typy — `enum class Zone` i `struct Item`
— są już w pełni zdefiniowane, dokładnie tak, jak wyżej. Nie musisz (i nie powinieneś) ich zmieniać.

To, czego brakuje, to **zachowanie**: sposób zamiany wartości `Zone` na czytelny dla człowieka
napis. Zobacz [`src/zone.cpp`](../../src/zone.cpp):

```cpp
std::string_view toString(Zone zone) {
    // TODO (Misja 1: paczka_i_strefy): każda strefa powinna dać inny napis.
    (void)zone;
    return "TODO";
}
```

## Co masz napisać

Uzupełnij `toString` tak, żeby dla każdej wartości `Zone` zwracał odpowiadający jej napis — dokładnie
nazwę enumeratora jako tekst: `Zone::Infeed` → `"Infeed"`, `Zone::PresenceCheck` →
`"PresenceCheck"`, i tak dalej dla wszystkich sześciu stref.

Do wyboru między sześcioma wartościami dobrze nadaje się instrukcja `switch`:

```cpp
switch (zone) {
    case Zone::Infeed: return "Infeed";
    // ...
}
```

Usuń linijkę `(void)zone;` — była tam tylko po to, żeby kompilator nie narzekał na nieużywany
parametr, zanim go faktycznie użyjesz.

## Self-check

```bash
ctest --preset test -L misja-1
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`.

## Częste błędy

- **Brak `return` w którymś `case`** — wykonanie "przechodzi" do następnego przypadku (tzw.
  fall-through) i zwraca zły napis. Każdy `case` w tej funkcji powinien kończyć się swoim `return`.
- **Literówka w napisie** — test porównuje dokładnie napis znak w znak (`"PresenceCheck"`, nie
  `"presence_check"` ani `"Presence Check"`).
- **Pominięta wartość `Zone`** — jeśli zapomnisz `case` dla którejś strefy, kompilator prawdopodobnie
  wypisze ostrzeżenie o niewyczerpanym `switch`. Warto je od razu naprawić, nie ignorować.

## Pytanie do zastanowienia

Dlaczego `enum class Zone` jest tu lepszym wyborem niż zwykły `int`, skoro i tak w środku komputera
to tylko liczba?

**Dalej:** [Misja 2: ruch paczki](./02_ruch_paczki.md).
