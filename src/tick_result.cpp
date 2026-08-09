#include <psm/tick_result.hpp>

#include <psm/zone.hpp>

namespace psm {

std::string describe(const TickResult& result) {
    if (!result.item.has_value()) {
        return "tick " + std::to_string(result.tick) + ": empty";
    }
    return "tick " + std::to_string(result.tick) + ": item " + std::to_string(result.item->id) +
           " in zone " + std::string(psm::toString(result.item->zone));
}

}  // namespace psm
