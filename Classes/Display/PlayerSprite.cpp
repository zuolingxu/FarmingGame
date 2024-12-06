#include "PlayerSprite.h"
#include "MapLayer.h"

USING_NS_CC;
#ifdef _MSC_VER
#undef GetObject
#endif

static constexpr float FRAME_LENGTH_WALK = 0.3f; // 18 frames
static constexpr float FRAME_LENGTH_RUN = 0.2f; // 12 frames
static constexpr int SPEED_WALK = 10; // 1.8 pixel / frame
static constexpr int SPEED_RUN = 15; // 1.25 pixel / frame
static constexpr float DISPLACEMENT_WALK = 1.8;
static constexpr float DISPLACEMENT_RUN = 1.25;
static constexpr char frame_format_[] = "player-{}.png";
const std::vector<Vec<int>> PlayerSprite::directions_({ {0,1}, {0,-1}, {-1,0}, {1,0} });
std::string PlayerSprite::plist_name_;
std::vector<std::vector<int>> PlayerSprite::movements_(12,std::vector<int>());

PlayerSprite::MOVEMENTS PlayerSprite::getMovementFromString(const std::string& name)
{
    if (name == "UP")
    {
        return MOVEMENTS::UP;
    }
    else if (name == "DOWN")
    {
        return MOVEMENTS::DOWN;
    }
    else if (name == "LEFT")
    {
        return MOVEMENTS::LEFT;
    }
    else if (name == "RIGHT")
    {
        return MOVEMENTS::RIGHT;
    }
    else if (name == "W_UP")
    {
        return MOVEMENTS::W_UP;
    }
    else if (name == "W_DOWN")
    {
        return MOVEMENTS::W_DOWN;
    }
    else if (name == "W_LEFT")
    {
        return MOVEMENTS::W_LEFT;
    }
    else if (name == "W_RIGHT")
    {
        return MOVEMENTS::W_RIGHT;
    }
    else if (name == "I_UP")
    {
        return MOVEMENTS::I_UP;
    }
    else if (name == "I_DOWN")
    {
        return MOVEMENTS::I_DOWN;
    }
    else if (name == "I_LEFT")
    {
        return MOVEMENTS::I_LEFT;
    }
    else if (name == "I_RIGHT")
    {
        return MOVEMENTS::I_RIGHT;
    }
    else
    {
        return MOVEMENTS::NONE;
    }
}

PlayerSprite::~PlayerSprite()
{
    if (repeat_action_) {
        repeat_action_->release();
    }
}

PlayerSprite* PlayerSprite::create(bool always_run)
{
    if (plist_name_.empty())
    {
        DocumentManager* manager = DocumentManager::getInstance();
        rapidjson::Document* doc = manager->getDocument(manager->getPath("player"));
        plist_name_ = (*doc)["plist"].GetString();
        MapLayer::loadPlist(plist_name_);
        for (auto& i: (*doc)["movement"].GetObject())
        {
            MOVEMENTS move_e = getMovementFromString(i.name.GetString());
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

    PlayerSprite* sprite = new (std::nothrow) PlayerSprite(always_run);
    if (sprite && sprite->initWithSpriteFrameName(std::format(frame_format_, 0)))
    {
        sprite->autorelease();
        sprite->setAnchorPoint(Vec2(0.5, 0));
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
    float frame_duration = run_ ? FRAME_LENGTH_RUN : FRAME_LENGTH_WALK;
    int move_speed = run_ ? SPEED_RUN : SPEED_WALK;
    animation->setDelayPerUnit(frame_duration);
    for (int i : movements_[static_cast<int>(move_e)])
    {
    	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(std::format(frame_format_, i));
        animation->addSpriteFrame(frame);
    }
    stand_direction_ = static_cast<MOVEMENTS>(static_cast<int>(move_e) - 4);

    Animate* animate = Animate::create(animation);
    MoveBy* moveBy = MoveBy::create(frame_duration * 2, Vec2(directions_[static_cast<int>(move_e) - 4]) * move_speed * 2);
    repeat_action_ = RepeatForever::create(Spawn::create(moveBy, animate, nullptr));
    repeat_action_->retain();
    runAction(repeat_action_);
    scheduleUpdate();
    is_moving_ = true;
}

void PlayerSprite::stop(MOVEMENTS move_e)
{
    if (repeat_action_ != nullptr && (move_e == MOVEMENTS::EVERY 
        || move_e == static_cast<MOVEMENTS>(static_cast<int>(stand_direction_) + 4)))
    {
        stopAction(repeat_action_);
        setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(
            std::format(frame_format_, movements_[static_cast<int>(stand_direction_)][0])));
        repeat_action_->release();
        repeat_action_ = nullptr;
        unscheduleUpdate();
        is_moving_ = false;
    }
}

void PlayerSprite::interact(MOVEMENTS move_e)
{
	
}

void PlayerSprite::changeSpeed()
{
	run_ = !run_;
    if (is_moving_)
    {
        MOVEMENTS move_e = static_cast<MOVEMENTS>(static_cast<int>(stand_direction_) + 4);
        move(move_e);
    }
}

void PlayerSprite::setParentMapLayer(MapLayer* parent)
{
	parent_ = parent;
}

void PlayerSprite::update(float delta)
{
	Node::update(delta);
    MOVEMENTS move_e = static_cast<MOVEMENTS>(static_cast<int>(stand_direction_) + 4);
    float displacement = run_ ? DISPLACEMENT_RUN : DISPLACEMENT_WALK;
    int dire = static_cast<int>(move_e) - 4;


    if (dire == static_cast<int>(MOVEMENTS::LEFT) || dire == static_cast<int>(MOVEMENTS::RIGHT))
    {
        displacement += GridSize / 2;
    }

    Vec2 next_pos = getPosition() + Vec2(directions_[dire]) * displacement;
    
    if (parent_->hasCollision(next_pos))
    {
	    stop(MOVEMENTS::EVERY);
    }
    setLocalZOrder(getPosition().y);
    parent_->getCamera()->setPosition(getPosition() + Vec2(GridSize / 2, GridSize)
        - Director::getInstance()->getWinSize() / ScaleFactor / 2);
}
