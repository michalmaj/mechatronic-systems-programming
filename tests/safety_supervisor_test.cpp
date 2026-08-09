#include "support/check.hpp"
#include <psm/belt_motor.hpp>
#include <psm/safety_supervisor.hpp>

int main() {
    using psm::EStopLatchState;
    using psm::Mode;

    psmCheck(!psm::checkEmergencyOverride(EStopLatchState::Released).overrideActive,
             "no override while latch is Released");
    psmCheck(psm::checkEmergencyOverride(EStopLatchState::Engaged).overrideActive,
             "override active while latch is Engaged");
    psmCheck(psm::checkEmergencyOverride(EStopLatchState::Armed).overrideActive,
             "override active while latch is Armed");

    psmCheck(psm::diverterMayMove(Mode::Running), "diverter may move while Running");
    psmCheck(!psm::diverterMayMove(Mode::Idle), "diverter may not move while Idle");
    psmCheck(!psm::diverterMayMove(Mode::EStopped), "diverter may not move while EStopped");

    psm::BeltMotor motor;
    motor.setCommand(psm::BeltMotorCommand::Run);
    motor.resolve();
    motor.resolve();
    psmCheck(motor.actualState() == psm::BeltMotorState::Running, "motor is Running before forceStop");

    motor.forceStop();
    psmCheck(motor.actualState() == psm::BeltMotorState::Stopped,
             "forceStop immediately sets Stopped, bypassing RampingDown");

    motor.resolve();
    psmCheck(motor.actualState() == psm::BeltMotorState::Stopped,
             "command_ was also forced to Stop, so a later resolve() does not start ramping up again");

    return 0;
}
