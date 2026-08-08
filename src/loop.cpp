#include <psm/loop.hpp>

#include <psm/controller.hpp>
#include <psm/diverter.hpp>

namespace psm {

void runTicks(Plant& plant, int tickCount) {
    Diverter diverter;
    for (int i = 0; i < tickCount; ++i) {
        if (plant.item.has_value()) {
            diverter.setCommand(toDiverterCommand(classify(plant.item->mass)));
        }
        diverter.resolve();
        advance(plant, diverter);
    }
}

}  // namespace psm
