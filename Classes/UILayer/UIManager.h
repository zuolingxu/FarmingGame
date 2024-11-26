#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "json/reader.h"

class UIManager{
    private:
	static std::map<std::string, UIManager*> ui_list;
	UIManager* ui;
	cocos2d::Layer* ui_layer;

    public:
	static UIManager* createWithFile(const std::string& path);

	cocos2d::Layer* getLayer() const;

};