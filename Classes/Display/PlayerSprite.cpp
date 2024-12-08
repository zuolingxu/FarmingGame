#include "PlayerSprite.h"
#include "MapLayer.h"

USING_NS_CC;
#ifdef _MSC_VER
#undef GetObject
#endif

static constexpr float SCALE_FACTOR = 1.5f;
static constexpr float FRAME_LENGTH_WALK = 0.3f; // 18 frames
static constexpr float FRAME_LENGTH_RUN = 0.2f; // 12 frames
static constexpr float FRAME_LENGTH_INTERACT = 0.08f; // 7 frames, 6 duration
static constexpr int SPEED_WALK = 12; // 1.8 pixel / frame
static constexpr int SPEED_RUN = 16; // 1.25 pixel / frame
static constexpr float DISPLACEMENT_WALK = SPEED_WALK / (FRAME_LENGTH_WALK * 60);
static constexpr float DISPLACEMENT_RUN = SPEED_RUN / (FRAME_LENGTH_RUN * 60);
const std::vector<Vec<int>> PlayerSprite::directions_({ {0,1}, {0,-1}, {-1,0}, {1,0} });

PlayerSprite::MOVEMENT PlayerSprite::getMovementFromString(const std::string& name)
{
    if (name == "UP")
    {
        return MOVEMENT::UP;
    }
    else if (name == "DOWN")
    {
        return MOVEMENT::DOWN;
    }
    else if (name == "LEFT")
    {
        return MOVEMENT::LEFT;
    }
    else if (name == "RIGHT")
    {
        return MOVEMENT::RIGHT;
    }
    else if (name == "W_UP")
    {
        return MOVEMENT::W_UP;
    }
    else if (name == "W_DOWN")
    {
        return MOVEMENT::W_DOWN;
    }
    else if (name == "W_LEFT")
    {
        return MOVEMENT::W_LEFT;
    }
    else if (name == "W_RIGHT")
    {
        return MOVEMENT::W_RIGHT;
    }
    else if (name == "I_UP")
    {
        return MOVEMENT::I_UP;
    }
    else if (name == "I_DOWN")
    {
        return MOVEMENT::I_DOWN;
    }
    else if (name == "I_LEFT")
    {
        return MOVEMENT::I_LEFT;
    }
    else if (name == "I_RIGHT")
    {
        return MOVEMENT::I_RIGHT;
    }
    else if (name == "STAY")
    {
	    return MOVEMENT::STAY;
    }
    else
    {
        return MOVEMENT::NONE;
    }
}

PlayerSprite::PlayerSprite(bool always_run, const rapidjson::Document* doc) : run_(always_run)
{
    plist_name_ = (*doc)["plist"].GetString();
    frame_format_ = (*doc)["frame_format"].GetString();
    MapLayer::loadPlist(plist_name_);
    for (auto& i : (*doc)["movement"].GetObject())
    {
        MOVEMENT move_e = getMovementFromString(i.name.GetString());
        if (move_e != MOVEMENT::NONE)
        {
            const int move_i = static_cast<int>(move_e);
            auto it = movements_[move_i].begin();
            for (auto& value : i.value.GetArray())
            {
                movements_[move_i].emplace_back(value.GetInt() - 1);
            }
        }
    }
}

PlayerSprite::~PlayerSprite()
{
    if (repeat_action_) {
        repeat_action_->release();
    }
}

PlayerSprite* PlayerSprite::create(const rapidjson::Document* doc, bool always_run)
{
    PlayerSprite* sprite = new PlayerSprite(always_run, doc);
    const std::string first_frame = getFrameName(sprite->frame_format_, sprite->movements_[static_cast<int>(MOVEMENT::DOWN)][0]);
    if (sprite && sprite->initWithSpriteFrameName(first_frame))
    {
        sprite->autorelease();
        sprite->setAnchorPoint(Vec2(0, 0));
        return sprite;
    }
    //�����ʼ��ʧ��
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void PlayerSprite::moveBy(MOVEMENT direction, int length)
{
	move(direction);
    destination = getPosition() + Vec2(directions_[static_cast<int>(direction) - 4]) * length * GridSize;
}


void PlayerSprite::move(MOVEMENT move_e)
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
    	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(getFrameName(frame_format_, i));
        animation->addSpriteFrame(frame);
    }
    stand_direction_ = static_cast<MOVEMENT>(static_cast<int>(move_e) - 4);

    Animate* animate = Animate::create(animation);
    MoveBy* moveBy = MoveBy::create(frame_duration * 2, Vec2(directions_[static_cast<int>(move_e) - 4]) * move_speed * 2);
    repeat_action_ = RepeatForever::create(Spawn::create(moveBy, animate, nullptr));
    repeat_action_->retain();
    runAction(repeat_action_);
    scheduleUpdate();
    is_moving = true;
}

