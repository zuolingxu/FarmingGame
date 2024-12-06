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
    enum class MOVEMENTS : char
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
        NONE,
        EVERY
    };
    static MOVEMENTS getMovementFromString(const std::string& name);

    PlayerSprite(bool always_run) : run_(always_run) {};
    ~PlayerSprite() override;

    static PlayerSprite* create(bool always_run);

    void move(MOVEMENTS move_e);
    void stop(MOVEMENTS move_e);
    void interact(MOVEMENTS move_e);
    void changeSpeed();
    virtual void update(float delta) override;
    void setParentMapLayer(MapLayer* parent);

private:
    static std::string plist_name_;
    static std::vector<std::vector<int>> movements_;
    const static std::vector<Vec<int>> directions_;

    MapLayer* parent_ = nullptr;
    cocos2d::RepeatForever* repeat_action_ = nullptr;
    MOVEMENTS stand_direction_ = MOVEMENTS::DOWN;
    bool run_ = false;
    bool is_moving_ = false;
};

