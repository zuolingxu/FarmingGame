#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "json/document.h"
#include "MapLayer.h"
#include <string>

USING_NS_CC;

class Land;

class Crop{
	private:
	MapObject::ObjectInfo info;
	MapLayer* parent;
	std::string CropName;

	bool Water;
	int LiveDay;
	const int MaturationDay;

	public:
	explicit Crop(Vec<int> position, MapLayer* parent, std::string CropName, bool Water, int LiveDay, int MaturationDay);
	~Crop();
	static Crop* create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos);
	static Crop* createByPlayer(const Vec<int>& position, MapLayer* parent, const std::string& CropName,bool Fertilizer);

	void init();
	bool harvest_successful();
	void clear();
	void settle();

	std::string getCropName() const { return CropName; }
	bool getWater() const { return Water; }
	int getLiveDay() const { return LiveDay; }
	int getMaturationDay() const { return MaturationDay; }

	void change_to_watered() { Water = 1; }
}; 

