#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "json/document.h"
#include "MapLayer.h"

class MapLayer;

class Object : public cocos2d::Ref {
public:
	struct ObjectInfo
	{
		cocos2d::Sprite* sprite;
		Vec<int> size;           //碰撞体积大小
	};

protected:
	ObjectInfo info_;

public:
	Object() = default;
	virtual ~Object() = default;
	Object(const Object& other) = delete;
	Object& operator=(const Object& other) = delete;
	
	static Object* create(rapidjson::Value& val, MapLayer* parents);

	virtual void init() = 0; 
	virtual void interact() = 0;
	virtual void settle() = 0;       //游戏内一天结束时调用
	virtual void clear() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;
	virtual bool hasCollision() = 0;  //有无碰撞体积
};