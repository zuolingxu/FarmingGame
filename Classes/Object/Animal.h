#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "Object.h"
#include "json/document.h"
#include "MapLayer.h"

class Animal final: protected Object {
private:
	MapLayer* parent_;
	std::string name;
	bool isSold;
	cocos2d::Vec2 position;

public:
	explicit Animal(MapLayer* parent);
	~Animal() override;
	static Object* create(rapidjson::Value& val, MapLayer* parent);
	virtual void init() override;
	virtual void interact() override;
	virtual void clear() override;
	virtual void pause() override;
	virtual void resume() override;
	virtual void settle() override;
	virtual bool hasCollision() override;
};