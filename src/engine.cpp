#include <psm/engine.hpp>

#include <cassert>

#include <psm/controller.hpp>
#include <psm/safety_supervisor.hpp>

namespace psm {

Engine::Engine(std::vector<ScenarioInput> script) : script_(std::move(script)) {
    assert(isValidScenario(script_));
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
    mode_ = modeStep(mode_, ModeInputs{latch_, startRequested, stopRequested, resetRequested});

    if (checkEmergencyOverride(latch_).overrideActive) {
        beltMotor_.forceStop();
    }

    plant_.advance(diverter_, beltMotor_);

    if (diverterMayMove(mode_)) {
        if (const auto& item = plant_.currentItem(); item.has_value()) {
            diverter_.setCommand(toDiverterCommand(classify(item->mass)));
        }
        diverter_.resolve();
    }

    beltMotor_.setCommand(filterRoutineBeltCommand(BeltMotorCommand::Run, mode_));
    beltMotor_.resolve();

    TickResult result{tick_, plant_.currentItem(), diverter_.actualPosition(), mode_, beltMotor_.actualState()};
    ++tick_;
    return result;
}

}  // namespace psm
