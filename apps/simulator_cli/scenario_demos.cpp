#include "scenario_demos.hpp"

psm::Scenario recoveryDemoScenario() {
    psm::Scenario scenario;
    scenario.arrivals = {{0, 1, 750}};
    scenario.operatorInputs = {{0, psm::ScenarioInputKind::StartRequested},
                                {9, psm::ScenarioInputKind::Reset},
                                {10, psm::ScenarioInputKind::StartRequested}};
    scenario.diverterFaults = {{0, 8, psm::DiverterFaultKind::Blocked}};
    scenario.duration = 12;
    return scenario;
}

psm::Scenario multiParcelDemoScenario() {
    psm::Scenario scenario;
    scenario.arrivals = {{0, 1, 100}, {2, 2, 800}, {4, 3, 150}};
    scenario.operatorInputs = {{0, psm::ScenarioInputKind::StartRequested}};
    scenario.duration = 20;
    return scenario;
}
