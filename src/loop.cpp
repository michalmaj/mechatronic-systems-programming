#include <psm/loop.hpp>

#include <psm/controller.hpp>

namespace psm {

void runTicks(Plant& plant, int tickCount) {
    for (int i = 0; i < tickCount; ++i) {
        DiverterPosition diverterPosition = DiverterPosition::Straight;
        if (plant.item.has_value()) {
            diverterPosition = toDiverterPosition(classify(plant.item->mass));
        }
        advance(plant, diverterPosition);
    }
}

}  // namespace psm
