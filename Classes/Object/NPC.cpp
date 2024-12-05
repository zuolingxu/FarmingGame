#include "NPC.h"

NPC::NPC(MapLayer* parent) : parent_(parent)
{
}

NPC::~NPC()
{
	
}

Object* NPC::create(rapidjson::Value& val, MapLayer* parent)
{
	return new NPC(parent);
}

void NPC::interact()
{
	
}

void NPC::init()
{
	info_.sprite = nullptr;
}

void NPC::clear()
{
	info_.sprite = nullptr;
}

void NPC::pause()
{
	
}

void NPC::resume()
{
	
}


void NPC::settle()
{
	
}

bool NPC::hasCollision()
{
	return true;
}
