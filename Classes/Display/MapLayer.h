#pragma once
#include "json/document.h"
#include "cocos2d.h"

class MapLayer{
    private:
		cocos2d::Layer* layer;

	public:
        cocos2d::TMXTiledMap* createTileMap(std::string tmx_name) const;

        //void CreatorReader::parseTilemap(cocos2d::TMXTiledMap* tilemap, const buffers::TileMap* tilemapBuffer) const
        //{
        //    const auto& nodeBuffer = tilemapBuffer->node();
        //    parseNode(tilemap, nodeBuffer);

        //    // calculate scale. changing the contentSize in TMX doesn't affect its visual size
        //    // so we have to re-scale the map
        //    const auto& desiredSize = tilemapBuffer->desiredContentSize();
        //    const auto& currentSize = tilemap->getContentSize();

        //    float wr = desiredSize->w() / currentSize.width;
        //    float hr = desiredSize->h() / currentSize.height;

        //    float sx = tilemap->getScaleX();
        //    float sy = tilemap->getScaleY();

        //    tilemap->setScaleX(wr * sx);
        //    tilemap->setScaleY(hr * sy);
        //}
};