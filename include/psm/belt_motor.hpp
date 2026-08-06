#pragma once

namespace psm {

enum class BeltMotorCommand { Run, Stop };
enum class BeltMotorState { Stopped, Running, RampingUp, RampingDown };

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
