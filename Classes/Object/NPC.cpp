#include "NPC.h"

NPC::NPC(MapLayer* parent, const Vec<int>& pos) : MapObject(pos), parent_(parent)
{
}

NPC::~NPC()
{
	
}

MapObject* NPC::create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos)
{
	return new NPC(parent, pos);
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
