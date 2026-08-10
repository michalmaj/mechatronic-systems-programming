#pragma once

#include <optional>

#include <psm/diverter.hpp>
#include <psm/item.hpp>
#include <psm/system_event_kind.hpp>

namespace psm {

struct Plant {
    std::optional<Item> item;
    int divertingWaitTicks = 0;
};

void spawnItem(Plant& plant, Item item);
std::optional<SystemEventKind> advance(Plant& plant, const Diverter& diverter, bool routingReady = true);

}  // namespace psm
