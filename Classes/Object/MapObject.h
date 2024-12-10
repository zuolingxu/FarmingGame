#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "HelperClasses.h"
#include "json/document.h"

class MapLayer;

class MapObject : public cocos2d::Ref {
public:
	struct ObjectInfo
	{
		cocos2d::Sprite* sprite; //  pointer to the sprite, if not loaded then nullptr
		Vec<int> size;           //  Size of the Sprite
		Vec<int> position;       //  Grid position of the Sprite
	};

protected:
	ObjectInfo info_;

public:
	MapObject(const Vec<int>& pos);
	virtual ~MapObject() = default;
	MapObject(const MapObject& other) = delete;
	MapObject& operator=(const MapObject& other) = delete;
	
	static MapObject* create(rapidjson::Value& val, MapLayer* parents, const Vec<int>& pos);
	MapObject::ObjectInfo& getInfo();
	virtual void init() = 0; 
	virtual void interact() = 0;
	virtual void settle() = 0;       //游戏内一天结束时调用
	virtual void clear() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;
	virtual bool hasCollision() = 0;  //有无碰撞体积
};