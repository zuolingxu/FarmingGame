#include "Crop.h"

Crop::Crop(MapLayer* parent) : parent_(parent)
{
	
}

Crop::~Crop()
{
	
}


Crop* Crop::create(rapidjson::Value& val, MapLayer* parent)
{
	return new Crop(parent);
}

void Crop::interact()
{
	
}

void Crop::init()
{
	
}

void Crop::clear()
{
	sprite_ = nullptr;
}


