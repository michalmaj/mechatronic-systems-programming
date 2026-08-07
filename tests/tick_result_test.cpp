#include "support/check.hpp"
#include <psm/tick_result.hpp>

int main() {
    psm::TickResult empty{5, std::nullopt, psm::DiverterPosition::Straight, psm::Mode::Idle,
                           psm::BeltMotorState::Stopped};
    psmCheck(empty.tick == 5, "TickResult stores its tick number");
    psmCheck(!empty.item.has_value(), "TickResult can represent an empty plant");
    psmCheck(empty.diverterActual == psm::DiverterPosition::Straight, "TickResult stores diverter actual position");
    psmCheck(empty.mode == psm::Mode::Idle, "TickResult stores Mode");
    psmCheck(empty.beltActual == psm::BeltMotorState::Stopped, "TickResult stores belt actual state");

    psm::TickResult withItem{6, psm::Item{1, psm::Zone::Diverting, 750}, psm::DiverterPosition::Moving,
                              psm::Mode::Running, psm::BeltMotorState::Running};
    psmCheck(withItem.item.has_value() && withItem.item->id == 1, "TickResult can carry an item snapshot");
    psmCheck(withItem.diverterActual == psm::DiverterPosition::Moving, "TickResult reflects a Moving diverter");
    psmCheck(withItem.mode == psm::Mode::Running, "TickResult reflects Running mode");
    psmCheck(withItem.beltActual == psm::BeltMotorState::Running, "TickResult reflects a Running belt");

    return 0;
}
