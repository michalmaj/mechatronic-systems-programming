#pragma once

#include <psm/belt_motor.hpp>
#include <psm/diverter.hpp>
#include <psm/estop_latch.hpp>
#include <psm/mode.hpp>
#include <psm/plant.hpp>
#include <psm/tick_result.hpp>

namespace psm {

class Engine {
public:
    void spawnItem(Item item);
    void requestStart();
    void requestStop();
    void requestEStop();
    void releaseEStop();
    void requestReset();
    TickResult step();

private:
    Tick tick_ = 0;
    Plant plant_;
    Diverter diverter_;
    Mode mode_ = Mode::Idle;
    BeltMotor beltMotor_;
    EStopLatchState latch_ = EStopLatchState::Released;
    bool startRequested_ = false;
    bool stopRequested_ = false;
    bool eStopPressed_ = false;
    bool eStopReleased_ = false;
    bool resetRequested_ = false;
};

}  // namespace psm
