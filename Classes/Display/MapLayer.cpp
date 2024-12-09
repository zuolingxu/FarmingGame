#include "MapLayer.h"
#include "json/reader.h"
#include "Object.h"
#include "SceneManager.h"
#include <functional>

USING_NS_CC;
#ifdef _MSC_VER
#undef GetObject
#endif

static constexpr int BACKGROUND_ZORDER = -512;
static constexpr int TMX_ZORDER = -256;
static constexpr int MAP_MAX_LENGTH = 10;
static constexpr float SCALE_FACTOR = 1.5f;

MapLayer::MapLayer(const std::string& tmx_path, const cocos2d::Color3B& background_color,
    rapidjson::Value* const_object, rapidjson::Value* archive_object) : background_color_(background_color)
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

MapLayer* MapLayer::createWithDocument(const std::string& tmx_path, const Color3B& background_color, rapidjson::Value* const_object, rapidjson::Value* archive_object)
{
    MapLayer* map = new(std::nothrow) MapLayer(tmx_path, background_color, const_object, archive_object);
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
        layer_->addChild(tiled_map_, TMX_ZORDER);
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
                for (int y = left_bottom.Y(); y <= right_top.Y(); y++)
                {
                    collision_map_[x][y] = true;
                }
            }
        }
    }
    else {
        CCLOG("Object layer not found!");
    }

    for (int i = 0; i < collision_map_.size(); i++)
    {
	    for (int j = 0; j < collision_map_[i].size(); j++)
	    {
            if (interact_map_[i][j] != nullptr)
            {
                
                collision_map_[i][j] = collision_map_[i][j] == true || interact_map_[i][j]->hasCollision();
            }
	    }
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
    case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
        main_player_->move(PlayerSprite::MOVEMENT::W_UP);
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_S:
    case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
        main_player_->move(PlayerSprite::MOVEMENT::W_DOWN);
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
    case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
        main_player_->move(PlayerSprite::MOVEMENT::W_LEFT);
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_D:
    case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
        main_player_->move(PlayerSprite::MOVEMENT::W_RIGHT);
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
    case cocos2d::EventKeyboard::KeyCode::KEY_CAPS_LOCK:
        main_player_->changeSpeed();
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_F1:
        // TODO: Test Code 
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_F2:
        // TODO: Test Code
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_F4:
        // TODO: Test Code
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_F5:
        // TODO: Test Code
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_F6:
        // TODO: Test Code
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_F7:
        // TODO: Test Code
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_F8:
        // TODO: Test Code
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_F9:
        // TODO: Test Code
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_F10:
        // TODO: Test Code
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_F11:
        // TODO: Test Code
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_F12:
        // TODO: Test Code
        break;
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
    case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
        main_player_ -> stop(PlayerSprite::MOVEMENT::W_UP);
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_S:
    case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
        main_player_->stop(PlayerSprite::MOVEMENT::W_DOWN);
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
    case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
        main_player_->stop(PlayerSprite::MOVEMENT::W_LEFT);
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_D:
    case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
        main_player_->stop(PlayerSprite::MOVEMENT::W_RIGHT);
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_SHIFT:
        main_player_->changeSpeed();
        break;
    default: break;
    }

    event->stopPropagation(); 
}

