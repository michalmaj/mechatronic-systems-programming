#include "support/check.hpp"
#include <psm/item_motion.hpp>

int main() {
    psm::Item item{1, psm::Zone::Infeed, 750};

    psm::advanceZone(item);
    psmCheck(item.zone == psm::Zone::PresenceCheck, "Infeed -> PresenceCheck");

    psm::advanceZone(item);
    psmCheck(item.zone == psm::Zone::Weighing, "PresenceCheck -> Weighing");

    psm::advanceZone(item);
    psmCheck(item.zone == psm::Zone::Diverting, "Weighing -> Diverting");

    psm::advanceZone(item);
    psmCheck(item.zone == psm::Zone::Diverting, "advanceZone stops at Diverting -- it does not decide the branch");

    return 0;
}
