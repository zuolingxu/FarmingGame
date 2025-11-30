#pragma once
#include <memory>
#include <functional>
#include <string>
#include "cocos2d.h"
#include "PlayerSprite.h"
#include "MapObject.h"

namespace MovementStrategies {

// 基础移动策略接口
class MovementStrategy {
public:
    virtual ~MovementStrategy() = default;
    virtual void apply(PlayerSprite* sprite, const MapObject::ObjectInfo& info) = 0;
};

// 动物的巡逻策略（替代 Animal::patrolPath）
class AnimalPatrolStrategy final : public MovementStrategy {
public:
    void apply(PlayerSprite* sprite, const MapObject::ObjectInfo& info) override;
};

// Haley 的来回移动策略（带暂停判断）
class HaleyPatrolStrategy final : public MovementStrategy {
public:
    explicit HaleyPatrolStrategy(std::function<bool()> isPausedFn)
        : isPausedFn_(std::move(isPausedFn)) {}

    void apply(PlayerSprite* sprite, const MapObject::ObjectInfo& info) override;

private:
    std::function<bool()> isPausedFn_;
};

// Caroline 的上下移动策略
class CarolinePatrolStrategy final : public MovementStrategy {
public:
    void apply(PlayerSprite* sprite, const MapObject::ObjectInfo& info) override;
};

// 简单工厂：根据 NPC 名称生成策略
std::unique_ptr<MovementStrategy> CreateNPCStrategy(
    const std::string& npcName,
    std::function<bool()> isPausedFn);

} // namespace MovementStrategies
