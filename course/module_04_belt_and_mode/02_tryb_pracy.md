🇵🇱 Polski | [🇬🇧 English](02_tryb_pracy.en.md)

# 4.2 Tryb pracy

## Problem

Do tej pory opisywaliśmy pojedynczą paczkę: jej położenie, masę i wybraną trasę. Nie zapisywaliśmy
jednak stanu **całego systemu**, czyli informacji o tym, czy ma pracować. Potrzebujemy wartości,
która go opisuje, oraz funkcji służącej do jego zmiany.

## Nowe elementy C++

**`enum class Mode { Idle, Running }`** ma na razie dwie wartości: przenośnik stoi albo pracuje.

**Wolna funkcja `Mode modeStep(Mode current, bool startRequested, bool stopRequested)`** przyjmuje
bieżący tryb i dwa żądania, a następnie zwraca nowy tryb. Działa podobnie do `classify` i
`toDiverterCommand`: nie przechowuje własnego stanu, tylko wyznacza wynik na podstawie argumentów.

## Dlaczego wystarcza typ wyliczeniowy

W tym module nie potrzebujemy osobnej klasy ze stanem i metodami. Nie ma tu stanu wymagającego
ochrony. `Engine` przechowuje wartość `Mode` i przekazuje ją do `modeStep`, podobnie jak przechowuje
licznik ticków. Inaczej jest w przypadku `BeltMotor`, którego niezmiennik wymaga, aby wartość
`actual_` zmieniała się tylko przez `resolve()`, krok po kroku. Jeśli w kolejnych modułach pojawi się
powód do ukrycia stanu trybu, wtedy będzie można wrócić do tej decyzji. Nie warto tworzyć klasy „na
wszelki wypadek”.

## Reguła konfliktu

Co się dzieje, gdy `startRequested` i `stopRequested` są prawdziwe **w tym samym wywołaniu**? Reguła
jest jednoznaczna: **`stopRequested` wygrywa**.

- Jeśli `stopRequested` ma wartość `true`, wynikiem jest `Idle`, niezależnie od `startRequested`.
- W przeciwnym razie, jeśli `startRequested` ma wartość `true`, wynikiem jest `Running`.
- Jeśli żadne żądanie nie jest aktywne, tryb się nie zmienia.

## Co już masz gotowe

[`include/psm/mode.hpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-04-start/include/psm/mode.hpp)
zawiera gotowy `enum class Mode` oraz deklarację `modeStep`.

W pliku
[`src/mode.cpp`](https://github.com/michalmaj/mechatronic-systems-programming/blob/module-04-start/src/mode.cpp)
znajdziesz pusty szkielet funkcji z komentarzem `// TODO`.

## Co masz napisać

Uzupełnij ciało `modeStep` zgodnie z regułą konfliktu powyżej, zachowując kolejność
sprawdzeń (najpierw `stopRequested`, potem `startRequested`, potem bez zmian).

## Sprawdź się

```bash
ctest --preset test -L misja-14
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza wszystkie cztery
kombinacje `startRequested`/`stopRequested`, w tym oba jednocześnie prawdziwe.

## Częste błędy

- **Sprawdzenie `startRequested` przed `stopRequested`:** odwraca regułę konfliktu i
  da złą odpowiedź, gdy oba żądania są prawdziwe naraz.
- **Zwrócenie zawsze tej samej wartości zamiast `current`, gdy nie ma żadnego żądania:** funkcja
  musi wtedy zwrócić dotychczasowy tryb.

## Pytanie do zastanowienia

Zarówno stan `BeltMotor` z misji 13, jak i wartość `Mode` zmieniają się w czasie. Dlaczego w
pierwszym przypadku potrzebujemy klasy z ukrytym stanem, a w drugim wystarczają typ wyliczeniowy i
wolna funkcja? Co niepotrzebnie skomplikowałaby dodatkowa klasa dla `Mode`?

**Dalej:** [Misja 15: przenośnik pod kontrolą trybu](./03_przenosnik_pod_kontrola_trybu.md).
