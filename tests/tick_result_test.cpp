#include "support/check.hpp"
#include <psm/tick_result.hpp>

int main() {
    psm::TickResult empty{5, std::nullopt, psm::DiverterPosition::Straight};
    psmCheck(empty.tick == 5, "TickResult stores its tick number");
    psmCheck(!empty.item.has_value(), "TickResult can represent an empty plant");
    psmCheck(empty.diverterActual == psm::DiverterPosition::Straight, "TickResult stores diverter actual position");

    psm::TickResult withItem{6, psm::Item{1, psm::Zone::Diverting, 750}, psm::DiverterPosition::Moving};
    psmCheck(withItem.item.has_value() && withItem.item->id == 1, "TickResult can carry an item snapshot");
    psmCheck(withItem.diverterActual == psm::DiverterPosition::Moving, "TickResult reflects a Moving diverter");

    return 0;
}
