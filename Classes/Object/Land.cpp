#include "Land.h"

USING_NS_CC;

Land::Land()
{
	
}

Land::~Land()
{
	
}


Object* Land::create(rapidjson::Value& val)
{
	Land* land = new Land;
	if (val.HasMember("SubVal"))
	{
		rapidjson::Value SubVal;
		land->crop_ = Crop::create(SubVal);
	}

	return land;
}

void Land::init()
{
	
}


void Land::interact()
{
	
}