void MapLayer::onMouseDown(cocos2d::Event* event)
{
    static const std::vector<Vec<int>> valid_pos = { {1,0} ,{1,1}, {0,0},
        {0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}};
    cocos2d::EventMouse* e = dynamic_cast<cocos2d::EventMouse*>(event);
    if (e)
    {
        if (e->getMouseButton() == cocos2d::EventMouse::MouseButton::BUTTON_LEFT) 
        {
            try
            {
                ::Object* focus = interact_map_.at(focus_pos_.X()).at(focus_pos_.Y());
                Vec<int> grid_pos = toGrid(main_player_->getPosition() + Vec2(GridSize / 2, 0));
                for (auto& pos : valid_pos)
                {
                    if (focus_pos_ == grid_pos + pos) {
                        main_player_->interact(pos);
                        if (focus != nullptr)
                        {
                            focus->interact();
                        }
                    }
                }
            }
            catch (std::exception& exception)
            {
	            
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
        Size win_size = Director::getInstance()->getWinSize();
    	mouse_pos_ = e->getLocation();
        mouse_pos_.y = win_size.height - mouse_pos_.y;
        mouse_pos_ = (mouse_pos_ - win_size / 2) / SCALE_FACTOR;
        refocus();
    }
    event->stopPropagation();
}

void MapLayer::onMouseUp(cocos2d::Event* event)
{
    // nothing to do
    event->stopPropagation();
}

void MapLayer::changeHolding(const int num)
{
	// TODO: UILogic::changeHoldings(num);
}

void MapLayer::refocus()
{
    focus_pos_ = toGrid(mouse_pos_ + main_player_->getPosition());
    focus_->setPosition(toPixel(focus_pos_));
}

Node* MapLayer::toFront(PlayerSprite* main_player)
{
    layer_ = Node::create();
    layer_->retain();
    is_front_ = true;

    addTiledMap();

    auto background = cocos2d::LayerColor::create(cocos2d::Color4B(background_color_));
    Size map_size = tiled_map_->getMapSize();
    Size tiled_size = tiled_map_->getTileSize();
    Size map_size_pixel(map_size.width * tiled_size.width, map_size.height * tiled_size.height);
    Size background_size = map_size_pixel + designResolutionSize + Size(1000, 1000);
	background->setContentSize(background_size);
    background->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
    background->setPosition(map_size_pixel / 2 - background_size / 2);
    layer_->addChild(background, BACKGROUND_ZORDER);


    Vec2 pixel_pos = map_size_pixel / 2;
    Size size = Director::getInstance()->getWinSize();
    float scale_factor = 0.9f;
    if (main_player != nullptr)
    {
        main_player_ = main_player;
        pixel_pos = main_player_->getPosition();
        main_player_->setParentMapLayer(this);
        layer_->addChild(main_player_, pixel_pos.y);
		scale_factor = SCALE_FACTOR;
    }
    size = size / scale_factor;
    camera_ = Camera::createOrthographic(size.width, size.height, -1024, 1024);
    camera_->setPosition(pixel_pos - size / 2);
    camera_->setCameraFlag(CameraFlag::USER1);
    camera_->setDepth(10);
    layer_->addChild(camera_);

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
    focus_ = Sprite::create(DocumentManager::getInstance()->getPath("FocusPic"));
    focus_->setPosition(toPixel(focus_pos_));
    focus_->setAnchorPoint(Vec(0, 0));
    layer_->addChild(focus_, 512);
    layer_->setCameraMask(static_cast<unsigned short>(CameraFlag::USER1));
    return layer_;
}

void MapLayer::pause() const
{
    main_player_->stop(PlayerSprite::MOVEMENT::ALL);
    for (auto& row : interact_map_)
    {
        for (::Object* object : row)
        {
            if (object != nullptr)
            {
                object->pause();
            }
        }
    }
    layer_->pause();
}

void MapLayer::resume() const
{
	layer_->resume();
    for (auto& row : interact_map_)
    {
	    for (::Object* object : row)
	    {
		    if (object != nullptr)
		    {
			    object->resume();
		    }
	    }
    }
}

void MapLayer::toBack()
{
	if (is_front_){
		is_front_ = false;
        tiled_map_ = nullptr;
        layer_ = nullptr;
        camera_ = nullptr;
        main_player_ = nullptr;
        touch_listener_ = nullptr;
        keyboard_Listener_ = nullptr;
        mouse_listener_ = nullptr;
        SpriteFrameCache::getInstance()->removeSpriteFrames();
	}
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

Camera* MapLayer::getCamera() const
{
    return camera_;
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

bool MapLayer::hasCollision(const cocos2d::Vec2& pos)
{
	Vec<int> grid = toGrid(pos);
    Vec<int> map_size = tiled_map_->getMapSize();
    if (pos.x > 0 && pos.y > 0 && grid.X() < map_size.X() && grid.Y() < map_size.Y())
    {
	    return collision_map_[grid.X()][grid.Y()];
    } 
    return true; 
}

void MapLayer::addPlayer(PlayerSprite* player)
{
    layer_->addChild(player, player->getPosition().y);
    player->setParentMapLayer(this);
    player->setCameraMask(static_cast<unsigned short>(CameraFlag::USER1));
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

Sprite* MapLayer::addSpriteWithFrame(const std::string& frame_name)
{
    if (is_front_)
    {
        SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frame_name);
        if (spriteFrame != nullptr) 
        {
            Sprite* sprite = Sprite::createWithSpriteFrame(spriteFrame);
            toPixel(focus_pos_);
            // sprite->setPosition3D({0,0,0});
            sprite->setAnchorPoint(Vec2(0, 0));
            sprite->setCameraMask(static_cast<unsigned short>(CameraFlag::USER1));
            layer_->addChild(sprite, focus_pos_.Y() * GridSize);
            return sprite;
        }
    }
	return nullptr;
}

PlayerSprite* MapLayer::addPlayerSpriteWithDocument(const rapidjson::Document* sprite_document)
{
	PlayerSprite* player = PlayerSprite::create(sprite_document);
    players_.emplace_back(player);
    return player;
}


void MapLayer::changeWithSingleFrame(int num)
{
	if (is_front_)
	{
		
	}
}
