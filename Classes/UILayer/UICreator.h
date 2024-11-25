#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "json/reader.h"

class UICreator{
    private:


    public:
	static cocos2d::Layer* createWithFile(const rapidjson::Value* doc);

};