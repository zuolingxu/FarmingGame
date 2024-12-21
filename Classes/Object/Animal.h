#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "MapObject.h"
#include "json/document.h"
#include "MapLayer.h"
#include"PlayerSprite.h"
#include "MainCharacter.h"

class Animal final: protected MapObject {
private:
	MapLayer* parent_;
	std::string type;
	bool isSold;
	int value;
	cocos2d::Vec2 position;
	int satiety;
	float breedProbility;
	bool change_archive_in_memory();
	bool delete_archive_in_memory();
	bool new_archive_in_memory(int x, int y);
public:
	explicit Animal(MapLayer* parent, const Vec<int>& pos, std::string Type, int AnimalValue,int satiety,int length,float breedPro=0.2);
	~Animal() override {};
	static MapObject* create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos);
	virtual void init() override;
	virtual void interact() override;
	virtual void clear() override;
	virtual void pause() override;
	virtual void resume() override;
	virtual void settle() override;
	virtual bool hasCollision() override;
	void defaultAction();
	void sold();
	void makeSound();
	void breed();
};


