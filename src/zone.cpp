#include <psm/zone.hpp>

namespace psm {

std::string_view toString(Zone zone) {
    switch (zone) {
        case Zone::Infeed: return "Infeed";
        case Zone::PresenceCheck: return "PresenceCheck";
        case Zone::Weighing: return "Weighing";
        case Zone::Diverting: return "Diverting";
        case Zone::OutputLight: return "OutputLight";
        case Zone::OutputHeavy: return "OutputHeavy";
    }
    return "Unknown";
}

}  // namespace psm
