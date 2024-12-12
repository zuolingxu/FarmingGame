#include "Gate.h"
#include "json/document.h"
#include "SceneManager.h"

Gate::Gate(MapLayer* parent, const Vec<int>& pos,std::string NM,std::string NP) : parent(parent), MapObject(pos),NextMap(NM),NextPosition(NP)
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

		return new Gate(parent, pos, NextMap, NextPosition);
	}
	return nullptr;
}

void Gate::interact()
{
	if (SceneManager* SM = SceneManager::getInstance()) {
		SM->NextMap(NextMap, NextPosition);
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

}

bool Gate::hasCollision()
{
	return false;
}
