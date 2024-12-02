#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "json/document.h"
#include "MapLayer.h"

class MapLayer;

class Object {
public:
	Object() = default;
	virtual ~Object() = default;
	Object(const Object& other) = delete;
	Object& operator=(const Object& other) = delete;
	
	static Object* create(rapidjson::Value& val, MapLayer* parents);
	virtual void init() = 0; 
	virtual void interact() = 0;
	virtual void clear() = 0;
	virtual void settle() = 0;
};