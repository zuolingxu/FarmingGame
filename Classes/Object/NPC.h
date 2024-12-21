#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "MapObject.h"
#include "json/document.h"
#include "MapLayer.h"
#include "MainCharacter.h"
#include "Audio.h"
#include "UIlogic.h"
#include "SceneManager.h"

class NPC final: protected MapObject {
	private:
	MapLayer* parent_;
	std::string name;
	int emotion;
	bool isPaused;
	const std::unordered_map<std::string, ItemType >Favorite = {
		{"Abigail", ItemType::CAULIFLOWER},
		{"Caroline", ItemType::PUMPKIN},
		{"Haley",ItemType::FISH }
	};
	const std::unordered_map<std::string, std::string >Scene = {
		{"Abigail", "Town"},
		{"Caroline","npc1house"},
		{"Haley","npc2house"}
	};
	void change_archive_in_memory();
	void defaultAction();
    public:
	explicit NPC(MapLayer* parent, const Vec<int>& pos, std::string name, int emo,int length,bool pause);
	~NPC() override;
	static MapObject* create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos);
	virtual void init(); 
	virtual void interact();
	virtual void clear();
	virtual void pause();
	virtual void resume();
	virtual void settle();
	virtual bool hasCollision();
};

