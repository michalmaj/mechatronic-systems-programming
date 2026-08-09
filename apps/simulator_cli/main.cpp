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

}  // namespace

int main() {
    psm::Engine engine;
    engine.spawnItem(psm::Item{1, psm::Zone::Infeed, 750});
    engine.requestStart();

    for (int i = 0; i < 8; ++i) {
        const psm::TickResult result = engine.step();
        std::cout << psm::describe(result) << ", mode=" << modeName(result.mode)
                  << ", belt=" << beltStateName(result.beltActual) << '\n';
    }

    return 0;
}
