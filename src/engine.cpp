#include <psm/engine.hpp>

#include <optional>

#include <psm/controller.hpp>

namespace psm {

void Engine::spawnItem(Item item) {
    psm::spawnItem(plant_, item);
}

void Engine::requestStart() {
    startRequested_ = true;
}

void Engine::requestStop() {
    stopRequested_ = true;
}

TickResult Engine::step() {
    const bool startRequested = startRequested_;
    const bool stopRequested = stopRequested_;
    startRequested_ = false;
    stopRequested_ = false;

    mode_ = modeStep(mode_, startRequested, stopRequested);

    beltMotor_.setCommand(mode_ == Mode::Running ? BeltMotorCommand::Run : BeltMotorCommand::Stop);
    beltMotor_.resolve();

    DiverterCommand diverterCommand = DiverterCommand::HoldStraight;
    if (plant_.item.has_value()) {
        diverterCommand = toDiverterCommand(classify(plant_.item->mass));
    }
    diverter_.setCommand(diverterCommand);
    diverter_.resolve();

    if (beltMotor_.actualState() == BeltMotorState::Running) {
        psm::advance(plant_, diverter_);
    }

    TickResult result{tick_, plant_.item, diverterCommand, diverter_.actualPosition(), mode_, beltMotor_.actualState()};
    ++tick_;
    return result;
}

}  // namespace psm
