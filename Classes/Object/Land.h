#pragma once
#include "cocos2d.h"
#include "Crop.h"
#include "DocumentManager.h"
#include "Object.h"
#include "json/document.h"
#include "MapLayer.h"

class Land final: protected Object {
    private:
	Crop* crop_ = nullptr;
    cocos2d::Sprite* sprite_ = nullptr;
    MapLayer* parent_;

    public:
	explicit Land(MapLayer* parent);
    ~Land() override;
    static Object* create(rapidjson::Value& val, MapLayer* parent);
    virtual void init() override;
    virtual void interact() override;
    virtual void clear() override;
}; 