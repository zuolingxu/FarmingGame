#pragma once
#include "json/document.h"
#include "cocos2d.h"
#include "CollideManager.h"

class MapManager{
    private:
	// Singleton mode, there's only one instance
	static MapManager* instance_;
	CollideManager collideManager;
	cocos2d::Layer* layer;
	MapManager() : layer(nullptr), collideManager(){}

    public:
	static MapManager* getMap(const rapidjson::Value* doc);

	cocos2d::Layer* getLayer();

};