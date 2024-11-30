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
            Vec<int> pos = toVec2(object.name.GetString());
            interact_map_[pos.X()][pos.Y()] = Object::create(object.value, this);
        }
    }

    if (archive_object != nullptr)
    {
        for (auto& object : archive_object->GetObject())
        {
            Vec<int> pos = toVec2(object.name.GetString());
            interact_map_[pos.X()][pos.Y()] = Object::create(object.value, this);
        }
    }
}

MapLayer* MapLayer::createWithDocument(const Vec<int>& size, const std::string& tmx_path, rapidjson::Value* const_object, rapidjson::Value* archive_object)
{
    return new MapLayer(size, tmx_path, const_object, archive_object);
}

void MapLayer::showTiledMap() const
{
    if (tiled_map_ != nullptr)
    {
        TMXTiledMap* tmx = TMXTiledMap::create(tmx_name_);
        layer_->addChild(tmx, -256);
    }
}

void MapLayer::init(Vec<int> pos)
{
    layer_ = Layer::create();

    // create player sprite
    player_ = getSpriteWithFrame("", pos);
    pos = toPixel(pos);

    // bind camera
    camera_ = Camera::create();
    camera_->setCameraFlag(CameraFlag::USER1);
    camera_->setDepth(0);
    camera_->setScale(4.0f);

    camera_->setPosition(static_cast<float>(pos.X()), static_cast<float>(pos.Y()));

    layer_->addChild(camera_);
    layer_->setCameraMask(static_cast<unsigned short>(CameraFlag::USER1));

    showTiledMap();
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

void MapLayer::loadPlist(std::string plist_name)
{
    DocumentManager* manager = DocumentManager::getInstance();
    if (manager->hasDocument(plist_name))
    {
        plist_name = manager->getPath(plist_name);
    }
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist_name);
}

void MapLayer::freePlist()
{
    SpriteFrameCache::getInstance()->removeSpriteFrames();
}

Sprite* MapLayer::getSpriteWithFrame(const std::string& frame_name, Vec<int> pos) const
{
    if (is_front_)
    {
        SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frame_name);
        if (spriteFrame != nullptr) 
        {
            Sprite* sprite = Sprite::createWithSpriteFrame(spriteFrame);
            pos = toPixel(pos);
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
