#pragma once

#include <optional>

#include <psm/diverter.hpp>
#include <psm/item.hpp>
#include <psm/system_event_kind.hpp>
#include <psm/zone.hpp>

namespace psm {

struct Plant {
    std::optional<Item> infeed;
    std::optional<Item> presenceCheck;
    std::optional<Item> weighing;
    std::optional<Item> diverting;
};

struct ItemDeparture {
    ItemId id;
    Zone destination;
};

struct AdvanceResult {
    std::optional<SystemEventKind> event;
    std::optional<ItemDeparture> departure;
};

bool spawnItem(Plant& plant, ItemId id, Grams mass);
AdvanceResult advance(Plant& plant, const Diverter& diverter, bool routingReady = true);

}  // namespace psm
