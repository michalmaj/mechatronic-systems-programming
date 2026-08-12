#include <iostream>
#include <vector>

#include <psm/engine.hpp>
#include <psm/item.hpp>
#include <psm/tick_result.hpp>

int main() {
    std::cout << "-- Multiple parcels in flight: Light/Heavy/Light --\n";

    psm::Engine engine;
    engine.requestStart();

    struct Pending {
        psm::ItemId id;
        psm::Grams mass;
    };
    std::vector<Pending> toSpawn{{1, 100}, {2, 800}, {3, 150}};
    std::size_t nextToSpawn = 0;
    int departed = 0;

    for (int tick = 0; tick < 30 && departed < 3; ++tick) {
        if (nextToSpawn < toSpawn.size()) {
            const auto& next = toSpawn[nextToSpawn];
            if (engine.spawnItem(next.id, next.mass)) {
                ++nextToSpawn;
            }
        }
        auto result = engine.step();
        std::cout << psm::describe(result) << '\n';
        if (result.departure.has_value()) {
            ++departed;
        }
    }

    return 0;
}
