#include <psm/plant.hpp>

namespace psm {

namespace {
constexpr int kMaxDivertingWaitTicks = 1;
}  // namespace

void Plant::spawnItem(Item item) {
    if (item_.has_value()) {
        return;
    }
    item.zone = Zone::Infeed;
    item_ = item;
}

std::optional<SystemEventKind> Plant::advance(const Diverter& diverter, const BeltMotor& beltMotor) {
    if (!item_.has_value()) {
        return std::nullopt;
    }
    if (beltMotor.actualState() != BeltMotorState::Running) {
        return std::nullopt;  // belt not running -- nothing moves
    }

    switch (item_->zone) {
        case Zone::Infeed:
            item_->zone = Zone::PresenceCheck;
            return std::nullopt;
        case Zone::PresenceCheck:
            item_->zone = Zone::Weighing;
            return std::nullopt;
        case Zone::Weighing:
            item_->zone = Zone::Diverting;
            divertingWaitTicks_ = 0;
            return std::nullopt;
        case Zone::Diverting: {
            if (!diverter.isSettled()) {
                ++divertingWaitTicks_;
                if (divertingWaitTicks_ > kMaxDivertingWaitTicks) {
                    return SystemEventKind::RoutingDeadlineMissed;
                }
                return SystemEventKind::DiverterNotReady;
            }
            const DiverterPosition position = diverter.actualPosition();
            item_->zone = (position == DiverterPosition::Straight) ? Zone::OutputLight : Zone::OutputHeavy;
            divertingWaitTicks_ = 0;
            return std::nullopt;
        }
        case Zone::OutputLight:
        case Zone::OutputHeavy:
            item_.reset();
            return std::nullopt;
    }
    return std::nullopt;
}

const std::optional<Item>& Plant::currentItem() const {
    return item_;
}

}  // namespace psm
