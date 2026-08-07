#include "support/check.hpp"
#include <psm/safety_supervisor.hpp>

int main() {
    using namespace psm;

    psmCheck(!checkEmergencyOverride(EStopLatchState::Released).overrideActive,
             "no override while latch is Released");
    psmCheck(checkEmergencyOverride(EStopLatchState::Engaged).overrideActive,
             "override active while latch is Engaged");
    psmCheck(checkEmergencyOverride(EStopLatchState::Armed).overrideActive,
             "override active while latch is Armed");

    psmCheck(diverterMayMove(Mode::Running), "diverter may move while Running");
    psmCheck(!diverterMayMove(Mode::Idle), "diverter may not move while Idle");
    psmCheck(!diverterMayMove(Mode::EStopped), "diverter may not move while EStopped");
    psmCheck(!diverterMayMove(Mode::Fault), "diverter may not move while Fault");
    psmCheck(!diverterMayMove(Mode::Maintenance), "diverter may not move while Maintenance");

    psmCheck(filterRoutineBeltCommand(BeltMotorCommand::Run, Mode::Running) == BeltMotorCommand::Run,
             "Run passes through while Running");
    psmCheck(filterRoutineBeltCommand(BeltMotorCommand::Run, Mode::Idle) == BeltMotorCommand::Stop,
             "Run is overridden to Stop while not Running");
    psmCheck(filterRoutineBeltCommand(BeltMotorCommand::Stop, Mode::EStopped) == BeltMotorCommand::Stop,
             "Stop stays Stop while EStopped");

    BeltMotor motor;
    motor.setCommand(BeltMotorCommand::Run);
    motor.resolve();
    motor.resolve();
    psmCheck(motor.actualState() == BeltMotorState::Running, "motor is Running before forceStop");
    motor.forceStop();
    psmCheck(motor.actualState() == BeltMotorState::Stopped,
             "forceStop immediately sets Stopped, bypassing RampingDown");

    return 0;
}
