#include <psm/engine.hpp>

#include <cassert>

#include <psm/controller.hpp>
#include <psm/safety_supervisor.hpp>

namespace psm {

Engine::Engine(std::vector<ScenarioInput> script, std::vector<ScriptedFault> faultScript)
    : script_(std::move(script)), faultScript_(std::move(faultScript)) {
    assert(isValidScenario(script_));
    assert(isValidFaultScript(faultScript_));
}

void Engine::spawnItem(Item item) {
    plant_.spawnItem(item);
}

TickResult Engine::step() {
    bool pressed = false;
    bool released = false;
    bool resetRequested = false;
    bool startRequested = false;
    bool stopRequested = false;

    for (const auto& input : script_) {
        if (input.at != tick_) {
            continue;
        }
        switch (input.kind) {
            case ScenarioInputKind::EmergencyStopPressed:
                pressed = true;
                break;
            case ScenarioInputKind::EmergencyStopReleased:
                released = true;
                break;
            case ScenarioInputKind::Reset:
                resetRequested = true;
                break;
            case ScenarioInputKind::StartRequested:
                startRequested = true;
                break;
            case ScenarioInputKind::StopRequested:
                stopRequested = true;
                break;
        }
    }

    latch_ = nextEStopLatchState(latch_, pressed, released, resetRequested);

    if (checkEmergencyOverride(latch_).overrideActive) {
        beltMotor_.forceStop();
    }

    const auto diverterFault = activeFault(FaultTarget::Diverter, tick_, faultScript_);
    const auto event = plant_.advance(diverter_, beltMotor_);

    mode_ = modeStep(mode_, ModeInputs{latch_, startRequested, stopRequested, resetRequested,
                                        event == SystemEventKind::RoutingDeadlineMissed});

    const auto presenceFault = activeFault(FaultTarget::PresenceSensor, tick_, faultScript_);
    const auto weightFault = activeFault(FaultTarget::WeightSensor, tick_, faultScript_);
    const PresenceReading presence = presenceSensor_.read(plant_.currentItem(), presenceFault);
    const WeightReading weight = weightSensor_.read(plant_.currentItem(), weightFault);

    if (diverterMayMove(mode_)) {
        if (auto weightClass = decideClassification(presence, weight)) {
            diverter_.setCommand(toDiverterCommand(*weightClass));
        }
        diverter_.resolve(diverterFault);
    }

    beltMotor_.setCommand(filterRoutineBeltCommand(BeltMotorCommand::Run, mode_));
    beltMotor_.resolve();

    SensorSnapshot sensors{tick_, presence, weight};
    TickResult result{tick_, plant_.currentItem(), diverter_.actualPosition(), mode_,
                       beltMotor_.actualState(), sensors, event};
    ++tick_;
    return result;
}

}  // namespace psm
