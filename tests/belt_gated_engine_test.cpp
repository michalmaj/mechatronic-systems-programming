#include "support/check.hpp"
#include <psm/engine.hpp>

int main() {
    psm::Engine engine;
    engine.spawnItem(1, 750);

    auto result = engine.step();
    psmCheck(result.mode == psm::Mode::Idle, "no start requested yet: mode stays Idle");
    psmCheck(result.beltActual == psm::BeltMotorState::Stopped, "belt never commanded to run: stays Stopped");
    psmCheck(result.infeed.has_value() && result.infeed->id == 1, "item does not move while the belt is Stopped");

    engine.requestStart();

    result = engine.step();
    psmCheck(result.mode == psm::Mode::Running, "requestStart takes effect on the very next step");
    psmCheck(result.beltActual == psm::BeltMotorState::RampingUp, "belt begins ramping up on that same step");
    psmCheck(result.infeed.has_value(), "item still waits -- belt is not Running yet");

    result = engine.step();
    psmCheck(result.mode == psm::Mode::Running, "mode stays Running");
    psmCheck(result.beltActual == psm::BeltMotorState::Running, "belt finishes ramping up");
    psmCheck(result.presenceCheck.has_value() && result.presenceCheck->id == 1,
             "belt is now Running: the item advances exactly one step");

    engine.requestStop();

    result = engine.step();
    psmCheck(result.mode == psm::Mode::Idle, "requestStop takes effect on the very next step");
    psmCheck(result.beltActual == psm::BeltMotorState::RampingDown, "belt begins ramping down this same step");
    psmCheck(result.presenceCheck.has_value(), "item does not move -- the gate already fails this tick");

    result = engine.step();
    psmCheck(result.beltActual == psm::BeltMotorState::Stopped, "belt finishes ramping down");
    psmCheck(result.presenceCheck.has_value(), "item stays put once the belt has fully stopped");

    return 0;
}
