#include <psm/plant.hpp>

namespace psm {

void spawnItem(Plant& plant, Item item) {
    // TODO (Misja 3: stan_przenosnika): jeśli plant jest teraz pusty, umieść w nim item
    // (na strefie Infeed). Jeśli nie jest pusty, nic nie rób.
    (void)plant;
    (void)item;
}

void advance(Plant& plant, DiverterPosition diverterPosition) {
    if (!plant.item.has_value()) {
        return;
    }
    // TODO (Misja 3: stan_przenosnika): jeśli item nie dotarł jeszcze do Diverting, przesuń go
    // (advanceZone). Na razie zignoruj parametr diverterPosition -- wrócimy do niego w Misji 4.
    //
    // TODO (Misja 4: decyzja_sortowania): jeśli item JEST na Diverting, użyj diverterPosition,
    // żeby zdecydować OutputLight albo OutputHeavy. Jeśli item dotarł już do OutputLight lub
    // OutputHeavy, wyczyść plant.item -- paczka wyjeżdża z systemu.
    (void)diverterPosition;
}

}  // namespace psm
