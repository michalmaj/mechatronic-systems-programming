#pragma once

#include <optional>

#include <psm/item.hpp>
#include <psm/reading_status.hpp>
#include <psm/tick.hpp>

namespace psm {

struct PresenceReading {
    ReadingStatus status;
    bool occupied;
};

struct WeightReading {
    ReadingStatus status;
    Grams grams;
};

struct SensorSnapshot {
    Tick observedAt;
    PresenceReading presence;
    std::optional<ItemId> presenceObservedItemId;
    WeightReading weight;
    std::optional<ItemId> weightObservedItemId;
};

}  // namespace psm
