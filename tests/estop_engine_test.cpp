#include "support/check.hpp"
#include <psm/engine.hpp>

int main() {
    psm::Engine engine;
    engine.spawnItem(1, 750);
    engine.requestStart();

    engine.step();
    auto result = engine.step();
    psmCheck(result.mode == psm::Mode::Running, "system is running before the emergency stop");
    psmCheck(result.beltActual == psm::BeltMotorState::Running, "belt is Running before the emergency stop");
    psmCheck(result.presenceCheck.has_value(), "item has started moving");

    engine.requestEStop();
    result = engine.step();
    psmCheck(result.mode == psm::Mode::EStopped, "emergency stop takes effect the very next step");
    psmCheck(result.latch == psm::EStopLatchState::Engaged, "latch reflects the button still being pressed");
    psmCheck(result.beltActual == psm::BeltMotorState::Stopped,
             "belt is forced to Stopped immediately -- no RampingDown, no competing routine call this tick");
    psmCheck(result.presenceCheck.has_value(), "item is frozen exactly where it was");

    engine.releaseEStop();
    result = engine.step();
    psmCheck(result.mode == psm::Mode::EStopped, "mode stays EStopped -- releasing the button alone is not enough");
    psmCheck(result.latch == psm::EStopLatchState::Armed, "latch becomes Armed: released, but not yet reset");
    psmCheck(result.beltActual == psm::BeltMotorState::Stopped, "belt stays Stopped");
    psmCheck(result.presenceCheck.has_value(), "item is still frozen");

    engine.requestReset();
    engine.requestStart();
    result = engine.step();
    psmCheck(result.latch == psm::EStopLatchState::Released, "latch finally clears");
    psmCheck(result.mode == psm::Mode::Idle,
             "same-tick reset+start still lands in Idle, never Running -- recovery always goes through Idle first");
    psmCheck(result.presenceCheck.has_value(), "item still has not moved");

    engine.requestStart();
    result = engine.step();
    psmCheck(result.mode == psm::Mode::Running, "a separate, later requestStart() actually resumes running");

    return 0;
}
