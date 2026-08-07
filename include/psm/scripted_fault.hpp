#pragma once

#include <optional>
#include <vector>

#include <psm/tick.hpp>

namespace psm {

enum class FaultTarget { PresenceSensor, WeightSensor, Diverter };
enum class FaultKind { Missing, Stale, OutOfRange, Noisy, Blocked };

struct ScriptedFault {
    Tick from;
    Tick until;
    FaultTarget target;
    FaultKind kind;
};

bool isValidFaultScript(const std::vector<ScriptedFault>& faults);
std::optional<FaultKind> activeFault(FaultTarget target, Tick now, const std::vector<ScriptedFault>& faults);

}  // namespace psm
