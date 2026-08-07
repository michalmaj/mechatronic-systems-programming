#include <psm/scenario_input.hpp>

namespace psm {

namespace {

bool isForbiddenPair(ScenarioInputKind a, ScenarioInputKind b) {
    const bool startStop = (a == ScenarioInputKind::StartRequested && b == ScenarioInputKind::StopRequested) ||
                            (a == ScenarioInputKind::StopRequested && b == ScenarioInputKind::StartRequested);
    const bool pressRelease =
        (a == ScenarioInputKind::EmergencyStopPressed && b == ScenarioInputKind::EmergencyStopReleased) ||
        (a == ScenarioInputKind::EmergencyStopReleased && b == ScenarioInputKind::EmergencyStopPressed);
    return startStop || pressRelease;
}

}  // namespace

bool isValidScenario(const std::vector<ScenarioInput>& inputs) {
    for (std::size_t i = 0; i < inputs.size(); ++i) {
        for (std::size_t j = i + 1; j < inputs.size(); ++j) {
            if (inputs[i].at == inputs[j].at && isForbiddenPair(inputs[i].kind, inputs[j].kind)) {
                return false;
            }
        }
    }
    return true;
}

}  // namespace psm
