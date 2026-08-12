#pragma once

#include <psm/tick.hpp>

namespace psm {

enum class ScenarioInputKind {
    EmergencyStopPressed,
    EmergencyStopReleased,
    Reset,
    StartRequested,
    StopRequested
};

struct ScenarioInput {
    Tick at;
    ScenarioInputKind kind;
};

}  // namespace psm
