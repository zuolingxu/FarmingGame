#include "MapLayer.h"

USING_NS_CC;

MapLayer::MapLayer(const Vec2 size ,const std::string& tmx_name, rapidjson::Value* object_list) :
    tmx_name_(tmx_name), object_list_(object_list), interact_map_(size.x, std::vector<Object*>(size.y, nullptr)),
    collision_map_(size.x, std::vector<bool>(size.y, false)){}

void MapLayer::addObject(Vec2 pos, rapidjson::Value object_json)
{
	
}

void MapLayer::createWithDocument(Vec2 size, std::string tmx_name, rapidjson::Value* object_json)
{

}

void MapLayer::freePictureResource()
{

}

cocos2d::TMXTiledMap* MapLayer::showTileMap(std::string tmx_name) const
{

}

cocos2d::Sprite* MapLayer::showWithPicture(std::string pic_name)
{

}

void MapLayer::changeWithPicture(cocos2d::Sprite* sprite, std::string pic_name)
{

}


cocos2d::Sprite* MapLayer::showWithSpriteFrame(std::string frame_name)
{

}


void MapLayer::changeWithActionSequence(std::vector<int> sequence)
{
	
}





cocos2d::TMXTiledMap* createTileMap(const std::string& tmxfilename)
{
    cocos2d::TMXTiledMap* tilemap = TMXTiledMap::create(tmxfilename);

        parseTilemap(tilemap, tilemapBuffer);
    return tilemap;
}

void parseTilemap(cocos2d::TMXTiledMap* tilemap)
{
    if (tilemap)
    {
        const auto& nodeBuffer = tilemapBuffer->node();
        parseNode(tilemap, nodeBuffer);

        // calculate scale. changing the contentSize in TMX doesn't affect its visual size
        // so we have to re-scale the map
        const auto& desiredSize = tilemapBuffer->desiredContentSize();
        const auto& currentSize = tilemap->getContentSize();

        float wr = desiredSize->w() / currentSize.width;
        float hr = desiredSize->h() / currentSize.height;

        float sx = tilemap->getScaleX();
        float sy = tilemap->getScaleY();

        tilemap->setScaleX(wr * sx);
        tilemap->setScaleY(hr * sy);
    }
   
}