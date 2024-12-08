#pragma once
#define CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL 1
#include <string>
#include <unordered_map>
#include "DocumentManager.h"
#include "MapLayer.h"
#include "UILayer.h"
#include "ui/UILoadingBar.h"
#include "cocos2d.h"
#include "TimeManager.h"

class SceneManager
{
private:
	static SceneManager* instance_;
	std::string current_map_name_;
	cocos2d::Director* director_;
	std::unordered_map<std::string, cocos2d::Layer*> layers_;
	std::unordered_map<std::string, MapLayer*> map_;
	cocos2d::Node* permanent_node_;

	SceneManager();
	~SceneManager();

	// void createUIWithDocument(rapidjson::Document* doc);
	void createMapWithDocument(rapidjson::Document* doc);

	// If the Archive has been load/unload the maps will be automatic created/removed
	// create map with document when loading Archive
	void createMaps();

	// clear all game logic when changing Archive 
	void clearMaps();

	class NextMapCallBack
	{
	private:
		float loading_per = 0;
		std::string map_name;
		std::string pos;
		cocos2d::ui::LoadingBar* loading_bar = nullptr;
		cocos2d::Scene* loading_scene = nullptr;
		cocos2d::Node* next_map = nullptr;
	public:
		NextMapCallBack(std::string map_name, std::string pos);
		~NextMapCallBack() = default;
		void operator()();
		void start();
		void create();
		void render();
		void assemble();
	};

public:
	SceneManager(SceneManager const&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(SceneManager const&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;

	static SceneManager* getInstance();

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

