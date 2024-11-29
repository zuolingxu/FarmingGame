#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "json/document.h"

class Crop{
	private:
	cocos2d::Sprite* sprite_ = nullptr;

	public:
	Crop();
	~Crop();
	static Crop* create(rapidjson::Value& val);
	void init();
	void interact();
}; 