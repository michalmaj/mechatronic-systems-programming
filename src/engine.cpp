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

void Engine::injectSensorFault(SensorTarget target, SensorFaultKind kind) {
    switch (target) {
        case SensorTarget::Presence:
            presenceFault_ = kind;
            return;
        case SensorTarget::Weight:
            weightFault_ = kind;
            return;
    }
}

void Engine::clearSensorFault(SensorTarget target) {
    switch (target) {
        case SensorTarget::Presence:
            presenceFault_ = std::nullopt;
            return;
        case SensorTarget::Weight:
            weightFault_ = std::nullopt;
            return;
    }
}

void Engine::injectDiverterFault(DiverterFaultKind kind) {
    diverterFault_ = kind;
}

void Engine::clearDiverterFault() {
    diverterFault_ = std::nullopt;
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
    const Mode modeForTick = modeStep(mode_, startRequested, stopRequested, latch_, resetRequested);

    const PresenceReading presence = presenceSensor_.read(plant_.item, presenceFault_);
    const WeightReading weight = weightSensor_.read(plant_.item, weightFault_);
    updateControllerState(controllerState_, plant_.item, presence, weight);

    const bool routingReady = !decision.overrideActive && diverterMayMove(modeForTick)
                               && controllerState_.classification.has_value();

    DiverterCommand diverterCommand = DiverterCommand::HoldStraight;
    if (!decision.overrideActive && diverterMayMove(modeForTick)) {
        if (controllerState_.classification.has_value()) {
            diverterCommand = toDiverterCommand(*controllerState_.classification);
        }
        diverter_.setCommand(diverterCommand);
        diverter_.resolve(diverterFault_);
    }

    if (decision.overrideActive) {
        beltMotor_.forceStop();
    } else {
        beltMotor_.setCommand(modeForTick == Mode::Running ? BeltMotorCommand::Run : BeltMotorCommand::Stop);
        beltMotor_.resolve();
    }

    std::optional<SystemEventKind> event;
    if (beltMotor_.actualState() == BeltMotorState::Running) {
        event = psm::advance(plant_, diverter_, routingReady);
    }

    mode_ = reactToSystemEvent(modeForTick, event);

    SensorSnapshot sensors{tick_, presence, weight};
    TickResult result{tick_,  plant_.item,          diverterCommand,          diverter_.actualPosition(),
                       mode_, beltMotor_.actualState(), latch_, sensors, event};
    ++tick_;
    return result;
}

}  // namespace psm
