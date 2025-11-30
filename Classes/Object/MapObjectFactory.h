#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include "json/document.h"
#include "MapLayer.h"
#include "MapObject.h"

class MapObjectFactory {
public:
    using Creator = std::function<MapObject*(rapidjson::Value&, MapLayer*, const Vec<int>&)>;

    static void registerCreator(const std::string& type, Creator c);
    static MapObject* create(const std::string& type, rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos);
};