# 7.1 Zablokowany dywerter

## Problem

Moduł 6 nauczył czujniki uczciwie przyznawać się do usterki, zamiast udawać, że wszystko działa.
Aktuator zasługuje na to samo: fizycznie zablokowany dywerter **nie może** się ruszyć, i
`Diverter::resolve()` nie powinien udawać inaczej.

## Nowy element C++

```cpp
enum class DiverterFaultKind { Blocked };
```

Zwróć uwagę: to **osobny** typ od `SensorFaultKind` (`Missing`/`Stale` — to, co do niedawna nazywało
się po prostu `FaultKind`, zanim ten moduł rozdzielił nazwę na `SensorFaultKind`, żeby jej zakres był
jawny w samej nazwie). Gdyby dywerter dzielił jeden wspólny typ usterki z czujnikami, nic nie
broniłoby przed wywołaniem w rodzaju "zablokowany czujnik obecności" albo "brakujący dywerter" —
kombinacji bez żadnego sensu, które i tak by się skompilowały. Osobny typ sprawia, że taka pomyłka po
prostu nie przechodzi przez kompilator.

```cpp
void resolve(std::optional<DiverterFaultKind> fault = std::nullopt);
```

## Dokładna reguła

Gdy `fault == DiverterFaultKind::Blocked`, `resolve()` jest **całkowitym no-opem** — `actual_` nie
zmienia się wcale, nawet w stronę `Moving`. W przeciwnym razie zachowanie jest identyczne jak
dotychczas — cała logika trzech stanów z Modułu 2 zostaje bez zmian, tylko poprzedzona tym jednym
sprawdzeniem.

`isSettled()` **nie wymaga żadnej zmiany**. Już teraz porównuje `actual_` z celem *aktualnego
polecenia* — a to jest dokładnie to, czego potrzeba, żeby poprawnie zgłosić "nieustawiony" dla
dywertera zamrożonego w połowie przejścia przez blokadę.

## Co już masz gotowe

[`include/psm/diverter_fault_kind.hpp`](../../include/psm/diverter_fault_kind.hpp) — typ gotowy.

[`include/psm/diverter.hpp`](../../include/psm/diverter.hpp) — sygnatura `resolve` już
zaktualizowana.

[`src/diverter.cpp`](../../src/diverter.cpp) — cała dotychczasowa logika trzech stanów obecna i
niezmieniona; brakuje tylko sprawdzenia `Blocked` na samym początku (`// TODO`).

## Co masz napisać

Dodaj sprawdzenie `fault == DiverterFaultKind::Blocked` na początku `resolve()`, zanim cokolwiek
innego się wykona.

## Self-check

```bash
ctest --preset test -L misja-25
```

Oczekiwany wynik: `100% tests passed, 0 tests failed out of 1`. Test sprawdza no-op dla `Blocked`
(łącznie z zamrożeniem dokładnie w stanie `Moving`, jeśli usterka pojawia się w połowie przejścia),
i że zwykłe, nieusterkowe działanie pozostaje dokładnie takie jak w Module 2.

## Częste błędy

- **Sprawdzanie `fault.has_value()` zamiast porównania z `DiverterFaultKind::Blocked`** — na razie to
  ten sam efekt (jest tylko jedna wartość), ale porównanie z konkretną wartością jest jaśniejsze i
  odporne na przyszłe rozszerzenie typu.
- **Umieszczenie sprawdzenia `Blocked` po istniejącej logice**, zamiast przed nią — wtedy dywerter
  zdążyłby się poruszyć o jeden krok, zanim blokada go zatrzyma.

## Pytanie do zastanowienia

`SensorFaultKind` ma dwie wartości (`Missing`, `Stale`), `DiverterFaultKind` na razie tylko jedną
(`Blocked`). Dlaczego mimo to warto zdefiniować go jako osobny `enum`, a nie np. jako `bool
isBlocked`?

**Dalej:** [Misja 26: termin rutowania](./02_termin_rutowania.md).
