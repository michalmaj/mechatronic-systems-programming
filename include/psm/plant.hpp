#pragma once

#include <optional>

#include <psm/diverter.hpp>
#include <psm/item.hpp>

namespace psm {

struct Plant {
    std::optional<Item> item;
};

void spawnItem(Plant& plant, Item item);
void advance(Plant& plant, const Diverter& diverter);

}  // namespace psm
