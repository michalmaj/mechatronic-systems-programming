#include <psm/system_event.hpp>

namespace psm {

std::string_view toString(SystemEventKind kind) {
    switch (kind) {
        case SystemEventKind::DiverterNotReady: return "DiverterNotReady";
        case SystemEventKind::RoutingDeadlineMissed: return "RoutingDeadlineMissed";
    }
    return "Unknown";
}

}  // namespace psm
