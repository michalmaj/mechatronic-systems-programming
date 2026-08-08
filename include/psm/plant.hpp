#pragma once

#include <optional>

#include <psm/diverter_position.hpp>
#include <psm/item.hpp>

namespace psm {

struct Plant {
    std::optional<Item> item;
};

void spawnItem(Plant& plant, Item item);
void advance(Plant& plant, DiverterPosition diverterPosition);

}  // namespace psm
