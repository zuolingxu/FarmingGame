#pragma once
#include "cocos2d.h"
#include "Crop.h"
#include "DocumentManager.h"
#include "MapObject.h"
#include "json/document.h"
#include "MapLayer.h"

class Land final: protected MapObject {
    private:
	Crop* crop_ = nullptr;
    MapLayer* parent_;

    public:
	explicit Land(MapLayer* parent, const Vec<int>& pos);
    ~Land() override;
    static MapObject* create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos);
    virtual void init() override;
    virtual void interact() override;
    virtual void clear() override;
    virtual void pause() override;
    virtual void resume() override;
    virtual void settle() override;
    virtual bool hasCollision() override;
}; 