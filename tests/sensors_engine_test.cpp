#include "support/check.hpp"
#include <psm/engine.hpp>

int main() {
    // -- Scenario 1: no fault ever injected -- the parcel is eventually classified and routed.
    {
        psm::Engine engine;
        engine.spawnItem(1, 750);
        engine.requestStart();

        bool departed = false;
        for (int i = 0; i < 10 && !departed; ++i) {
            auto result = engine.step();
            if (result.departure.has_value() && result.departure->id == 1) {
                departed = true;
            }
        }
        psmCheck(departed, "with no fault, a 750g parcel eventually departs, routed to OutputHeavy");
    }

    // -- Scenario 2: the weight sensor is faulted (Stale, from before the parcel ever reaches
    // Weighing) throughout the run. Classification never succeeds for this parcel -- it reaches
    // diverting and waits there, frozen, never routed.
    {
        psm::Engine engine;
        engine.spawnItem(1, 750);
        engine.requestStart();
        engine.injectSensorFault(psm::SensorTarget::Weight, psm::SensorFaultKind::Stale);

        bool stillWaitingAtEnd = false;
        for (int i = 0; i < 8; ++i) {
            auto result = engine.step();
            stillWaitingAtEnd = result.diverting.has_value() && result.diverting->id == 1;
        }
        psmCheck(stillWaitingAtEnd,
                 "with the weight sensor faulted the whole time, the parcel reaches diverting and waits "
                 "there -- never routed, because no trusted classification was ever produced for it");
    }

    return 0;
}
