#include "Land.h"

USING_NS_CC;

Land::Land(MapLayer* parent) : parent_(parent)
{
	
}

Land::~Land()
{
	
}


::Object* Land::create(rapidjson::Value& val, MapLayer* parent)
{
	Land* land = new Land(parent);
	if (val.HasMember("SubVal"))
	{
		rapidjson::Value SubVal;
		land->crop_ = Crop::create(SubVal, parent);
	}

	return land;
}

void Land::init()
{
	
}


void Land::interact()
{
	
}

void Land::clear()
{
	sprite_ = nullptr;
}

void Land::settle()
{
	
}