void PlayerSprite::stop(MOVEMENT move_e)
{
    if (repeat_action_ != nullptr && (move_e == MOVEMENT::ALL 
        || move_e == static_cast<MOVEMENT>(static_cast<int>(stand_direction_) + 4)))
    {
        stopAction(repeat_action_);
        setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(
            getFrameName(frame_format_, movements_[static_cast<int>(stand_direction_)][0])));
        repeat_action_->release();
        repeat_action_ = nullptr;
        unscheduleUpdate();
        is_moving = false;
    }
}

void PlayerSprite::interact(Vec<int> pos)
{
    if (repeat_action_ != nullptr)
    {
        stopAction(repeat_action_);
        repeat_action_->release();
        repeat_action_ = nullptr;
        is_moving = false;
    }

	if (pos.Y() == -1)
	{
        stand_direction_ = MOVEMENT::DOWN;
	}
    else if (pos.X() == 1)
    {
	    stand_direction_ = MOVEMENT::RIGHT;
    }
    else if (pos.X() == -1)
    {
	    stand_direction_ = MOVEMENT::LEFT;
    }
    else if (pos.Y() == 1)
    {
	    stand_direction_ = MOVEMENT::UP;
    }

    Animation* animation = Animation::create();
    animation->setDelayPerUnit(FRAME_LENGTH_INTERACT);
    MOVEMENT move_e = static_cast<MOVEMENT>(static_cast<int>(stand_direction_) + 8);
    for (int i : movements_[static_cast<int>(move_e)])
    {
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(getFrameName(frame_format_, i));
        animation->addSpriteFrame(frame);
    }
    auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(getFrameName(frame_format_, 
        movements_[static_cast<int>(stand_direction_)][0]));
    animation->addSpriteFrame(frame);
    Animate* animate = Animate::create(animation);
    runAction(animate);
    parent_->layer_->getEventDispatcher()->pauseEventListenersForTarget(parent_->layer_);
    auto functionCallback = std::function<void(float)>([this](float dt) {
        parent_->layer_->getEventDispatcher()->resumeEventListenersForTarget(parent_->layer_);
        });
    schedule(functionCallback, 0, 0, 0.48f, "interact");
}

void PlayerSprite::changeSpeed()
{
	run_ = !run_;
    if (is_moving)
    {
        MOVEMENT move_e = static_cast<MOVEMENT>(static_cast<int>(stand_direction_) + 4);
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

    if (is_moving)
    {
        MOVEMENT move_e = static_cast<MOVEMENT>(static_cast<int>(stand_direction_) + 4);
        float displacement = run_ ? DISPLACEMENT_RUN : DISPLACEMENT_WALK;
        int dire = static_cast<int>(move_e) - 4;

        Vec2 next_pos = getPosition() + Vec2(directions_[dire]) * displacement;
        Vec2 next_pos1 = next_pos + Vec2(2, 0);
        Vec2 next_pos2 = next_pos + Vec2(GridSize - 2,0);

        if (parent_->hasCollision(next_pos1) || parent_->hasCollision(next_pos2)
            || (getPosition() - destination).length() < delta * 20)
        {
            destination = {-1,-1};
            stop(MOVEMENT::ALL);
        }
        setLocalZOrder(getPosition().y);
    }

    parent_->getCamera()->setPosition(getPosition()
        - Director::getInstance()->getWinSize() / SCALE_FACTOR / 2);
    parent_->refocus();
}
