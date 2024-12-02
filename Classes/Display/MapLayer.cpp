#include "MapLayer.h"
#include "json/reader.h"
#include "Object.h"

USING_NS_CC;
#ifdef _MSC_VER
#undef GetObject
#endif

MapLayer::MapLayer(const Vec<int>& size ,std::string tmx_path, 
    rapidjson::Value* const_object, rapidjson::Value* archive_object) :
    tmx_name_(std::move(tmx_path)), interact_map_(size.X(), std::vector<Object*>(size.Y(), nullptr)),
    collision_map_(size.X(), std::vector<bool>(size.Y(), false))
{
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

MapLayer* MapLayer::createWithDocument(const Vec<int>& size, const std::string& tmx_path, rapidjson::Value* const_object, rapidjson::Value* archive_object)
{
    return new MapLayer(size, tmx_path, const_object, archive_object);
}

void MapLayer::addTiledMap() const
{
    if (tiled_map_ != nullptr)
    {
        TMXTiledMap* tmx = TMXTiledMap::create(tmx_name_);
        layer_->addChild(tmx, -256);
    }
}

void MapLayer::addObject(Vec<int> pos, rapidjson::Value& val)
{
    add_pos_ = std::move(pos);
    interact_map_[add_pos_.X()][add_pos_.Y()] = Object::create(val, this);
}


void MapLayer::addEventListener()
{
    // Touch Listener
    touch_listener_ = cocos2d::EventListenerTouchAllAtOnce::create();
    touch_listener_->onTouchesBegan = CC_CALLBACK_2(onTouchesBegan, this);
    touch_listener_->onTouchesMoved = CC_CALLBACK_2(onTouchesMoved, this);
    touch_listener_->onTouchesEnded = CC_CALLBACK_2(onTouchesEnded, this);

    // Keyboard Listener
    keyboard_Listener_ = cocos2d::EventListenerKeyboard::create();
    keyboard_Listener_->onKeyPressed = CC_CALLBACK_2(onKeyPressed, this);
    keyboard_Listener_->onKeyReleased = CC_CALLBACK_2(onKeyReleased, this);

    // Mouse Listener
    mouse_listener_ = cocos2d::EventListenerMouse::create();
    mouse_listener_->onMouseDown = CC_CALLBACK_1(onMouseDown, this);
    mouse_listener_->onMouseMove = CC_CALLBACK_1(onMouseMove, this);
    mouse_listener_->onMouseUp = CC_CALLBACK_1(onMouseUp, this);

    // add Listener to Layer
    layer_->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener_, layer_);
    layer_->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboard_Listener_, layer_);
    layer_->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse_listener_, layer_);
}

void MapLayer::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
    CCLOG("Touches began!");
    event->stopPropagation();
}

void MapLayer::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
    CCLOG("Touches moved!");
    event->stopPropagation(); 
}

void MapLayer::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
    CCLOG("Touches ended!");
    event->stopPropagation(); 
}

void MapLayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    CCLOG("Key pressed: %d", keyCode);
    event->stopPropagation();
}

void MapLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    CCLOG("Key released: %d", keyCode);
    event->stopPropagation(); 
}

void MapLayer::onMouseDown(cocos2d::Event* event)
{
    CCLOG("Mouse button pressed!");
    event->stopPropagation(); 
}

void MapLayer::onMouseMove(cocos2d::Event* event)
{
    CCLOG("Mouse moved!");
    event->stopPropagation();
}

void MapLayer::onMouseUp(cocos2d::Event* event)
{
    CCLOG("Mouse button released!");
    event->stopPropagation();
}


void MapLayer::toFront(Vec<int> pos)
{
    layer_ = Layer::create();

    // create player sprite
    player_ = getSpriteWithFrame("");
    pos = toPixel(pos);

    // bind camera
    camera_ = Camera::create();
    camera_->setCameraFlag(CameraFlag::USER1);
    camera_->setDepth(0);
    camera_->setScale(4.0f);

    camera_->setPosition(static_cast<float>(pos.X()), static_cast<float>(pos.Y()));

    layer_->addChild(camera_);
    layer_->setCameraMask(static_cast<unsigned short>(CameraFlag::USER1));

    addTiledMap();
    is_front_ = true;
    for (auto& row : interact_map_)
    {
	    for (Object* object: row)
	    {
		    if (object != nullptr)
		    {
			    object->init();
		    }
	    }
    }
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
		for (Object* object : row)
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

Sprite* MapLayer::getSpriteWithFrame(const std::string& frame_name) const
{
    if (is_front_)
    {
        SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frame_name);
        if (spriteFrame != nullptr) 
        {
            Sprite* sprite = Sprite::createWithSpriteFrame(spriteFrame);
            Vec<int> pos = toPixel(add_pos_);
            sprite->setPosition(static_cast<float>(pos.X()), static_cast<float>(pos.Y()));
            sprite->setAnchorPoint(Vec2(0, 0));
            layer_->addChild(sprite, 0);
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
