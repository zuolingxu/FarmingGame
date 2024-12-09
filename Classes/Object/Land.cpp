#include "Land.h"

USING_NS_CC;

Land::Land(MapLayer* parent, const Vec<int>& pos) : MapObject(pos), parent_(parent)
{
	
}

Land::~Land()
{
	
}


::MapObject* Land::create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos)
{
	Land* land = new Land(parent, pos);
	if (val.HasMember("SubVal"))
	{
		rapidjson::Value SubVal;
		land->crop_ = Crop::create(SubVal, parent, pos);
	}

	return land;
}

void Land::init()
{
	info_.sprite = nullptr;
}


void Land::interact()
{
	
}

void Land::clear()
{
	info_.sprite = nullptr;
}

void Land::pause()
{
	
}

void Land::resume()
{
	
}

void Land::settle()
{
	
}

bool Land::hasCollision()
{
	return true;
}
