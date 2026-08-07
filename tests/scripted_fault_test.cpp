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

    psmCheck(!isValidFaultScript({
                  {0, 10, FaultTarget::PresenceSensor, FaultKind::Missing},
                  {5, 8, FaultTarget::PresenceSensor, FaultKind::Noisy},
              }),
             "a script is invalid if any single fault in it uses a disallowed kind for its target");

    return 0;
}
