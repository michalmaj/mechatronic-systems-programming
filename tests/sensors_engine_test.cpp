#include "support/check.hpp"
#include <psm/engine.hpp>

int main() {
    // -- Scenario 1: no fault ever injected -- the parcel is eventually classified and routed.
    {
        psm::Engine engine;
        engine.spawnItem(psm::Item{1, psm::Zone::Infeed, 750});
        engine.requestStart();

        bool reachedOutputHeavy = false;
        for (int i = 0; i < 10 && !reachedOutputHeavy; ++i) {
            auto result = engine.step();
            if (result.item.has_value() && result.item->zone == psm::Zone::OutputHeavy) {
                reachedOutputHeavy = true;
            }
        }
        psmCheck(reachedOutputHeavy, "with no fault, a 750g parcel eventually reaches OutputHeavy");
    }

    // -- Scenario 2: the weight sensor is faulted (Stale, from before the parcel ever reaches
    // Weighing) throughout the run. Classification never succeeds for this parcel -- it reaches
    // Diverting and waits there, frozen, never routed.
    {
        psm::Engine engine;
        engine.spawnItem(psm::Item{1, psm::Zone::Infeed, 750});
        engine.requestStart();
        engine.injectFault(psm::FaultTarget::Weight, psm::FaultKind::Stale);

        psm::Zone lastZone = psm::Zone::Infeed;
        for (int i = 0; i < 8; ++i) {
            auto result = engine.step();
            if (result.item.has_value()) {
                lastZone = result.item->zone;
            }
        }
        psmCheck(lastZone == psm::Zone::Diverting,
                 "with the weight sensor faulted the whole time, the parcel reaches Diverting and waits "
                 "there -- never routed, because no trusted classification was ever produced for it");
    }

    return 0;
}
