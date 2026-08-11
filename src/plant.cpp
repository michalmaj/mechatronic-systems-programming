#include <psm/plant.hpp>

namespace psm {

bool spawnItem(Plant& plant, ItemId id, Grams mass) {
    // TODO (Misja 29: partie_i_paczki): odrzuć (zwróć false), jeśli infeed jest zajęty LUB id
    // koliduje z dowolnym innym aktualnie obecnym parcelem (presenceCheck/weighing/diverting).
    // W przeciwnym razie umieść nowy Item{id, mass} w infeed i zwróć true.
    (void)plant;
    (void)id;
    (void)mass;
    return false;
}

AdvanceResult advance(Plant& plant, const Diverter& diverter, bool routingReady) {
    // TODO (Misja 30: przesuwanie_partii): pełny algorytm downstream-to-upstream -- patrz
    // materiały misji. Ten stub celowo nic nie przesuwa i nigdy nic nie zwraca.
    (void)plant;
    (void)diverter;
    (void)routingReady;
    return AdvanceResult{};
}

}  // namespace psm
