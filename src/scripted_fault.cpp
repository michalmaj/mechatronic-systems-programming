#include <psm/scripted_fault.hpp>

namespace psm {

namespace {

bool isAllowed(FaultTarget target, FaultKind kind) {
    switch (target) {
        case FaultTarget::PresenceSensor:
            return kind == FaultKind::Missing || kind == FaultKind::Stale;
        case FaultTarget::WeightSensor:
            return kind == FaultKind::Missing || kind == FaultKind::Stale ||
                   kind == FaultKind::OutOfRange || kind == FaultKind::Noisy;
        case FaultTarget::Diverter:
            return kind == FaultKind::Blocked;
    }
    return false;
}

}  // namespace

bool isValidFaultScript(const std::vector<ScriptedFault>& faults) {
    for (const auto& fault : faults) {
        if (!isAllowed(fault.target, fault.kind)) {
            return false;
        }
    }
    return true;
}

std::optional<FaultKind> activeFault(FaultTarget target, Tick now, const std::vector<ScriptedFault>& faults) {
    for (const auto& fault : faults) {
        if (fault.target == target && fault.from <= now && now < fault.until) {
            return fault.kind;
        }
    }
    return std::nullopt;
}

}  // namespace psm
