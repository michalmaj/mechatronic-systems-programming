#include "support/check.hpp"
#include <psm/belt_motor.hpp>

int main() {
    psm::BeltMotor motor;
    psmCheck(motor.actualState() == psm::BeltMotorState::Stopped, "starts Stopped");

    motor.setCommand(psm::BeltMotorCommand::Run);
    motor.resolve();
    psmCheck(motor.actualState() == psm::BeltMotorState::RampingUp, "first resolve begins RampingUp");

    motor.resolve();
    psmCheck(motor.actualState() == psm::BeltMotorState::Running, "second resolve arrives at Running");

    motor.resolve();
    psmCheck(motor.actualState() == psm::BeltMotorState::Running, "stays Running while still commanded Run");

    motor.setCommand(psm::BeltMotorCommand::Stop);
    motor.resolve();
    psmCheck(motor.actualState() == psm::BeltMotorState::RampingDown, "commanding Stop begins RampingDown");

    motor.resolve();
    psmCheck(motor.actualState() == psm::BeltMotorState::Stopped, "arrives at Stopped");

    motor.setCommand(psm::BeltMotorCommand::Run);
    motor.resolve();
    psmCheck(motor.actualState() == psm::BeltMotorState::RampingUp, "Stopped -> RampingUp again");

    motor.setCommand(psm::BeltMotorCommand::Stop);
    motor.resolve();
    psmCheck(motor.actualState() == psm::BeltMotorState::RampingDown,
             "reversing the command mid-ramp reverses ramp direction instead of completing the old one");

    return 0;
}
