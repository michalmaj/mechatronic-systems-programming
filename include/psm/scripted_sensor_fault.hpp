#pragma once

#include <psm/sensor_fault_kind.hpp>
#include <psm/sensor_target.hpp>
#include <psm/tick.hpp>

namespace psm {

struct ScriptedSensorFault {
    Tick from;
    Tick until;
    SensorTarget target;
    SensorFaultKind kind;
};

}  // namespace psm
