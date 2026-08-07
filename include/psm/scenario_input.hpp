#pragma once

#include <vector>

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

bool isValidScenario(const std::vector<ScenarioInput>& inputs);

}  // namespace psm
