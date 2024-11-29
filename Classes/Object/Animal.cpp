#include "Animal.h"

Animal::Animal()
{
	
}

Animal::~Animal()
{
	
}


Object* Animal::create(rapidjson::Value& val)
{
	return new Animal;
}

void Animal::init()
{
	
}

void Animal::interact()
{
	
}

