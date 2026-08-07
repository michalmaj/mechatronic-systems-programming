#include <psm/engine.hpp>

#include <psm/controller.hpp>

namespace psm {

void Engine::spawnItem(Item item) {
    plant_.spawnItem(item);
}

TickResult Engine::step() {
    if (const auto& item = plant_.currentItem(); item.has_value()) {
        diverter_.setCommand(toDiverterCommand(classify(item->mass)));
    }

    plant_.advance(diverter_);
    diverter_.resolve();

    TickResult result{tick_, plant_.currentItem(), diverter_.actualPosition()};
    ++tick_;
    return result;
}

}  // namespace psm
