#pragma once
#define CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL 1
#include <bitset>
#include "HelperClasses.h"
#include "cocos2d.h"
#include "MapLayer.h"

class MapLayer;

class PlayerSprite : public cocos2d::Sprite
{
public:
    enum class MOVEMENT : char
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        W_UP,
        W_DOWN,
        W_LEFT,
        W_RIGHT,
        I_UP,
        I_DOWN,
        I_LEFT,
        I_RIGHT,
        STAY,
        NONE,
        ALL
    };
    static MOVEMENT getMovementFromString(const std::string& name);

    PlayerSprite(bool always_run, const rapidjson::Document* doc);
    ~PlayerSprite() override;

    static PlayerSprite* create(const rapidjson::Document* doc, bool always_run = false);

    void moveBy(MOVEMENT direction, int length);
    void move(MOVEMENT move_e);
    void stop(MOVEMENT move_e);
    void interact(MOVEMENT move_e);
    void changeSpeed();
    virtual void update(float delta) override;
    void setParentMapLayer(MapLayer* parent);

private:

    const static std::vector<Vec<int>> directions_;

    std::vector<std::vector<int>> movements_ = std::vector(12, std::vector<int>());
    std::string plist_name_;
    std::string frame_format_;
    MapLayer* parent_ = nullptr;
    cocos2d::RepeatForever* repeat_action_ = nullptr;
    MOVEMENT stand_direction_ = MOVEMENT::DOWN;
    bool run_ = false;
    bool is_moving = false;
    cocos2d::Vec2 destination = {-1,-1};
};

