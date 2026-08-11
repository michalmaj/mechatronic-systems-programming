#include "support/check.hpp"
#include <psm/engine.hpp>

#include <cstddef>
#include <vector>

int main() {
    using psm::Engine;
    using psm::Grams;
    using psm::ItemDeparture;
    using psm::ItemId;

    Engine engine;
    engine.requestStart();

    struct Pending {
        ItemId id;
        Grams mass;
    };
    std::vector<Pending> toSpawn{{1, 100}, {2, 800}, {3, 150}};
    std::size_t nextToSpawn = 0;
    std::vector<ItemDeparture> departures;

    for (int tick = 0; tick < 60 && departures.size() < toSpawn.size(); ++tick) {
        if (nextToSpawn < toSpawn.size()) {
            const auto& next = toSpawn[nextToSpawn];
            if (engine.spawnItem(next.id, next.mass)) {
                ++nextToSpawn;
            }
        }
        auto result = engine.step();
        if (result.departure.has_value()) {
            departures.push_back(*result.departure);
        }
    }

    psmCheck(departures.size() == 3, "all three parcels eventually depart");
    if (departures.size() == 3) {
        psmCheck(departures[0].id == 1 && departures[0].destination == psm::Zone::OutputLight,
                 "parcel 1 (100g) departs first, correctly classified Light");
        psmCheck(departures[1].id == 2 && departures[1].destination == psm::Zone::OutputHeavy,
                 "parcel 2 (800g) departs second, correctly classified Heavy -- not sharing parcel 1's "
                 "classification");
        psmCheck(departures[2].id == 3 && departures[2].destination == psm::Zone::OutputLight,
                 "parcel 3 (150g) departs third, correctly classified Light -- not sharing parcel 2's "
                 "classification");
    }

    return 0;
}
