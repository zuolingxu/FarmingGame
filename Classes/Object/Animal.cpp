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
	
}

void Animal::interact()
{
	
}

void Animal::clear()
{

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

