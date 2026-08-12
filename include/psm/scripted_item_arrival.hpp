#pragma once

#include <psm/item.hpp>
#include <psm/tick.hpp>

namespace psm {

struct ScriptedItemArrival {
    Tick at;
    ItemId id;
    Grams mass;
};

}  // namespace psm
