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
	sprite_ = nullptr;
}

void NPC::clear()
{
	sprite_ = nullptr;
}

void NPC::settle()
{
	
}
