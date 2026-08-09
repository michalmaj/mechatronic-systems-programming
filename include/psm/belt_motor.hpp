#pragma once

#include <psm/belt_motor_command.hpp>
#include <psm/belt_motor_state.hpp>

namespace psm {

class BeltMotor {
public:
    void setCommand(BeltMotorCommand command);
    void resolve();
    BeltMotorState actualState() const;

private:
    BeltMotorCommand command_ = BeltMotorCommand::Stop;
    BeltMotorState actual_ = BeltMotorState::Stopped;
};

}  // namespace psm
