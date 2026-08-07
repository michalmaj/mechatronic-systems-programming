#pragma once

#include <vector>

#include <psm/belt_motor.hpp>
#include <psm/diverter.hpp>
#include <psm/estop_latch.hpp>
#include <psm/mode.hpp>
#include <psm/plant.hpp>
#include <psm/scenario_input.hpp>
#include <psm/tick_result.hpp>

namespace psm {

class Engine {
public:
    explicit Engine(std::vector<ScenarioInput> script = {});

    void spawnItem(Item item);
    TickResult step();

private:
    std::vector<ScenarioInput> script_;
    Tick tick_ = 0;
    Mode mode_ = Mode::Idle;
    EStopLatchState latch_ = EStopLatchState::Released;
    Plant plant_;
    Diverter diverter_;
    BeltMotor beltMotor_;
};

}  // namespace psm
