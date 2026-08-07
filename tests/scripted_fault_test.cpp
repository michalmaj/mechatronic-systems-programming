#include "support/check.hpp"
#include <psm/scripted_fault.hpp>

int main() {
    using psm::FaultKind;
    using psm::FaultTarget;
    using psm::isValidFaultScript;
    using psm::ScriptedFault;

    psmCheck(isValidFaultScript({}), "an empty fault script is valid");

    psmCheck(isValidFaultScript({{0, 10, FaultTarget::PresenceSensor, FaultKind::Missing}}),
             "Missing is allowed on PresenceSensor");
    psmCheck(isValidFaultScript({{0, 10, FaultTarget::PresenceSensor, FaultKind::Stale}}),
             "Stale is allowed on PresenceSensor");
    psmCheck(!isValidFaultScript({{0, 10, FaultTarget::PresenceSensor, FaultKind::OutOfRange}}),
             "OutOfRange is not allowed on PresenceSensor");
    psmCheck(!isValidFaultScript({{0, 10, FaultTarget::PresenceSensor, FaultKind::Noisy}}),
             "Noisy is not allowed on PresenceSensor");

    psmCheck(isValidFaultScript({{0, 10, FaultTarget::WeightSensor, FaultKind::Missing}}),
             "Missing is allowed on WeightSensor");
    psmCheck(isValidFaultScript({{0, 10, FaultTarget::WeightSensor, FaultKind::Stale}}),
             "Stale is allowed on WeightSensor");
    psmCheck(isValidFaultScript({{0, 10, FaultTarget::WeightSensor, FaultKind::OutOfRange}}),
             "OutOfRange is allowed on WeightSensor");
    psmCheck(isValidFaultScript({{0, 10, FaultTarget::WeightSensor, FaultKind::Noisy}}),
             "Noisy is allowed on WeightSensor");

    psmCheck(isValidFaultScript({{0, 10, FaultTarget::Diverter, FaultKind::Blocked}}),
             "Blocked is allowed on Diverter");
    psmCheck(!isValidFaultScript({{0, 10, FaultTarget::Diverter, FaultKind::Missing}}),
             "Missing is not allowed on Diverter");

    psmCheck(!isValidFaultScript({
                  {0, 10, FaultTarget::PresenceSensor, FaultKind::Missing},
                  {5, 8, FaultTarget::PresenceSensor, FaultKind::Noisy},
              }),
             "a script is invalid if any single fault in it uses a disallowed kind for its target");

    using psm::activeFault;

    psmCheck(!activeFault(FaultTarget::WeightSensor, 0, {}).has_value(), "no faults scripted: never active");

    const std::vector<ScriptedFault> faults = {{2, 5, FaultTarget::WeightSensor, FaultKind::Missing}};
    psmCheck(!activeFault(FaultTarget::WeightSensor, 1, faults).has_value(), "before the window: not active");
    psmCheck(activeFault(FaultTarget::WeightSensor, 2, faults) == FaultKind::Missing,
             "at the start of the window: active");
    psmCheck(activeFault(FaultTarget::WeightSensor, 4, faults) == FaultKind::Missing, "inside the window: active");
    psmCheck(!activeFault(FaultTarget::WeightSensor, 5, faults).has_value(),
             "at the end tick: no longer active (until is exclusive)");
    psmCheck(!activeFault(FaultTarget::PresenceSensor, 3, faults).has_value(), "different target: not active");

    return 0;
}
