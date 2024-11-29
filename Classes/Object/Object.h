#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "json/document.h"


class Object {
public:
	Object() = default;
	virtual ~Object();
	Object(const Object& other) = delete;
	Object& operator=(const Object& other) = delete;
	
	static Object* create(rapidjson::Value& val); 
	virtual void init() = 0; 
	virtual void interact() = 0; 
};