#include <psm/item_motion.hpp>

namespace psm {

void advanceZone(Item& item) {
    switch (item.zone) {
        case Zone::Infeed: item.zone = Zone::PresenceCheck; break;
        case Zone::PresenceCheck: item.zone = Zone::Weighing; break;
        case Zone::Weighing: item.zone = Zone::Diverting; break;
        case Zone::Diverting: break;
        case Zone::OutputLight: break;
        case Zone::OutputHeavy: break;
    }
}

}  // namespace psm
