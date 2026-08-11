#include <psm/diverter.hpp>

namespace psm {

void Diverter::setCommand(DiverterCommand command) {
    command_ = command;
}

void Diverter::resolve(std::optional<DiverterFaultKind> fault) {
    if (fault.has_value() && *fault == DiverterFaultKind::Blocked) {
        return;
    }
    const DiverterPosition target =
        (command_ == DiverterCommand::HoldStraight) ? DiverterPosition::Straight : DiverterPosition::Diverted;
    if (actual_ == target) {
        return;
    }
    if (actual_ == DiverterPosition::Moving) {
        actual_ = target;
        return;
    }
    actual_ = DiverterPosition::Moving;
}

DiverterPosition Diverter::actualPosition() const {
    return actual_;
}

bool Diverter::isSettled() const {
    const DiverterPosition target =
        (command_ == DiverterCommand::HoldStraight) ? DiverterPosition::Straight : DiverterPosition::Diverted;
    return actual_ == target;
}

}  // namespace psm
