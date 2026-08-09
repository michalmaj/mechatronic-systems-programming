#include "support/check.hpp"
#include <psm/belt_motor.hpp>

int main() {
    psm::BeltMotor motor;
    psmCheck(motor.actualState() == psm::BeltMotorState::Stopped, "starts Stopped");

    motor.setCommand(psm::BeltMotorCommand::Run);
    motor.resolve();
    psmCheck(motor.actualState() == psm::BeltMotorState::RampingUp, "first resolve begins ramping up");

    motor.resolve();
    psmCheck(motor.actualState() == psm::BeltMotorState::Running, "second resolve reaches Running");

    motor.resolve();
    psmCheck(motor.actualState() == psm::BeltMotorState::Running, "stays Running while still commanded to Run");

    motor.setCommand(psm::BeltMotorCommand::Stop);
    motor.resolve();
    psmCheck(motor.actualState() == psm::BeltMotorState::RampingDown,
             "commanding Stop from Running begins ramping down");

    motor.resolve();
    psmCheck(motor.actualState() == psm::BeltMotorState::Stopped, "reaches Stopped");

    motor.setCommand(psm::BeltMotorCommand::Run);
    motor.resolve();
    psmCheck(motor.actualState() == psm::BeltMotorState::RampingUp,
             "commanding Run from Stopped begins ramping up (again)");

    motor.setCommand(psm::BeltMotorCommand::Stop);
    motor.resolve();
    psmCheck(motor.actualState() == psm::BeltMotorState::RampingDown,
             "wanting Stop while RampingUp reverses toward RampingDown");

    return 0;
}
