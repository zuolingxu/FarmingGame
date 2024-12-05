#pragma once
#include <bitset>
#include "HelperClasses.h"
#include "cocos2d.h"

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
        NONE
    };

    PlayerSprite();
    ~PlayerSprite() override;

    static PlayerSprite* create();

    void move(MOVEMENTS move_e);
    void stop(MOVEMENTS move_e);
    void interact(MOVEMENTS move_e);
    void changeSpeed();

private:
    static std::string plist_name_;
    static std::vector<std::vector<int>> movements_;
    const static std::vector<Vec<int>> directions_;

    cocos2d::RepeatForever* repeat_action_ = nullptr;
    MOVEMENTS stand_direction_ = MOVEMENTS::DOWN;
    bool fast_ = false;
};

