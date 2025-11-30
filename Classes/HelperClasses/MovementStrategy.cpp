#include "MovementStrategy.h"

using namespace cocos2d;

namespace MovementStrategies {

void AnimalPatrolStrategy::apply(PlayerSprite* sprite, const MapObject::ObjectInfo& info) {
    if (!sprite) return;

    const int startY = info.position.Y();

    sprite->schedule([sprite, startY](float /*deltaTime*/) {
        sprite->move(PlayerSprite::MOVEMENT::W_DOWN, startY - 2);

        sprite->scheduleOnce([sprite, startY](float /*dt*/) {
            sprite->move(PlayerSprite::MOVEMENT::W_UP, 5 - startY);
        }, 3.0f, "up_move_key");

    }, 6.0f, "down_move_key");
}

void HaleyPatrolStrategy::apply(PlayerSprite* sprite, const MapObject::ObjectInfo& /*info*/) {
    if (!sprite) return;

    sprite->schedule([sprite, fn = isPausedFn_](float /*deltaTime*/) {
        if (fn && fn()) {
            return;
        }

        sprite->move(PlayerSprite::MOVEMENT::W_LEFT, 6);

        sprite->scheduleOnce([sprite, fn](float /*dt*/) {
            if (fn && fn()) {
                return;
            }
            sprite->move(PlayerSprite::MOVEMENT::W_RIGHT, 6);
        }, 4.0f, "right_move_key");

    }, 11.0f, "left_move_key");
}

void CarolinePatrolStrategy::apply(PlayerSprite* sprite, const MapObject::ObjectInfo& /*info*/) {
    if (!sprite) return;

    sprite->move(PlayerSprite::MOVEMENT::W_DOWN, 3);
    sprite->schedule([sprite](float /*deltaTime*/) {
        sprite->move(PlayerSprite::MOVEMENT::W_UP, 2);

        sprite->scheduleOnce([sprite](float /*dt*/) {
            sprite->move(PlayerSprite::MOVEMENT::W_DOWN, 2);
        }, 4.0f, "down_move_key");

    }, 8.0f, "up_move_key");
}

std::unique_ptr<MovementStrategy> CreateNPCStrategy(
    const std::string& npcName,
    std::function<bool()> isPausedFn) {
    if (npcName == "Haley") {
        return std::make_unique<HaleyPatrolStrategy>(std::move(isPausedFn));
    }
    if (npcName == "Caroline") {
        return std::make_unique<CarolinePatrolStrategy>();
    }
    return nullptr;
}

} // namespace MovementStrategies
