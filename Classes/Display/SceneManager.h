#pragma once
#include <string>
#include <unordered_map>
#include "MapLayer.h"
#include "ui/UILoadingBar.h"
#include "cocos2d.h"


// Singleton, SceneManager manage every scene in the game
// SceneManager will be created when the app start
class SceneManager
{
public:
	// Singleton, only one instance can be created
	SceneManager(SceneManager const&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(SceneManager const&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;

	// get the instance of SceneManager
	static SceneManager* getInstance();

	// settle every object on the map after day passed
	// This will call every settle function in every MapLayer
	void settle();

	// get the name of current map or UI
	std::string getCurrentMapName() { return current_map_name_; }
	std::string getCurrentUIName() { return current_UI_name_; }

	// change UI render order
	// showUILayer will show the UI on the top of the map
	// UI_name is the name of the Layer
	// base shall be true when the UI will be shown troughout the scene
	void showUILayer(const std::string& UI_name, bool base = false);

	// hideUILayer will hide all UI and Display the map
	// if UI_name is ALL, it will hide all UI
	// if UI_name is CURRENT, it will hide the top UI
	 void hideUILayer(const std::string& UI_name);

	// pos (string)x y init position of playsprite ,default in the middle of the map
	// in the archive pos is the key
	// Go to Next Map. If pos is not specified, it won't add a player and all Listener will be removed(for start UI)
	// If the map cannot be found, it will throw an exception
	void NextMap(const std::string& map_name, const std::string& pos = "default", float interval = 0.0f) const;

private:
	static SceneManager* instance_; //  the only instance
	std::string current_map_name_; //  the name of the current map
	std::string current_UI_name_; // the name of the top UI
	cocos2d::Director* director_; //  the director of the game
	std::unordered_map<std::string, MapLayer*> map_; //  the maps of the game
	cocos2d::Node* permanent_node_ = nullptr; //  the node that will be permanent in the game, used to store UILayers

	// hide Constructor and Destructor
	SceneManager();
	~SceneManager();

	// void createUIWithDocument(rapidjson::Document* doc);
	void createMapWithDocument(rapidjson::Document* doc);

	// If the Archive has been load/unload the maps will be automatic created/removed
	// create map with document when loading Archive
	void createMaps();

	// clear all game logic when changing Archive 
	void clearMaps();

	// The callbacks for NextMap function
	class NextMapCallBack
	{
	private:
		float loading_per = 0; //  loading percentage
		std::string map_name; //  the name of the target map
		std::string pos; //  the position of the player
		cocos2d::ui::LoadingBar* loading_bar = nullptr; //  the loading bar
		cocos2d::Node* next_map = nullptr; //  the target map's Node
	public:
		// Constructor and Destructor
		NextMapCallBack(std::string map_name, std::string pos);
		~NextMapCallBack() = default;

		// the callback function
		void operator()();

		// create a new Scene with a loading bar
		void start();

		// create and clear the maps, this steps will only be effective when archive status changes
		// After creating/loading/free Archive, And when you change the map, this function will be called
		void create();

		// make the target map to front
		void render();

		// assemble new scene
		void assemble();
	};
};

