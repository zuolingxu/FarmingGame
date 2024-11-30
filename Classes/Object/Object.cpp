#include "Object.h"
#include "Animal.h"
#include "NPC.h"
#include "Land.h"

Object* Object::create(rapidjson::Value& val, MapLayer* parents)
{
	std::string type = val["Type"].GetString();
    rapidjson::Value& SubVal = val["Info"];
    if (type == "Animal")
    {
        return Animal::create(SubVal, parents);
    }
    else if (type == "NPC")
    {
        return NPC::create(SubVal, parents);
    }
    else if (type == "Land")
    {
        return Land::create(SubVal,parents);
    }
    else
    {
        throw std::runtime_error("Archive has been corrupted");
    }

}
