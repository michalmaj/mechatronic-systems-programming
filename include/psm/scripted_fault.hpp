#pragma once

#include <vector>

#include <psm/tick.hpp>

namespace psm {

enum class FaultTarget { PresenceSensor, WeightSensor };
enum class FaultKind { Missing, Stale, OutOfRange, Noisy };

struct ScriptedFault {
    Tick from;
    Tick until;
    FaultTarget target;
    FaultKind kind;
};

bool isValidFaultScript(const std::vector<ScriptedFault>& faults);

}  // namespace psm
