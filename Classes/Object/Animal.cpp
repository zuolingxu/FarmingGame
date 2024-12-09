#include "Animal.h"

Animal::Animal(MapLayer* parent)
	: parent_(parent), name(""), isSold(false), position(0, 0){
	
}

Animal::~Animal()
{

}


Object* Animal::create(rapidjson::Value& val, MapLayer* parent)
{
	return nullptr;
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

