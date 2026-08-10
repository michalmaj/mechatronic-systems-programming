#include <iostream>

#include <psm/engine.hpp>
#include <psm/item.hpp>
#include <psm/tick_result.hpp>
#include <psm/zone.hpp>

namespace {

const char* modeName(psm::Mode mode) {
    switch (mode) {
        case psm::Mode::Idle: return "Idle";
        case psm::Mode::Running: return "Running";
        case psm::Mode::EStopped: return "EStopped";
    }
    return "Unknown";
}

const char* beltStateName(psm::BeltMotorState state) {
    switch (state) {
        case psm::BeltMotorState::Stopped: return "Stopped";
        case psm::BeltMotorState::RampingUp: return "RampingUp";
        case psm::BeltMotorState::Running: return "Running";
        case psm::BeltMotorState::RampingDown: return "RampingDown";
    }
    return "Unknown";
}

const char* latchName(psm::EStopLatchState latch) {
    switch (latch) {
        case psm::EStopLatchState::Released: return "Released";
        case psm::EStopLatchState::Engaged: return "Engaged";
        case psm::EStopLatchState::Armed: return "Armed";
    }
    return "Unknown";
}

void printTick(const psm::TickResult& result) {
    std::cout << psm::describe(result) << ", mode=" << modeName(result.mode)
              << ", belt=" << beltStateName(result.beltActual) << ", latch=" << latchName(result.latch)
              << '\n';
}

}  // namespace

int main() {
    psm::Engine engine;
    engine.spawnItem(psm::Item{1, psm::Zone::Infeed, 750});
    engine.requestStart();

    for (int i = 0; i < 3; ++i) {
        printTick(engine.step());
    }

    engine.requestEStop();
    printTick(engine.step());

    engine.releaseEStop();
    printTick(engine.step());

    engine.requestReset();
    printTick(engine.step());

    engine.requestStart();
    for (int i = 0; i < 5; ++i) {
        printTick(engine.step());
    }

    return 0;
}
