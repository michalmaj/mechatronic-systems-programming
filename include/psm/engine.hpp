#pragma once

#include <vector>

#include <psm/belt_motor.hpp>
#include <psm/diverter.hpp>
#include <psm/estop_latch.hpp>
#include <psm/mode.hpp>
#include <psm/plant.hpp>
#include <psm/presence_sensor.hpp>
#include <psm/scenario_input.hpp>
#include <psm/scripted_fault.hpp>
#include <psm/tick_result.hpp>
#include <psm/weight_sensor.hpp>

namespace psm {

class Engine {
public:
    explicit Engine(std::vector<ScenarioInput> script = {}, std::vector<ScriptedFault> faultScript = {});

    void spawnItem(Item item);
    TickResult step();

private:
    std::vector<ScenarioInput> script_;
    std::vector<ScriptedFault> faultScript_;
    Tick tick_ = 0;
    Mode mode_ = Mode::Idle;
    EStopLatchState latch_ = EStopLatchState::Released;
    Plant plant_;
    Diverter diverter_;
    BeltMotor beltMotor_;
    PresenceSensor presenceSensor_;
    WeightSensor weightSensor_;
};

}  // namespace psm
