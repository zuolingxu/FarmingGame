#pragma once
#include "cocos2d.h"

class PlayerSprite : public cocos2d::Sprite
{
public:
    PlayerSprite();
    ~PlayerSprite();

    static PlayerSprite* create();

    bool initWithFile(const std::string& filename) override;

    // TODO: add more animation

    void startAnimation();
    void stopAnimation();

private:
    bool isAnimating;
    cocos2d::Animate* animation;
};

