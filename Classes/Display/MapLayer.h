#pragma once
#include "json/document.h"
#include "cocos2d.h"
#include "PlayerSprite.h"
#include <vector>
#include "HelperClasses.h"
#include "MapObject.h"

class MapObject;
class PlayerSprite;

class MapLayer: public cocos2d::Ref{
public:
	// Avoid multiple instance
	MapLayer(const MapLayer&) = delete;
	MapLayer& operator=(const MapLayer&) = delete;
	MapLayer(MapLayer&&) = delete;
	MapLayer& operator=(MapLayer&&) = delete;

	// These function is for MapObject, load .plist file and picture file
	// If you want to use anything in a picture set, you should call this function first.
	static void loadPlist(std::string plist_name);

	// to know if a place has a collision, the pos should be a pixel position
	bool hasCollision(const cocos2d::Vec2& pos);

	// This is for NetWork, TODO: MultiPlayer
	void addPlayer(PlayerSprite* player);

	// frame_name is name of frame in .plist file.
	// pos is GRID position of sprite, the anchor is (0,0) (bottom left).
	// You should call loadPlist method first, if the sprite set has not been loaded.
	void addSpriteWithFrame(MapObject::ObjectInfo& obj_info, const std::string& frame_name) const;

	// add a PlayerSprite into the Map, sprite_document is a json document that contains all information of the sprite.
	void addPlayerSpriteWithDocument(MapObject::ObjectInfo& obj_info, const rapidjson::Document* sprite_document);

	// change frame of a sprite, frame_name is name of frame in .plist file.
	// This function is only for normal Sprite
	void changeWithSingleFrame(cocos2d::Sprite* stationary_sprite, const std::string& new_frame_name) const;

	void updateMaps(const Vec<int>& old_pos, const Vec<int>& new_pos, const Vec<int>& size);


	friend class SceneManager;
	// These function is for SceneManager
	static MapLayer* createWithDocument(const std::string& tmx_path,
		const cocos2d::Color3B& background_color, rapidjson::Value* const_object, rapidjson::Value* archive_object);

	// Make this layer to front, and load tmx file
	cocos2d::Node* toFront(PlayerSprite* main_player);

	// Pause the layer and MapObject
	void pause() const;

	// Resume the layer and MapObject
	void resume() const;

	// Make this layer to back, and unload tmx file
	void toBack();

	// clear all objects in the map, It will be called when Archive unloaded
	void clearObjects();

	// settle should be called after a day
	void settle() const;

	// The following function is for camera reset
	friend PlayerSprite;

	// Get camera pointer
	cocos2d::Camera* getCamera() const;

private:
	// These data member will be initialized when map layer is created
	std::vector<std::vector<MapObject*>> interact_map_; //  Store Objects
	std::vector<std::vector<bool>> collision_map_; //  Use a bitset to represent collision
	std::string tmx_name_; //  Store tmx file name, for loading tmx file when toFront()
	cocos2d::Color3B background_color_; //  Store background color
	std::vector<cocos2d::Sprite*> players_; //  Store all PlayerSprite

	// These data member will be initialized when toFront() is called
	cocos2d::Node* layer_ = nullptr; //  The Node Store the layer
	cocos2d::TMXTiledMap* tiled_map_ = nullptr; //  Store the tmx file
	PlayerSprite* main_player_ = nullptr; //  Store the main player, where camera focus on
	cocos2d::Camera* camera_ = nullptr; // Store the camera
	cocos2d::EventListenerTouchAllAtOnce* touch_listener_ = nullptr; //  Store the touch listener, for multiplatform
	cocos2d::EventListenerKeyboard* keyboard_Listener_ = nullptr; //  Store the keyboard listener
	cocos2d::EventListenerMouse* mouse_listener_ = nullptr; //  Store the mouse listener
	cocos2d::Sprite* focus_ = nullptr;  //  Store the focus sprite
	cocos2d::Vec2 mouse_pos_ = cocos2d::Vec2::ZERO; //  Store the mouse position, for reFocus
	Vec<int> focus_pos_; //  Store the focus position
	bool is_front_ = false; //  represent whether the layer is being displayed

	// private Constructor and Destructor
	MapLayer(const std::string& tmx_path, const cocos2d::Color3B& background_color, 
		rapidjson::Value* const_object, rapidjson::Value* archive_object);
	~MapLayer() = default;

	// These function is for Create step
	void addObject(const Vec<int>& pos, rapidjson::Value& val); // add Objects
	void addCollisions(); //  add Collisions

	// These function is for InitStep
	void addTiledMap(); //  init tiled map
	void addEventListener(); //  init event listener

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
	void changeHolding(int num);

	// refocus will be called when mouse pos changed, player pos changed
	void refocus();
};