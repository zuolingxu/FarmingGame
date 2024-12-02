#pragma once
#include "cocos2d.h"
#include <string>
#include <unordered_map>
#include "DocumentManager.h"
#include "MapLayer.h"
#include "UILayer.h"

class SceneManager
{
private:
	static SceneManager* instance_;
	cocos2d::Director* director_;
	std::unordered_map<std::string, cocos2d::Layer*> layers_;
	std::unordered_map<std::string, MapLayer*> map_;
	std::unordered_map<std::string, UILayer*> UI_;

	SceneManager();
	~SceneManager();

	void createMapWithDocument(rapidjson::Document* doc);

public:
	SceneManager(SceneManager const&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(SceneManager const&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;

	static SceneManager* getInstance();
	void createMaps();

	void showLayer(std::string layer_name);
	void hideLayer(std::string layer_name);
	void NextScene(std::string scene_name);
};

