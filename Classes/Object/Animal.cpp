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
	
}

void Animal::interact()
{
	
}

void Animal::clear()
{
	sprite_ = nullptr;
}

void Animal::settle()
{
	
}
