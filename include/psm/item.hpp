#pragma once

#include <cstdint>
#include <optional>

#include <psm/weight_class.hpp>

namespace psm {

using ItemId = std::uint32_t;
using Grams = std::int32_t;

struct Item {
    ItemId id;
    Grams mass;
    bool presenceConfirmed = false;
    std::optional<WeightClass> classification;
    int divertingWaitTicks = 0;
};

}  // namespace psm
