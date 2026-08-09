#include <psm/engine.hpp>

#include <optional>

#include <psm/controller.hpp>

namespace psm {

void Engine::spawnItem(Item item) {
    psm::spawnItem(plant_, item);
}

TickResult Engine::step() {
    DiverterCommand diverterCommand = DiverterCommand::HoldStraight;
    if (plant_.item.has_value()) {
        diverterCommand = toDiverterCommand(classify(plant_.item->mass));
    }
    diverter_.setCommand(diverterCommand);
    diverter_.resolve();
    psm::advance(plant_, diverter_);

    TickResult result{tick_, plant_.item, diverterCommand, diverter_.actualPosition()};
    ++tick_;
    return result;
}

}  // namespace psm
