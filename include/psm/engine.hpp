#pragma once

#include <psm/diverter.hpp>
#include <psm/plant.hpp>
#include <psm/tick_result.hpp>

namespace psm {

class Engine {
public:
    void spawnItem(Item item);
    TickResult step();

private:
    Tick tick_ = 0;
    Plant plant_;
    Diverter diverter_;
};

}  // namespace psm
