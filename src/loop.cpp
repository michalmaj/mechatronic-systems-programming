#include <psm/loop.hpp>

#include <psm/controller.hpp>

namespace psm {

void runTicks(Plant& plant, int tickCount) {
    // TODO (Misja 5: petla_sterowania): powtórz tickCount razy: 1) jeśli plant.item ma wartość,
    // policz WeightClass przez classify(plant.item->mass), a potem DiverterPosition przez
    // toDiverterPosition(...); 2) wywołaj advance(plant, ta pozycja).
    (void)plant;
    (void)tickCount;
}

}  // namespace psm
