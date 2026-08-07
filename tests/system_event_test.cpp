#include "support/check.hpp"
#include <psm/system_event.hpp>

int main() {
    psm::SystemEvent event{3, psm::SystemEventKind::DiverterNotReady};
    psmCheck(event.at == 3, "SystemEvent stores its tick");
    psmCheck(event.kind == psm::SystemEventKind::DiverterNotReady, "SystemEvent stores its kind");

    psmCheck(psm::SystemEventKind::DiverterNotReady != psm::SystemEventKind::RoutingDeadlineMissed,
             "SystemEventKind values are distinct");

    return 0;
}
