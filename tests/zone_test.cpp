#include "support/check.hpp"
#include <psm/zone.hpp>

int main() {
    psmCheck(psm::toString(psm::Zone::Infeed) == "Infeed", "Infeed prints as Infeed");
    psmCheck(psm::toString(psm::Zone::PresenceCheck) == "PresenceCheck", "PresenceCheck prints as PresenceCheck");
    psmCheck(psm::toString(psm::Zone::Weighing) == "Weighing", "Weighing prints as Weighing");
    psmCheck(psm::toString(psm::Zone::Diverting) == "Diverting", "Diverting prints as Diverting");
    psmCheck(psm::toString(psm::Zone::OutputLight) == "OutputLight", "OutputLight prints as OutputLight");
    psmCheck(psm::toString(psm::Zone::OutputHeavy) == "OutputHeavy", "OutputHeavy prints as OutputHeavy");
    return 0;
}
