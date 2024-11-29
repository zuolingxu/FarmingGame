#pragma once
#include "json/document.h"
#include "cocos2d.h"

class MapLayer{
    private:
	// Singleton mode, there's only one instance
	static MapLayer* instance_;
	cocos2d::Layer* layer;

public:
};