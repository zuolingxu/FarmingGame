#include "PlayerSprite.h"
#include "MapLayer.h"

USING_NS_CC;
#ifdef _MSC_VER
#undef GetObject
#endif

static constexpr char frame_format_[] = "player-{}.png";
const std::vector<Vec<int>> PlayerSprite::directions_({ {0,1}, {0,-1}, {-1,0}, {1,0} });
std::string PlayerSprite::plist_name_;
std::vector<std::vector<int>> PlayerSprite::movements_(12,std::vector<int>());


PlayerSprite::PlayerSprite()
{

}

PlayerSprite::~PlayerSprite()
{
    if (repeat_action_) {
        repeat_action_->release();
    }
}

PlayerSprite* PlayerSprite::create()
{
    if (plist_name_.empty())
    {
        DocumentManager* manager = DocumentManager::getInstance();
        rapidjson::Document* doc = manager->getDocument(manager->getPath("player"));
        plist_name_ = (*doc)["plist"].GetString();
        MapLayer::loadPlist(plist_name_);
        for (auto& i: (*doc)["movement"].GetObject())
        {
            MOVEMENTS move_e;
            std::string name = i.name.GetString();
            if (name == "UP")
            {
                move_e = MOVEMENTS::UP;
            }
            else if (name == "DOWN")
            {
                move_e = MOVEMENTS::DOWN;
            }
            else if (name == "LEFT")
            {
                move_e = MOVEMENTS::LEFT;
            }
            else if (name == "RIGHT")
            {
                move_e = MOVEMENTS::RIGHT;
            }
            else if (name == "W_UP")
            {
                move_e = MOVEMENTS::W_UP;
            }
            else if (name == "W_DOWN")
            {
	            move_e = MOVEMENTS::W_DOWN;
            }
            else if (name == "W_LEFT")
            {
	            move_e = MOVEMENTS::W_LEFT;
            }
            else if (name == "W_RIGHT")
            {
                move_e = MOVEMENTS::W_RIGHT;
            }
            else if (name == "I_UP")
            {
	            move_e = MOVEMENTS::I_UP;
            }
            else if (name == "I_DOWN")
            {
	            move_e = MOVEMENTS::I_DOWN;
            }
            else if (name == "I_LEFT")
            {
	            move_e = MOVEMENTS::I_LEFT;
            }
            else if (name == "I_RIGHT")
            {
	            move_e = MOVEMENTS::I_RIGHT;
            }
            else
            {
	            move_e = MOVEMENTS::NONE;
            }
            
            if (move_e != MOVEMENTS::NONE)
            {
                const int move_i = static_cast<int>(move_e);
                auto it = movements_[move_i].begin();
                for (auto& value: i.value.GetArray())
                {
                    movements_[move_i].emplace_back(value.GetInt() - 1);
                }
            }
        }
    }

    PlayerSprite* sprite = new (std::nothrow) PlayerSprite();
    if (sprite && sprite->initWithSpriteFrameName(std::format(frame_format_, 0)))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);

    return nullptr;
}

void PlayerSprite::move(MOVEMENTS move_e)
{
    if (repeat_action_ != nullptr)
    {
	    stopAction(repeat_action_);
        repeat_action_->release();
        repeat_action_ = nullptr;
    }

    Animation* animation = Animation::create();
    animation->setDelayPerUnit(0.3f);
    for (int i : movements_[static_cast<int>(move_e)])
    {
    	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(std::format(frame_format_, i));
        animation->addSpriteFrame(frame);
    }
    stand_direction_ = static_cast<MOVEMENTS>(static_cast<int>(move_e) - 4);

    Animate* animate = Animate::create(animation);
    MoveBy* moveBy = MoveBy::create(0.6f, Vec2(directions_[static_cast<int>(move_e) - 4]) * 30);
    repeat_action_ = RepeatForever::create(Spawn::create(moveBy, animate, nullptr));
    repeat_action_->retain();
    runAction(repeat_action_);
}

void PlayerSprite::stop(MOVEMENTS move_e)
{
    if (repeat_action_ != nullptr && move_e == static_cast<MOVEMENTS>(static_cast<int>(stand_direction_) + 4))
    {
        stopAction(repeat_action_);
        setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(
            std::format(frame_format_, movements_[static_cast<int>(stand_direction_)][0])));
        repeat_action_->release();
        repeat_action_ = nullptr;
    }
}

void PlayerSprite::interact(MOVEMENTS move_e)
{
	
}

void PlayerSprite::changeSpeed()
{
	
}
