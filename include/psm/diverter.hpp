#pragma once

#include <psm/diverter_command.hpp>
#include <psm/diverter_position.hpp>

namespace psm {

class Diverter {
public:
    void setCommand(DiverterCommand command);
    void resolve();
    DiverterPosition actualPosition() const;
    bool isSettled() const;

private:
    DiverterCommand command_ = DiverterCommand::HoldStraight;
    DiverterPosition actual_ = DiverterPosition::Straight;
};

}  // namespace psm
