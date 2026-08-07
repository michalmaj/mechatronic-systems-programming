#pragma once

#include <optional>

#include <psm/belt_motor.hpp>
#include <psm/diverter_position.hpp>
#include <psm/item.hpp>
#include <psm/mode.hpp>
#include <psm/sensor_snapshot.hpp>
#include <psm/system_event.hpp>
#include <psm/tick.hpp>

namespace psm {

struct TickResult {
    Tick tick;
    std::optional<Item> item;
    DiverterPosition diverterActual;
    Mode mode;
    BeltMotorState beltActual;
    SensorSnapshot sensors;
    std::optional<SystemEventKind> event;
};

}  // namespace psm
