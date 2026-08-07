#include "support/check.hpp"
#include <psm/scenario_input.hpp>

int main() {
    using psm::isValidScenario;
    using psm::ScenarioInput;
    using psm::ScenarioInputKind;

    psmCheck(isValidScenario({}), "an empty scenario is valid");

    psmCheck(isValidScenario({
                 {0, ScenarioInputKind::StartRequested},
                 {5, ScenarioInputKind::StopRequested},
             }),
             "Start and Stop at different ticks is valid");

    psmCheck(!isValidScenario({
                  {3, ScenarioInputKind::StartRequested},
                  {3, ScenarioInputKind::StopRequested},
              }),
             "Start and Stop at the same tick is invalid");

    psmCheck(!isValidScenario({
                  {7, ScenarioInputKind::EmergencyStopPressed},
                  {7, ScenarioInputKind::EmergencyStopReleased},
              }),
             "Pressed and Released at the same tick is invalid");

    psmCheck(isValidScenario({
                 {2, ScenarioInputKind::EmergencyStopReleased},
                 {2, ScenarioInputKind::Reset},
             }),
             "Released and Reset at the same tick is explicitly permitted");

    psmCheck(isValidScenario({
                 {1, ScenarioInputKind::EmergencyStopPressed},
                 {4, ScenarioInputKind::EmergencyStopReleased},
                 {4, ScenarioInputKind::Reset},
                 {5, ScenarioInputKind::StartRequested},
             }),
             "a realistic multi-input scenario is valid");

    return 0;
}
