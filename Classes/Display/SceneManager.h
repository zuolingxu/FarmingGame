#pragma once
#include "cocos2d.h"
#include <string>
#include <unordered_map>

class SceneManager
{
    private:
	static SceneManager* instance_;
	cocos2d::Director* director_;
	std::unordered_map<std::string, cocos2d::Layer*> layers_;

	SceneManager();
	~SceneManager();
	void initLayer();
	void parser_layer();

    public:
	static SceneManager* getInstance();
	void showLayer(std::string layer_name);
	void hideLayer(std::string layer_name);

	void NextScene(std::string scene_name);
};

