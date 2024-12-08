#pragma once
#define CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL 1
#include "json/document.h"
#include "cocos2d.h"
#include "PlayerSprite.h"
#include <vector>
#include "HelperClasses.h"


class Object;
class PlayerSprite;

class MapLayer: public cocos2d::Ref{
private:
	std::vector<std::vector<::Object*>> interact_map_;
	std::vector<std::vector<bool>> collision_map_;
	std::string tmx_name_;
	cocos2d::Color3B background_color_;
	std::vector<PlayerSprite*> players_;

	cocos2d::Node* layer_ = nullptr;
	cocos2d::TMXTiledMap* tiled_map_ = nullptr;
	PlayerSprite* main_player_ = nullptr;
	cocos2d::Camera* camera_ = nullptr;
	cocos2d::EventListenerTouchAllAtOnce* touch_listener_ = nullptr;
	cocos2d::EventListenerKeyboard* keyboard_Listener_ = nullptr;
	cocos2d::EventListenerMouse* mouse_listener_ = nullptr;
	Vec<int> focus_pos_;
	bool is_front_ = false;

	MapLayer(const std::string& tmx_path, const cocos2d::Color3B& background_color, 
		rapidjson::Value* const_object, rapidjson::Value* archive_object);
	~MapLayer() = default;

	static MapLayer* createWithDocument(const std::string& tmx_path,
	                                    const cocos2d::Color3B& background_color, rapidjson::Value* const_object, rapidjson::Value* archive_object);


	// These function is for InitStep
	void addTiledMap();
	void addObject(Vec<int> pos, rapidjson::Value& val);
	void addCollisions();
	void addEventListener();

	// CallBacks for Listener
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	void onMouseDown(cocos2d::Event* event);
	void onMouseMove(cocos2d::Event* event);
	void onMouseUp(cocos2d::Event* event);

	// changes for CallBacks
	void changeFocus();
	void changeHolding(int num);


protected:
	friend class SceneManager;
	//  These function is for SceneManager
	cocos2d::Node* toFront(PlayerSprite* main_player);
	void pause() const;
	void resume() const;
	void toBack();
	void clearObjects();

	// settle should be called after a day
	void settle() const;

	// The following function is for camera reset
	friend PlayerSprite;
	cocos2d::Camera* getCamera() const;

public:
	MapLayer(const MapLayer&) = delete;
	MapLayer& operator=(const MapLayer&) = delete;
	MapLayer(MapLayer&&) = delete;
	MapLayer& operator=(MapLayer&&) = delete;

	// These function is for Object
	// load .plist file and picture
	static void loadPlist(std::string plist_name);

	// to know if a place has a collision, the pos should be a pixel position
	bool hasCollision(const cocos2d::Vec2& pos);

	// This is for NetWork
	void addPlayerSprite(PlayerSprite* player);

	// frame_name is name of frame in .plist file.
	// pos is GRID position of sprite, the anchor is (0,0) (bottom left).
	cocos2d::Sprite* addSpriteWithFrame(const std::string& frame_name);
	PlayerSprite* addPlayerSpriteWithDocument(const rapidjson::Document* sprite_document);

	void changeWithActionSequence(std::vector<int> sequence);
	void changeWithSingleFrame(int num);
};