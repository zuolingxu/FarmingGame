#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "json/document.h"
#include "MapLayer.h"

class Crop{
	private:
	cocos2d::Sprite* sprite_ = nullptr;
	MapLayer* parent_;

	public:
	explicit Crop(MapLayer* parent);
	~Crop();
	static Crop* create(rapidjson::Value& val, MapLayer* parent);
	void init();
	void interact();
	void clear();
	void settle();
}; 