#pragma once

#include <optional>

#include <psm/diverter_command.hpp>
#include <psm/diverter_position.hpp>
#include <psm/scripted_fault.hpp>

namespace psm {

class Diverter {
public:
    void setCommand(DiverterCommand command);
    void resolve(std::optional<FaultKind> fault = std::nullopt);
    DiverterPosition actualPosition() const;
    bool isSettled() const;

private:
    DiverterCommand command_ = DiverterCommand::HoldStraight;
    DiverterPosition actual_ = DiverterPosition::Straight;
};

}  // namespace psm
