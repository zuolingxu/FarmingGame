#pragma once
#include "json/document.h"
#include "cocos2d.h"
#include <vector>
#include "HelperClasses.h"


class Object;

class MapLayer{
private:
	std::vector<std::vector<Object*>> interact_map_;
	std::vector<std::vector<bool>> collision_map_;
	std::string tmx_name_;

	cocos2d::Layer* layer_ = nullptr;
	cocos2d::TMXTiledMap* tiled_map_ = nullptr;
	cocos2d::Sprite* player_ = nullptr;
	cocos2d::Camera* camera_ = nullptr;
	cocos2d::EventListenerTouchAllAtOnce* touch_listener_ = nullptr;
	cocos2d::EventListenerKeyboard* keyboard_Listener_ = nullptr;
	cocos2d::EventListenerMouse* mouse_listener_ = nullptr;
	Vec<int> add_pos_;
	bool is_front_ = false;

	MapLayer(const Vec<int>& size, std::string tmx_path,
		rapidjson::Value* const_object, rapidjson::Value* archive_object);
	~MapLayer() = default;

	void addTiledMap() const;
	void addObject(Vec<int> pos, rapidjson::Value& val);
	void addEventListener();

	// CallBacks
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	void onMouseDown(cocos2d::Event* event);
	void onMouseMove(cocos2d::Event* event);
	void onMouseUp(cocos2d::Event* event);

public:
	MapLayer(const MapLayer&) = delete;
	MapLayer& operator=(const MapLayer&) = delete;
	MapLayer(MapLayer&&) = delete;
	MapLayer& operator=(MapLayer&&) = delete;

	static MapLayer* createWithDocument(const Vec<int>& size, const std::string& tmx_path,
	                                    rapidjson::Value* const_object, rapidjson::Value* archive_object);

	// pos is start GRID position of player sprite
	void toFront(Vec<int> pos);
	void pause() const;
	void resume() const;
	void toBack();

	void settle() const;

	// load .plist file and picture
	static void loadPlist(std::string plist_name);

	// frame_name is name of frame in .plist file.
	// pos is GRID position of sprite, the anchor is (0,0) (bottom left).
	cocos2d::Sprite* getSpriteWithFrame(const std::string& frame_name) const;

	void changeWithActionSequence(std::vector<int> sequence);
	void changeWithSingleFrame(int num);
};