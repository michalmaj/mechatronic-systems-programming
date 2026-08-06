#include <psm/plant.hpp>

namespace psm {

void Plant::spawnItem(Item item) {
    if (item_.has_value()) {
        return;
    }
    item.zone = Zone::Infeed;
    item_ = item;
}

void Plant::advance(DiverterPosition diverter) {
    if (!item_.has_value()) {
        return;
    }

    switch (item_->zone) {
        case Zone::Infeed:
            item_->zone = Zone::PresenceCheck;
            break;
        case Zone::PresenceCheck:
            item_->zone = Zone::Weighing;
            break;
        case Zone::Weighing:
            item_->zone = Zone::Diverting;
            break;
        case Zone::Diverting:
            item_->zone = (diverter == DiverterPosition::Straight) ? Zone::OutputLight : Zone::OutputHeavy;
            break;
        case Zone::OutputLight:
        case Zone::OutputHeavy:
            item_.reset();
            break;
    }
}

const std::optional<Item>& Plant::currentItem() const {
    return item_;
}

}  // namespace psm
