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
    // TODO (Misja 32: silnik_z_wieloma_paczkami): to jest tymczasowy, kompilowalny placeholder --
    // NIE ostateczna logika. Zastąp go pełną integracją: odczyty czujników z
    // plant_.presenceCheck/plant_.weighing, updatePresenceConfirmation/updateClassification
    // wywoływane tylko gdy odpowiedni slot jest zajęty, polecenie dywertera wyliczane WYŁĄCZNIE z
    // plant_.diverting (nigdy z tego, co właśnie sklasyfikowano w weighing w tym samym ticku,
    // ponieważ ta paczka nie mogła jeszcze dotrzeć do diverting), wywołanie psm::advance(...) pod
    // bramką beltMotor_.actualState() == BeltMotorState::Running, i TickResult zbudowany z
    // rzeczywistych czterech slotów oraz korelacji presenceObservedItemId/weightObservedItemId/
    // diverterCommandItemId. Zachowaj bez zmian: flagi wejściowe, latch_, decision, modeForTick,
    // bramkowanie pasa przez modeForTick, reactToSystemEvent na końcu.
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

    if (decision.overrideActive) {
        beltMotor_.forceStop();
    } else {
        beltMotor_.setCommand(modeForTick == Mode::Running ? BeltMotorCommand::Run : BeltMotorCommand::Stop);
        beltMotor_.resolve();
    }

    mode_ = reactToSystemEvent(modeForTick, std::nullopt);

    SensorSnapshot sensors{tick_, PresenceReading{ReadingStatus::Ok, false}, std::nullopt,
                            WeightReading{ReadingStatus::Ok, 0}, std::nullopt};
    TickResult result{tick_,
                       plant_.infeed,
                       plant_.presenceCheck,
                       plant_.weighing,
                       plant_.diverting,
                       std::nullopt,
                       DiverterCommand::HoldStraight,
                       std::nullopt,
                       diverter_.actualPosition(),
                       mode_,
                       beltMotor_.actualState(),
                       latch_,
                       sensors,
                       std::nullopt};
    ++tick_;
    return result;
}

}  // namespace psm
