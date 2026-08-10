#pragma once

#include <optional>

#include <psm/belt_motor.hpp>
#include <psm/controller_state.hpp>
#include <psm/diverter.hpp>
#include <psm/estop_latch.hpp>
#include <psm/fault_kind.hpp>
#include <psm/fault_target.hpp>
#include <psm/mode.hpp>
#include <psm/plant.hpp>
#include <psm/presence_sensor.hpp>
#include <psm/tick_result.hpp>
#include <psm/weight_sensor.hpp>

namespace psm {

class Engine {
public:
    void spawnItem(Item item);
    void requestStart();
    void requestStop();
    void requestEStop();
    void releaseEStop();
    void requestReset();
    void injectFault(FaultTarget target, FaultKind kind);
    void clearFault(FaultTarget target);
    TickResult step();

private:
    Tick tick_ = 0;
    Plant plant_;
    Diverter diverter_;
    Mode mode_ = Mode::Idle;
    BeltMotor beltMotor_;
    EStopLatchState latch_ = EStopLatchState::Released;
    PresenceSensor presenceSensor_;
    WeightSensor weightSensor_;
    ControllerState controllerState_;
    std::optional<FaultKind> presenceFault_;
    std::optional<FaultKind> weightFault_;
    bool startRequested_ = false;
    bool stopRequested_ = false;
    bool eStopPressed_ = false;
    bool eStopReleased_ = false;
    bool resetRequested_ = false;
};

}  // namespace psm
