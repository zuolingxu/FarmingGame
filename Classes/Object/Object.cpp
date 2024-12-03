#include "Object.h"
#include "Animal.h"
#include "NPC.h"
#include "Land.h"

Object* Object::create(rapidjson::Value& val, MapLayer* parents)
{
	std::string type = val["Type"].GetString();
    rapidjson::Value& SubVal = val["Info"];
    Object* ref = nullptr;
    if (type == "Animal")
    {
        ref = Animal::create(SubVal, parents);
    }
    else if (type == "NPC")
    {
        ref = NPC::create(SubVal, parents);
    }
    else if (type == "Land")
    {
        ref = Land::create(SubVal,parents);
    }
    else
    {
        throw std::runtime_error("Archive has been corrupted");
    }

    if (ref)
    {
        ref->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ref);
        CCLOG("Object::create() failed");
    }
    return ref;
}