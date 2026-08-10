#pragma once

#include <optional>
#include <string>

#include <psm/belt_motor_state.hpp>
#include <psm/diverter_command.hpp>
#include <psm/diverter_position.hpp>
#include <psm/estop_latch.hpp>
#include <psm/item.hpp>
#include <psm/mode.hpp>
#include <psm/sensor_snapshot.hpp>
#include <psm/system_event_kind.hpp>
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
    SensorSnapshot sensors{0, PresenceReading{ReadingStatus::Ok, false}, WeightReading{ReadingStatus::Ok, 0}};
    std::optional<SystemEventKind> event;
};

std::string describe(const TickResult& result);

}  // namespace psm
