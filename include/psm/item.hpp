#pragma once

#include <cstdint>

#include <psm/zone.hpp>

namespace psm {

using ItemId = std::uint32_t;
using Grams = std::int32_t;

struct Item {
    ItemId id;
    Zone zone;
    Grams mass;
};

}  // namespace psm
