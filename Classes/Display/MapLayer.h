#pragma once
#include "json/document.h"
#include "cocos2d.h"
#include <vector>
#include "HelperClasses.h"


class Object;

class MapLayer{
private:
	cocos2d::Layer* layer_ = nullptr;
	std::string tmx_name_;
	cocos2d::TMXTiledMap* tiled_map_ = nullptr;
	std::vector<std::vector<Object*>> interact_map_;
	std::vector<std::vector<bool>> collision_map_;
	cocos2d::Sprite* player_ = nullptr;
	cocos2d::Camera* camera_ = nullptr;
	bool is_front_ = false;

	MapLayer(const Vec<int>& size, std::string tmx_path,
		rapidjson::Value* const_object, rapidjson::Value* archive_object);
	~MapLayer() = default;

	void showTiledMap() const;

public:
	MapLayer(const MapLayer&) = delete;
	MapLayer& operator=(const MapLayer&) = delete;
	MapLayer(MapLayer&&) = delete;
	MapLayer& operator=(MapLayer&&) = delete;

	static MapLayer* createWithDocument(const Vec<int>& size, const std::string& tmx_path,
	                                    rapidjson::Value* const_object, rapidjson::Value* archive_object);

	// pos is start GRID position of player sprite
	void init(Vec<int> pos);

	// load .plist file and picture
	static void loadPlist(std::string plist_name);
	static void freePlist();

	// frame_name is name of frame in .plist file.
	// pos is GRID position of sprite, the anchor is (0,0) (bottom left).
	cocos2d::Sprite* getSpriteWithFrame(const std::string& frame_name, Vec<int> pos) const;

	void changeWithActionSequence(std::vector<int> sequence);
	void changeWithSingleFrame(int num);
};