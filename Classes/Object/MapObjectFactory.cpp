#include "MapObjectFactory.h"
#include <stdexcept>

static std::unordered_map<std::string, MapObjectFactory::Creator>& registry() {
    static std::unordered_map<std::string, MapObjectFactory::Creator> inst;
    return inst;
}

void MapObjectFactory::registerCreator(const std::string& type, Creator c) {
    registry()[type] = std::move(c);
}

MapObject* MapObjectFactory::create(const std::string& type, rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos) {
    auto it = registry().find(type);
    if (it == registry().end()) {
        throw std::runtime_error("Unknown MapObject type: " + type);
    }
    return it->second(val, parent, pos);
}