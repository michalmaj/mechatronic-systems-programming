#include <psm/diverter.hpp>

namespace psm {

void Diverter::setCommand(DiverterCommand command) {
    command_ = command;
}

void Diverter::resolve(std::optional<DiverterFaultKind> fault) {
    // TODO (Misja 25: zablokowany_dywerter): jeśli fault == DiverterFaultKind::Blocked, ta funkcja
    // powinna być całkowitym no-opem -- actual_ nie zmienia się wcale, nawet w stronę Moving.
    // W przeciwnym razie zachowanie jest identyczne jak dotychczas (logika poniżej, bez zmian).
    (void)fault;
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
