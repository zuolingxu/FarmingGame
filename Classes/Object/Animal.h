#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "Object.h"
#include "json/document.h"

class Animal: protected Object {
private:
	cocos2d::Sprite* sprite_ = nullptr;

public:
	Animal();
	~Animal() override;
	static Object* create(rapidjson::Value& val);
	virtual void init() override;
	virtual void interact() override;
};