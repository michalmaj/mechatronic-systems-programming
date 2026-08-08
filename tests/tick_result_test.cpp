#include "support/check.hpp"
#include <psm/tick_result.hpp>

int main() {
    psm::TickResult withItem{3, psm::Item{1, psm::Zone::Diverting, 750}, psm::DiverterCommand::Divert,
                              psm::DiverterPosition::Moving};
    psmCheck(psm::describe(withItem) == "tick 3: item 1 in zone Diverting",
             "describes a tick with an item present");

    psm::TickResult empty{5, std::nullopt, psm::DiverterCommand::HoldStraight,
                           psm::DiverterPosition::Straight};
    psmCheck(psm::describe(empty) == "tick 5: empty", "describes a tick with no item");

    return 0;
}
