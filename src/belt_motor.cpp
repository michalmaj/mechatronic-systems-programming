#include <psm/belt_motor.hpp>

namespace psm {

void BeltMotor::setCommand(BeltMotorCommand command) {
    // TODO (Misja 13: silnik_przenosnika): zapisz command w polu command_.
    (void)command;
}

void BeltMotor::resolve() {
    // TODO (Misja 13: silnik_przenosnika): zgodnie z regułą z treści misji, przesuń actual_
    // o jeden krok w stronę stanu wyznaczonego przez command_.
}

BeltMotorState BeltMotor::actualState() const {
    // TODO (Misja 13: silnik_przenosnika): zwróć actual_.
    return BeltMotorState::Stopped;
}

}  // namespace psm
