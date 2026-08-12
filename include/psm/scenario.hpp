#pragma once

#include <optional>
#include <vector>

#include <psm/scenario_input.hpp>
#include <psm/scripted_diverter_fault.hpp>
#include <psm/scripted_item_arrival.hpp>
#include <psm/scripted_sensor_fault.hpp>
#include <psm/tick.hpp>
#include <psm/tick_result.hpp>

namespace psm {

struct Scenario {
    std::vector<ScenarioInput> operatorInputs;
    std::vector<ScriptedItemArrival> arrivals;
    std::vector<ScriptedSensorFault> sensorFaults;
    std::vector<ScriptedDiverterFault> diverterFaults;
    Tick duration;
};

bool isValidScenario(const Scenario& scenario);
std::optional<std::vector<TickResult>> runScenario(const Scenario& scenario);

}  // namespace psm
