#pragma once

#include <optional>
#include <string>

#include <psm/diverter_command.hpp>
#include <psm/diverter_position.hpp>
#include <psm/item.hpp>
#include <psm/tick.hpp>

namespace psm {

struct TickResult {
    Tick tick;
    std::optional<Item> item;
    DiverterCommand diverterCommand;
    DiverterPosition diverterActual;
};

std::string describe(const TickResult& result);

}  // namespace psm
