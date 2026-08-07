#pragma once

#include <optional>

#include <psm/belt_motor.hpp>
#include <psm/diverter.hpp>
#include <psm/item.hpp>
#include <psm/system_event.hpp>

namespace psm {

class Plant {
public:
    void spawnItem(Item item);
    std::optional<SystemEventKind> advance(const Diverter& diverter, const BeltMotor& beltMotor);
    const std::optional<Item>& currentItem() const;

private:
    std::optional<Item> item_;
    int divertingWaitTicks_ = 0;
};

}  // namespace psm
