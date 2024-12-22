#include "Gate.h"
#include "json/document.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "cocos2d.h"
#include "UILogic.h"

Gate::Gate(MapLayer* parent, const Vec<int>& pos,std::string NM,std::string NP,std::string FC) : parent(parent), MapObject(pos),NextMap(NM),NextPosition(NP),Function(FC)
{
	info_.size = { 1,1 };
}

Gate::~Gate()
{

}

MapObject* Gate::create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos)
{
	if (val.HasMember("NextMap") && val["NextMap"].IsString() &&
		val.HasMember("NextPosition") && val["NextPosition"].IsString()) {
		std::string NextMap= val["NextMap"].GetString();
		std::string NextPosition = val["NextPosition"].GetString();

		//function
		std::string Function = "";
		if (val.HasMember("Function") && val["Function"].IsString()) {
			Function = val["Function"].GetString();
		}
		return new Gate(parent, pos, NextMap, NextPosition, Function);
	}
	return nullptr;
}

void Gate::interact()
{
	if (SceneManager* SM = SceneManager::getInstance()) {
		// Function
		if (Function == "for_sleep") {
			SM->NextMap(NextMap, NextPosition);
			TimeManager::getInstance()->sleep();
		}
		else if (Function == "for_fishing")
		{
			if (MainCharacter::getInstance()->getCurrentItemType() != ItemType::FISHING_ROD)
				return;
			if (!MainCharacter::getInstance()->modifyEnergy(MainCharacter::getInstance()->Fishing_consumes_energy))
				return;
			UILogic::getInstance()->refreshFishUI();
			SceneManager::getInstance()->showUILayer("fish");
			UILogic::getInstance()->completeTask(3);
		}
		// no function
		else {
			SM->NextMap(NextMap, NextPosition);
		}

	}
	else {
		CCLOG("Error: SceneManager instance is null!");
	}
}

void Gate::init()
{
	info_.sprite = nullptr;
}

void Gate::clear()
{
	info_.sprite = nullptr;
}

void Gate::pause()
{

}

void Gate::resume()
{

}


void Gate::settle()
{
	if (TimeManager::getInstance()->isFestivalDay()&&NextMap=="town") {
		NextMap = "town_festival";
	}
	if (NextMap == "town_festival" && !(TimeManager::getInstance()->isFestivalDay())) {
		NextMap = "town";
	}
}

bool Gate::hasCollision()
{
	return false;
}
