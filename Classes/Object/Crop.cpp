#include "Crop.h"

Crop::Crop(MapLayer* parent, const Vec<int>& pos) : parent_(parent)
{
	
}

Crop::~Crop()
{
	
}


Crop* Crop::create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos)
{
	return new Crop(parent, pos);
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

void Crop::pause()
{
	
}

void Crop::resume()
{
	
}

void Crop::settle()
{
	
}
