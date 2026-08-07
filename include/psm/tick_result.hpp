#pragma once

#include <optional>

#include <psm/diverter_position.hpp>
#include <psm/item.hpp>
#include <psm/tick.hpp>

namespace psm {

struct TickResult {
    Tick tick;
    std::optional<Item> item;
    DiverterPosition diverterActual;
};

}  // namespace psm
