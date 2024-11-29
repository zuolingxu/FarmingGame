#include "Object.h"
#include "Animal.h"
#include "NPC.h"
#include "Land.h"

Object::~Object()
{
}


Object* Object::create(rapidjson::Value& val)
{
	std::string type = val[0].GetString();
    rapidjson::Value& SubVal = val[1];
    if (type == "Animal")
    {
        return Animal::create(SubVal);
    }
    else if (type == "NPC")
    {
        return NPC::create(SubVal);
    }
    else if (type == "Land")
    {
        return Land::create(SubVal);
    }
    else
    {
        throw std::runtime_error("Archive has been crupted");
    }

}
