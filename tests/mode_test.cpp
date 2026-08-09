#include "support/check.hpp"
#include <psm/mode.hpp>

int main() {
    psmCheck(psm::modeStep(psm::Mode::Idle, false, false) == psm::Mode::Idle, "no request: stays Idle");
    psmCheck(psm::modeStep(psm::Mode::Idle, true, false) == psm::Mode::Running,
             "start request: Idle becomes Running");
    psmCheck(psm::modeStep(psm::Mode::Running, false, false) == psm::Mode::Running, "no request: stays Running");
    psmCheck(psm::modeStep(psm::Mode::Running, false, true) == psm::Mode::Idle,
             "stop request: Running becomes Idle");
    psmCheck(psm::modeStep(psm::Mode::Running, true, true) == psm::Mode::Idle,
             "both requested at once: stop takes priority");
    psmCheck(psm::modeStep(psm::Mode::Idle, true, true) == psm::Mode::Idle,
             "both requested at once while Idle: stop still wins, stays Idle");
    return 0;
}
