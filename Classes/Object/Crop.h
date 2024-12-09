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
	explicit Crop(MapLayer* parent, const Vec<int>& pos);
	~Crop();
	static Crop* create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos);
	void init();
	void interact();
	void clear();
	void pause();
	void resume();
	void settle();
}; 