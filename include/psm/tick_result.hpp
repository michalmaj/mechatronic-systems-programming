#pragma once

#include <optional>
#include <string>

#include <psm/belt_motor_state.hpp>
#include <psm/diverter_command.hpp>
#include <psm/diverter_position.hpp>
#include <psm/estop_latch.hpp>
#include <psm/item.hpp>
#include <psm/mode.hpp>
#include <psm/tick.hpp>

namespace psm {

struct TickResult {
    Tick tick;
    std::optional<Item> item;
    DiverterCommand diverterCommand;
    DiverterPosition diverterActual;
    Mode mode = Mode::Idle;
    BeltMotorState beltActual = BeltMotorState::Stopped;
    EStopLatchState latch = EStopLatchState::Released;
};

std::string describe(const TickResult& result);

}  // namespace psm
