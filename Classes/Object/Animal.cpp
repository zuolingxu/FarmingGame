#include "Animal.h"

Animal::Animal(MapLayer* parent, const Vec<int>& pos) : MapObject(pos), parent_(parent)
{
	
}

Animal::~Animal()
{
	
}


MapObject* Animal::create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos)
{
	return new Animal(parent, pos);
}

void Animal::init()
{
	info_.sprite = nullptr;
}

void Animal::interact()
{
	
}

void Animal::clear()
{
	info_.sprite = nullptr;
}

void Animal::pause()
{
	
}

void Animal::resume()
{
	
}


void Animal::settle()
{
	
}

bool Animal::hasCollision()
{
	return true;
}

