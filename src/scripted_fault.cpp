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

}  // namespace psm
