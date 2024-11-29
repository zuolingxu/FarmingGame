#pragma once
#include "cocos2d.h"
#include "Crop.h"
#include "DocumentManager.h"
#include "Object.h"
#include "json/document.h"

class Land: protected Object {
    private:
	Crop* crop_ = nullptr;
    cocos2d::Sprite* sprite_ = nullptr;

    public:
	Land();
    ~Land() override;
    static Object* create(rapidjson::Value& val);
    virtual void init() override;
    virtual void interact() override;
}; 