#pragma once
#include "json/document.h"
#include "cocos2d.h"
#include <vector>
#include "Object.h"

class MapLayer{
    private:
	cocos2d::Layer* layer_ = nullptr;
	std::string tmx_name_;
	cocos2d::TMXTiledMap* tiled_map_ = nullptr;
	rapidjson::Value* object_list_;
	std::vector<std::vector<Object*>> interact_map_;
	std::vector<std::vector<bool>> collision_map_;
	MapLayer(const cocos2d::Vec2 size, const std::string& tmx_name, rapidjson::Value* object_list);
	~MapLayer() = default;

	void addObject(cocos2d::Vec2 pos, rapidjson::Value object_json);

	public:
	static void createWithDocument(cocos2d::Vec2 size, std::string tmx_name, rapidjson::Value* object_json);
	void freePictureResource();

	cocos2d::TMXTiledMap* showTileMap(std::string tmx_name) const;

	cocos2d::Sprite* showWithPicture(std::string pic_name);
	void changeWithPicture(cocos2d::Sprite* sprite, std::string pic_name);

	cocos2d::Sprite* showWithSpriteFrame(std::string frame_name);
	void changeWithActionSequence(std::vector<int> sequence);
};