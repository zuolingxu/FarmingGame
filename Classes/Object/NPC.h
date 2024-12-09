#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "MapObject.h"
#include "json/document.h"
#include "MapLayer.h"

class NPC final: protected MapObject {
	private:
	MapLayer* parent_;

    public:
	explicit NPC(MapLayer* parent, const Vec<int>& pos);
	~NPC() override;
	static MapObject* create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos);
	virtual void init() override; 
	virtual void interact() override;
	virtual void clear() override;
	virtual void pause() override;
	virtual void resume() override;
	virtual void settle() override;
	virtual bool hasCollision() override;
};