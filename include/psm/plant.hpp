#pragma once

#include <optional>

#include <psm/belt_motor.hpp>
#include <psm/diverter.hpp>
#include <psm/item.hpp>

namespace psm {

class Plant {
public:
    void spawnItem(Item item);
    void advance(const Diverter& diverter, const BeltMotor& beltMotor);
    const std::optional<Item>& currentItem() const;

private:
    std::optional<Item> item_;
};

}  // namespace psm
