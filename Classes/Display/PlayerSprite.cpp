#include "PlayerSprite.h"

USING_NS_CC;

PlayerSprite::PlayerSprite()
    : isAnimating(false), animation(nullptr)
{

}

PlayerSprite::~PlayerSprite()
{
    if (animation) {
        animation->release();
    }
}

PlayerSprite* PlayerSprite::create()
{
    PlayerSprite* sprite = new (std::nothrow) PlayerSprite();
    //精灵初始化成功
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    //精灵初始化失败
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool PlayerSprite::initWithFile(const std::string& filename)
{
    if (!Sprite::initWithFile(filename)) {
        return false;
    }

    isAnimating = false;

    return true;
}

void PlayerSprite::startAnimation()
{
    if (!isAnimating) {
        Vector<SpriteFrame*> frames;
        for (int i = 1; i <= 5; ++i) {
            auto frame = SpriteFrame::create("frame_" + std::to_string(i) + ".png", Rect(0, 0, 64, 64));
            frames.pushBack(frame);
        }

        animation = Animate::create(Animation::createWithSpriteFrames(frames, 0.1f));
        animation->retain();

        runAction(RepeatForever::create(animation));
        isAnimating = true;
    }
}

void PlayerSprite::stopAnimation()
{
    if (isAnimating) {
        stopAllActions();
        isAnimating = false;
    }
}

