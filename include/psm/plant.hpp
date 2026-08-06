#pragma once

#include <optional>

#include <psm/diverter_position.hpp>
#include <psm/item.hpp>

namespace psm {

class Plant {
public:
    void spawnItem(Item item);
    void advance(DiverterPosition diverter);
    const std::optional<Item>& currentItem() const;

private:
    std::optional<Item> item_;
};

}  // namespace psm
