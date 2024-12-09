#include "MapObject.h"
#include "Animal.h"
#include "NPC.h"
#include "Land.h"

MapObject::MapObject(const Vec<int>& pos) : info_(nullptr, {}, pos) { }

MapObject::ObjectInfo& MapObject::getInfo()
{
	return info_;
}

MapObject* MapObject::create(rapidjson::Value& val, MapLayer* parents, const Vec<int>& pos)
{
	std::string type = val["Type"].GetString();
    rapidjson::Value& SubVal = val["Info"];
    MapObject* ref = nullptr;
    if (type == "Animal")
    {
        ref = Animal::create(SubVal, parents, pos);
    }
    else if (type == "NPC")
    {
        ref = NPC::create(SubVal, parents, pos);
    }
    else if (type == "Land")
    {
        ref = Land::create(SubVal,parents, pos);
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
        CCLOG("MapObject::create() failed");
    }
    return ref;
}
