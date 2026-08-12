#include <psm/engine.hpp>

#include <optional>

#include <psm/controller.hpp>
#include <psm/safety_supervisor.hpp>

namespace psm {

bool Engine::spawnItem(ItemId id, Grams mass) {
    return psm::spawnItem(plant_, id, mass);
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

    const PresenceReading presence = presenceSensor_.read(plant_.presenceCheck, presenceFault_);
    if (plant_.presenceCheck.has_value()) {
        updatePresenceConfirmation(*plant_.presenceCheck, presence);
    }
    const WeightReading weight = weightSensor_.read(plant_.weighing, weightFault_);
    if (plant_.weighing.has_value()) {
        updateClassification(*plant_.weighing, weight);
    }

    const bool diverterAllowedToMove = !decision.overrideActive && diverterMayMove(modeForTick);

    DiverterCommand diverterCommand = DiverterCommand::HoldStraight;
    std::optional<ItemId> diverterCommandItemId;
    if (plant_.diverting.has_value() && plant_.diverting->classification.has_value()) {
        diverterCommand = toDiverterCommand(*plant_.diverting->classification);
        diverterCommandItemId = plant_.diverting->id;
    }
    if (diverterAllowedToMove) {
        diverter_.setCommand(diverterCommand);
        diverter_.resolve(diverterFault_);
    }

    const bool routingReady = diverterAllowedToMove && plant_.diverting.has_value()
                               && plant_.diverting->classification.has_value();

    if (decision.overrideActive) {
        beltMotor_.forceStop();
    } else {
        beltMotor_.setCommand(modeForTick == Mode::Running ? BeltMotorCommand::Run : BeltMotorCommand::Stop);
        beltMotor_.resolve();
    }

    AdvanceResult advanceResult;
    if (beltMotor_.actualState() == BeltMotorState::Running) {
        advanceResult = psm::advance(plant_, diverter_, routingReady);
    }

    mode_ = reactToSystemEvent(modeForTick, advanceResult.event);

    const std::optional<ItemId> presenceObservedItemId =
        (presence.status == ReadingStatus::Ok && plant_.presenceCheck.has_value())
            ? std::optional<ItemId>{plant_.presenceCheck->id}
            : std::nullopt;
    const std::optional<ItemId> weightObservedItemId =
        (weight.status == ReadingStatus::Ok && plant_.weighing.has_value())
            ? std::optional<ItemId>{plant_.weighing->id}
            : std::nullopt;

    SensorSnapshot sensors{tick_, presence, presenceObservedItemId, weight, weightObservedItemId};
    TickResult result{tick_,
                       plant_.infeed,
                       plant_.presenceCheck,
                       plant_.weighing,
                       plant_.diverting,
                       advanceResult.departure,
                       diverterCommand,
                       diverterCommandItemId,
                       diverter_.actualPosition(),
                       mode_,
                       beltMotor_.actualState(),
                       latch_,
                       sensors,
                       advanceResult.event};
    ++tick_;
    return result;
}

}  // namespace psm
