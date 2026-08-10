#include <psm/engine.hpp>

#include <optional>

#include <psm/controller.hpp>
#include <psm/safety_supervisor.hpp>

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

void Engine::requestEStop() {
    eStopPressed_ = true;
}

void Engine::releaseEStop() {
    eStopReleased_ = true;
}

void Engine::requestReset() {
    resetRequested_ = true;
}

TickResult Engine::step() {
    const bool startRequested = startRequested_;
    const bool stopRequested = stopRequested_;
    const bool pressed = eStopPressed_;
    const bool released = eStopReleased_;
    const bool resetRequested = resetRequested_;
    startRequested_ = false;
    stopRequested_ = false;
    eStopPressed_ = false;
    eStopReleased_ = false;
    resetRequested_ = false;

    latch_ = nextEStopLatchState(latch_, pressed, released, resetRequested);
    const SafetyDecision decision = checkEmergencyOverride(latch_);
    mode_ = modeStep(mode_, startRequested, stopRequested, latch_);

    if (decision.overrideActive) {
        beltMotor_.forceStop();
    } else {
        beltMotor_.setCommand(mode_ == Mode::Running ? BeltMotorCommand::Run : BeltMotorCommand::Stop);
        beltMotor_.resolve();
    }

    DiverterCommand diverterCommand = DiverterCommand::HoldStraight;
    if (!decision.overrideActive && diverterMayMove(mode_)) {
        if (plant_.item.has_value()) {
            diverterCommand = toDiverterCommand(classify(plant_.item->mass));
        }
        diverter_.setCommand(diverterCommand);
        diverter_.resolve();
    }

    if (beltMotor_.actualState() == BeltMotorState::Running) {
        psm::advance(plant_, diverter_);
    }

    TickResult result{tick_,  plant_.item,          diverterCommand,          diverter_.actualPosition(),
                       mode_, beltMotor_.actualState(), latch_};
    ++tick_;
    return result;
}

}  // namespace psm
