#include "NPC.h"

NPC::NPC()
{
}

NPC::~NPC()
{
	
}


Object* NPC::create(rapidjson::Value& val)
{
	return new NPC;
}

void NPC::interact()
{
	
}

void NPC::init()
{
	
}

