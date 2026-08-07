#pragma once

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
    WeightReading weight;
};

}  // namespace psm
