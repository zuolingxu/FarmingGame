#include "MapObject.h"
#include "Animal.h"
#include "NPC.h"
#include "Land.h"
#include "Mineral.h"
#include "Gate.h"
#include "MapObjectFactory.h"

MapObject::MapObject(const Vec<int>& pos) : info_{ nullptr, {}, pos } { }

MapObject::ObjectInfo& MapObject::getInfo()
{
	return info_;
}
