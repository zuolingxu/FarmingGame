#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "Object.h"
#include "json/document.h"
#include "MapLayer.h"

class NPC final: protected Object {
	private:
	cocos2d::Sprite* sprite_ = nullptr;
	MapLayer* parent_;

    public:
	explicit NPC(MapLayer* parent);
	~NPC() override;
	static Object* create(rapidjson::Value& val, MapLayer* parent); 
	virtual void init() override; 
	virtual void interact() override;
	virtual void clear() override;
	virtual void settle() override;
};