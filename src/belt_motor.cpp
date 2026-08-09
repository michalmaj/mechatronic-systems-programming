#include <psm/belt_motor.hpp>

namespace psm {

void BeltMotor::setCommand(BeltMotorCommand command) {
    command_ = command;
}

void BeltMotor::resolve() {
    const BeltMotorState target =
        (command_ == BeltMotorCommand::Run) ? BeltMotorState::Running : BeltMotorState::Stopped;
    switch (actual_) {
        case BeltMotorState::Stopped:
            if (target == BeltMotorState::Running) {
                actual_ = BeltMotorState::RampingUp;
            }
            break;
        case BeltMotorState::RampingUp:
            actual_ = (target == BeltMotorState::Running) ? BeltMotorState::Running : BeltMotorState::RampingDown;
            break;
        case BeltMotorState::Running:
            if (target == BeltMotorState::Stopped) {
                actual_ = BeltMotorState::RampingDown;
            }
            break;
        case BeltMotorState::RampingDown:
            actual_ = (target == BeltMotorState::Stopped) ? BeltMotorState::Stopped : BeltMotorState::RampingUp;
            break;
    }
}

void BeltMotor::forceStop() {
    // TODO (Misja 18: dwie_niezalezne_sciezki): ustaw command_ na Stop ORAZ actual_ na Stopped,
    // w tym samym wywołaniu -- to jedyna metoda tej klasy, która celowo pomija rampowanie.
}

BeltMotorState BeltMotor::actualState() const {
    return actual_;
}

}  // namespace psm
