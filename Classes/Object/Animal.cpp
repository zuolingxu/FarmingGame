#include "Animal.h"

Animal::Animal(MapLayer* parent) : parent_(parent)
{
	
}

Animal::~Animal()
{
	
}


Object* Animal::create(rapidjson::Value& val, MapLayer* parent)
{
	return new Animal(parent);
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

