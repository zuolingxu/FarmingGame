#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "json/reader.h"

class UILayer : cocos2d::Layer{
    public:
	static UILayer* createWithFile(const std::string& name);

	// initialize();

};