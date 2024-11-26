#pragma once
#include "json/document.h"
#include "cocos2d.h"
#include "CollideManager.h"

class MapLayer{
    private:
	// Singleton mode, there's only one instance
	static MapLayer* instance_;
	CollideManager collideManager;
	cocos2d::Layer* layer;
	MapLayer() : layer(nullptr), collideManager(){}

    public:
	static MapLayer* getMap(const rapidjson::Value* doc);

	cocos2d::Layer* getLayer();

};