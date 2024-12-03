#pragma once
#include <string>
#include <unordered_map>
#include "DocumentManager.h"
#include "cocos2d.h"
#include "MapLayer.h"
#include "UILayer.h"

class SceneManager
{
private:
	static SceneManager* instance_;
	cocos2d::Director* director_;
	std::unordered_map<std::string, cocos2d::Layer*> layers_;
	std::unordered_map<std::string, MapLayer*> map_;
	cocos2d::Node* permanent_node_;

	SceneManager();
	~SceneManager();

	// void createUIWithDocument(rapidjson::Document* doc);
	void createMapWithDocument(rapidjson::Document* doc);

public:
	SceneManager(SceneManager const&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(SceneManager const&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;

	static SceneManager* getInstance();

	// create map with document when loading Archive
	void createMaps();

	// clear all game logic when changing Archive 
	void clearMaps();

	// settle every object on the map after day passed
	void settle();

	// change UI render order
	void showUILayer(const std::string& UI_name) const;
	void changeUILayer(const std::string& UI_name) const;
	void hideUILayer() const;

	// Go to Next Map. If pos is not specified, it won't add a player and all Listener will be removed(for start UI)
	// If the map cannot be found, it will throw an exception
	void NextMap(const std::string& map_name, const std::string& pos = "default") const;
};

