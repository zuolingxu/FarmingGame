#include "MapLayer.h"
#include "json/reader.h"
#include "Object.h"
#include <functional>

USING_NS_CC;
#ifdef _MSC_VER
#undef GetObject
#endif

MapLayer::MapLayer(const std::string& tmx_path, 
    rapidjson::Value* const_object, rapidjson::Value* archive_object) 
{
    DocumentManager* manager = DocumentManager::getInstance();
    if (manager->hasDocument(tmx_path))
    {
	    tmx_name_ = manager->getPath(tmx_path);
    }
    else
    {
	    tmx_name_ = tmx_path;
    }

    if (!tmx_name_.empty())
    {
        tiled_map_ = TMXTiledMap::create(tmx_name_);
        tiled_map_->retain();
        Vec<int> size = tiled_map_->getMapSize();
        interact_map_ = std::vector(size.X(), std::vector<::Object*>(size.Y(), nullptr));
        collision_map_ = std::vector(size.X(), std::vector<bool>(size.Y(), false));
        addCollisions(); 
        tiled_map_->release();
    }

    if (const_object != nullptr)
    {
        for (auto& object : const_object->GetObject())
        {
            addObject(toVec2(object.name.GetString()), object.value);
        }
    }

    if (archive_object != nullptr)
    {
        for (auto& object : archive_object->GetObject())
        {
            addObject(toVec2(object.name.GetString()), object.value);
        }
    }
}

MapLayer* MapLayer::createWithDocument(const std::string& tmx_path, rapidjson::Value* const_object, rapidjson::Value* archive_object)
{
    MapLayer* map = new(std::nothrow) MapLayer(tmx_path, const_object, archive_object);
    if (map)
    {
        map->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(map);
    }
    return map;
}

void MapLayer::addTiledMap()
{
    if (!tmx_name_.empty())
    {
        tiled_map_ = TMXTiledMap::create(tmx_name_);
        tiled_map_->setTileAnimEnabled(true);
        layer_->addChild(tiled_map_, -256);
    }
}

void MapLayer::addObject(Vec<int> pos, rapidjson::Value& val)
{
    focus_pos_ = std::move(pos);
    ::Object* obj = ::Object::create(val, this);
    obj->retain();
    interact_map_[focus_pos_.X()][focus_pos_.Y()] = obj;
}

void MapLayer::addCollisions()
{
    const cocos2d::TMXObjectGroup* objectGroup = tiled_map_->getObjectGroup("collision");

    if (objectGroup != nullptr) 
    {
        const ValueVector& objects = objectGroup->getObjects();
        for (const Value& obj : objects) {
            ValueMap dict = obj.asValueMap();
            const float x1 = dict.at("x").asFloat();
            const float x2 = x1 + dict.at("width").asFloat();
            const float y1 = dict.at("y").asFloat();
            const float y2 = y1 + dict.at("height").asFloat();
            Vec<int> left_bottom = toGrid({x1,y1});
            Vec<int> right_top = toGrid({x2,y2});
            for (int x = left_bottom.X(); x <= right_top.X(); x++)
            {
                for (int y = right_top.Y(); y <= right_top.Y(); y++)
                {
                    collision_map_[x][y] = true;
                }
            }
        }
    }
    else {
        CCLOG("Object layer not found!");
    }
}

void MapLayer::addEventListener()
{
    // Touch Listener
    touch_listener_ = cocos2d::EventListenerTouchAllAtOnce::create();
    touch_listener_->onTouchesBegan = std::bind(&MapLayer::onTouchesBegan, this, std::placeholders::_1, std::placeholders::_2);
    touch_listener_->onTouchesMoved = std::bind(&MapLayer::onTouchesMoved, this, std::placeholders::_1, std::placeholders::_2);
    touch_listener_->onTouchesEnded = std::bind(&MapLayer::onTouchesEnded, this, std::placeholders::_1, std::placeholders::_2);

    // Keyboard Listener
    keyboard_Listener_ = cocos2d::EventListenerKeyboard::create();
    keyboard_Listener_->onKeyPressed = std::bind(&MapLayer::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
    keyboard_Listener_->onKeyReleased = std::bind(&MapLayer::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2);

    // Mouse Listener
    mouse_listener_ = cocos2d::EventListenerMouse::create();
    mouse_listener_->onMouseDown = std::bind(&MapLayer::onMouseDown, this, std::placeholders::_1);
    mouse_listener_->onMouseMove = std::bind(&MapLayer::onMouseMove, this, std::placeholders::_1);
    mouse_listener_->onMouseUp = std::bind(&MapLayer::onMouseUp, this, std::placeholders::_1);

    // add Listener to Layer
    layer_->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener_, layer_);
    layer_->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboard_Listener_, layer_);
    layer_->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse_listener_, layer_);
}

void MapLayer::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
    // Multi-platform
    event->stopPropagation();
}

void MapLayer::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
    // Multi-platform
    event->stopPropagation(); 
}

void MapLayer::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
    // Multi-platform
    event->stopPropagation(); 
}

void MapLayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
        // main_player_->move(Vec<int>(0, 1));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_S:
        // main_player_->move(Vec<int>(0, -1));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
        // main_player_->move(Vec<int>(-1, 0));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_D:
        // main_player_->changePosition(Vec<int>(1, 0));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_1:
    case cocos2d::EventKeyboard::KeyCode::KEY_2:
    case cocos2d::EventKeyboard::KeyCode::KEY_3:
    case cocos2d::EventKeyboard::KeyCode::KEY_4:
    case cocos2d::EventKeyboard::KeyCode::KEY_5:
    case cocos2d::EventKeyboard::KeyCode::KEY_6:
    case cocos2d::EventKeyboard::KeyCode::KEY_7:
    case cocos2d::EventKeyboard::KeyCode::KEY_8:
    case cocos2d::EventKeyboard::KeyCode::KEY_9:
        changeHolding(1 + static_cast<int>(keyCode) -
            static_cast<int>(cocos2d::EventKeyboard::KeyCode::KEY_1));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_0:
        changeHolding(10);
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_MINUS:
        changeHolding(11);
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_EQUAL:
        changeHolding(12);
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_SHIFT:
            // main_player_->changeSpeed();
    default: break;
    }
    event->stopPropagation();
}

void MapLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
        // main_player -> stop(Vec<int>(0, 1));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_S:
        // main_player -> stop(Vec<int>(0, -1));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
        // main_player -> stop(Vec<int>(-1, 0));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_D:
        // main_player -> stop(Vec<int>(1, 0));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_SHIFT:
        // main_player_->changeSpeed();
        break;
    default: break;
    }

    event->stopPropagation(); 
}

void MapLayer::onMouseDown(cocos2d::Event* event)
{
    cocos2d::EventMouse* e = dynamic_cast<cocos2d::EventMouse*>(event);

    if (e)
    {
        if (e->getMouseButton() == cocos2d::EventMouse::MouseButton::BUTTON_LEFT) 
        {
            ::Object* focus = interact_map_[focus_pos_.X()][focus_pos_.Y()];
            if (focus != nullptr)
            {
                focus->interact();
            }
        }
        else if (e->getMouseButton() == cocos2d::EventMouse::MouseButton::BUTTON_RIGHT) 
        {
            // TODO: interact with holdings
        }
    }
    event->stopPropagation(); 
}

void MapLayer::onMouseMove(cocos2d::Event* event)
{
    cocos2d::EventMouse* e = dynamic_cast<cocos2d::EventMouse*>(event);
    if (e)
    {
        cocos2d::Vec2 mousePosition = e->getLocation();
        Vec<int> gird_pos = toGrid(camera_->getPosition() / 4.0f);

        // TODO: calculate grid position

        CCLOG("Êó±êÎ»ÖÃ: (%f, %f)", mousePosition.x, mousePosition.y);
    }
    event->stopPropagation();
}

void MapLayer::onMouseUp(cocos2d::Event* event)
{
    // nothing to do
    event->stopPropagation();
}

void MapLayer::changeFocus()
{
	// TODO: change focus
}

void MapLayer::changePosition(const Vec<int>& direction)
{
	// TODO: main_player_->move(direction);
}

void MapLayer::changeHolding(const int num)
{
	// UILogic::changeHoldings(num);
}


Node* MapLayer::toFront(PlayerSprite* main_player)
{
    layer_ = Node::create();
    layer_->retain();
    addTiledMap();

    Size map_size = tiled_map_->getMapSize();
    Size tiled_size = tiled_map_->getTileSize();
    Vec2 pixel_pos = { map_size.width * tiled_size.width / 2, map_size.height * tiled_size.height / 2 };
    if (main_player != nullptr)
    {
	    pixel_pos = main_player->getPosition();
        main_player_ = main_player;
    }

    // bind camera
    camera_ = Camera::create();
    camera_->setCameraFlag(CameraFlag::USER1);
    camera_->setDepth(10);
    // camera_->setScale(main_player == nullptr ? 2.0f : 4.0f);
    camera_->setPosition(pixel_pos);

    layer_->addChild(camera_);

    is_front_ = true;
    for (auto& row : interact_map_)
    {
	    for (::Object* object: row)
	    {
		    if (object != nullptr)
		    {
			    object->init();
		    }
	    }
    }

    addEventListener();
    layer_->setCameraMask(static_cast<unsigned short>(CameraFlag::USER1));
    return layer_;
}

void MapLayer::pause() const
{
    layer_->pause();
}

void MapLayer::resume() const
{
	layer_->resume();
}

void MapLayer::toBack()
{
    is_front_ = false;
    SpriteFrameCache::getInstance()->removeSpriteFrames();
}

void MapLayer::settle() const
{
	for (auto& row : interact_map_)
	{
		for (::Object* object : row)
		{
			if (object != nullptr)
			{
				object->settle();
			}
		}
	}
}

void MapLayer::loadPlist(std::string plist_name)
{
    DocumentManager* manager = DocumentManager::getInstance();
    if (manager->hasDocument(plist_name))
    {
        plist_name = manager->getPath(plist_name);
    }
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist_name);
}

void MapLayer::addPlayerSprite(PlayerSprite* player)
{
    // leave for network
}

void MapLayer::clearObjects()
{
    for (auto& row : interact_map_)
    {
	    for (::Object* object : row)
	    {
            object->clear();
            object->release();
	    }
    }
}


Sprite* MapLayer::addSpriteWithFrame(const std::string& frame_name) const
{
    if (is_front_)
    {
        SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frame_name);
        if (spriteFrame != nullptr) 
        {
            Sprite* sprite = Sprite::createWithSpriteFrame(spriteFrame);
            sprite->setPosition(toPixel(focus_pos_));
            sprite->setAnchorPoint(Vec2(0, 0));
            layer_->addChild(sprite, focus_pos_.Y());
            return sprite;
        }
    }
	return nullptr;
}

void MapLayer::changeWithActionSequence(std::vector<int> sequence)
{
	if (is_front_)
	{
		
	}
}

void MapLayer::changeWithSingleFrame(int num)
{
	if (is_front_)
	{
		
	}
}
