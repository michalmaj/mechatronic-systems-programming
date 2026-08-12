#include "support/check.hpp"
#include <psm/scenario.hpp>

int main() {
    using psm::DiverterFaultKind;
    using psm::Scenario;
    using psm::ScenarioInputKind;
    using psm::SensorFaultKind;
    using psm::SensorTarget;

    {
        Scenario scenario{};
        scenario.duration = 0;
        psmCheck(psm::isValidScenario(scenario), "an empty, zero-duration scenario is valid");
    }

    {
        Scenario scenario{};
        scenario.arrivals = {{0, 1, 750}};
        scenario.operatorInputs = {{0, ScenarioInputKind::StartRequested}};
        scenario.duration = 5;
        psmCheck(psm::isValidScenario(scenario), "a simple, well-formed scenario is valid");
    }

    {
        Scenario scenario{};
        scenario.sensorFaults = {{3, 3, SensorTarget::Weight, SensorFaultKind::Stale}};
        scenario.duration = 5;
        psmCheck(!psm::isValidScenario(scenario), "a fault interval with from == until is invalid");
    }
    {
        Scenario scenario{};
        scenario.sensorFaults = {{3, 2, SensorTarget::Weight, SensorFaultKind::Stale}};
        scenario.duration = 5;
        psmCheck(!psm::isValidScenario(scenario), "a fault interval with from > until is invalid");
    }
    {
        Scenario scenario{};
        scenario.diverterFaults = {{0, 6, DiverterFaultKind::Blocked}};
        scenario.duration = 5;
        psmCheck(!psm::isValidScenario(scenario), "a fault interval extending past duration is invalid");
    }
    {
        Scenario scenario{};
        scenario.diverterFaults = {{0, 5, DiverterFaultKind::Blocked}};
        scenario.duration = 5;
        psmCheck(psm::isValidScenario(scenario), "a fault interval ending exactly at duration is valid");
    }

    {
        Scenario scenario{};
        scenario.sensorFaults = {{0, 5, SensorTarget::Weight, SensorFaultKind::Stale},
                                  {3, 8, SensorTarget::Weight, SensorFaultKind::Missing}};
        scenario.duration = 10;
        psmCheck(!psm::isValidScenario(scenario), "overlapping sensor faults on the same target are invalid");
    }
    {
        Scenario scenario{};
        scenario.sensorFaults = {{0, 5, SensorTarget::Weight, SensorFaultKind::Stale},
                                  {3, 8, SensorTarget::Presence, SensorFaultKind::Missing}};
        scenario.duration = 10;
        psmCheck(psm::isValidScenario(scenario),
                 "overlapping sensor faults on different targets are valid -- they're independent");
    }
    {
        Scenario scenario{};
        scenario.sensorFaults = {{0, 3, SensorTarget::Weight, SensorFaultKind::Stale},
                                  {3, 6, SensorTarget::Weight, SensorFaultKind::Missing}};
        scenario.duration = 10;
        psmCheck(psm::isValidScenario(scenario),
                 "adjacent, non-overlapping [from,until) sensor fault intervals are valid");
    }

    {
        Scenario scenario{};
        scenario.diverterFaults = {{0, 5, DiverterFaultKind::Blocked}, {3, 8, DiverterFaultKind::Blocked}};
        scenario.duration = 10;
        psmCheck(!psm::isValidScenario(scenario), "overlapping diverter faults are invalid");
    }

    {
        Scenario scenario{};
        scenario.operatorInputs = {{0, ScenarioInputKind::StartRequested}, {0, ScenarioInputKind::StartRequested}};
        scenario.duration = 5;
        psmCheck(!psm::isValidScenario(scenario), "a duplicate kind at the same tick is invalid");
    }
    {
        Scenario scenario{};
        scenario.operatorInputs = {{0, ScenarioInputKind::StartRequested}, {0, ScenarioInputKind::StopRequested}};
        scenario.duration = 5;
        psmCheck(!psm::isValidScenario(scenario),
                 "StartRequested and StopRequested at the same tick are invalid -- not the allowed pair");
    }
    {
        Scenario scenario{};
        scenario.operatorInputs = {{0, ScenarioInputKind::Reset}, {0, ScenarioInputKind::StartRequested}};
        scenario.duration = 5;
        psmCheck(!psm::isValidScenario(scenario),
                 "Reset and StartRequested at the same tick are invalid, even though Engine tolerates it safely");
    }
    {
        Scenario scenario{};
        scenario.operatorInputs = {{0, ScenarioInputKind::EmergencyStopReleased}, {0, ScenarioInputKind::Reset}};
        scenario.duration = 5;
        psmCheck(psm::isValidScenario(scenario),
                 "EmergencyStopReleased and Reset at the same tick are exactly the one allowed pair");
    }

    {
        Scenario scenario{};
        scenario.operatorInputs = {{5, ScenarioInputKind::StartRequested}};
        scenario.duration = 5;
        psmCheck(!psm::isValidScenario(scenario), "an operator input at >= duration is invalid");
    }
    {
        Scenario scenario{};
        scenario.arrivals = {{5, 1, 750}};
        scenario.duration = 5;
        psmCheck(!psm::isValidScenario(scenario), "an arrival at >= duration is invalid");
    }

    {
        Scenario scenario{};
        scenario.arrivals = {{0, 1, 750}, {0, 2, 100}};
        scenario.duration = 5;
        psmCheck(!psm::isValidScenario(scenario), "two arrivals at the same tick are invalid");
    }

    {
        Scenario scenario{};
        scenario.arrivals = {{0, 1, 750}, {5, 1, 100}};
        scenario.duration = 10;
        psmCheck(!psm::isValidScenario(scenario),
                 "reusing an id across two arrivals is invalid, even at different ticks");
    }

    return 0;
}
