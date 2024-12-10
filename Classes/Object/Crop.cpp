#include "Crop.h"

Crop::Crop(MapLayer* parent, const Vec<int>& pos) : parent_(parent), LiveDay(1), MaturationDay(7)
{
	
}

Crop::~Crop()
{
	
}


Crop* Crop::create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos) {
    Crop* crop = new Crop(parent,pos);

    if (val.HasMember("CropName")) {
        crop->CropName = val["CropName"].GetString();
    }
    if (val.HasMember("LiveDay")) {
        crop->LiveDay = val["LiveDay"].GetInt();
    }
    if (val.HasMember("MaturationDay")) {
        crop->MaturationDay = val["MaturationDay"].GetInt();
    }

    return crop;
}

Crop* Crop::createByPlayer(const cocos2d::Vec2& position, MapLayer* parent, Land* land) {
    Crop* crop = new Crop(parent,position);
    crop->saveToArchive(position, land); // ���� Land ����
    return crop;
}

void Crop::saveToArchive(const cocos2d::Vec2& position, Land* land) {
    DocumentManager* docManager = DocumentManager::getInstance();
    rapidjson::Document* archiveDoc = docManager->getArchiveDocument();

    std::string positionKey = std::to_string(static_cast<int>(position.x)) + " " + std::to_string(static_cast<int>(position.y));

    rapidjson::Value cropInfo(rapidjson::kObjectType);
    cropInfo.AddMember("CropName", rapidjson::Value(CropName.c_str(), archiveDoc->GetAllocator()), archiveDoc->GetAllocator());
    cropInfo.AddMember("LiveDay", LiveDay, archiveDoc->GetAllocator());
    cropInfo.AddMember("MaturationDay", MaturationDay, archiveDoc->GetAllocator());
    //cropInfo.AddMember("Water", land->isWatered(), archiveDoc->GetAllocator()); // ʹ�� Land �� Water ״̬
    //cropInfo.AddMember("Fertilizer", land->isFertilized(), archiveDoc->GetAllocator()); // ʹ�� Land �� Fertilizer ״̬

    if (archiveDoc->HasMember("Map")) {
        rapidjson::Value& map = (*archiveDoc)["Map"];
        if (map.HasMember("Farm")) {
            map["Farm"].AddMember(rapidjson::Value(positionKey.c_str(), archiveDoc->GetAllocator()), cropInfo, archiveDoc->GetAllocator());
        }
        else {
            rapidjson::Value farm(rapidjson::kObjectType);
            farm.AddMember(rapidjson::Value(positionKey.c_str(), archiveDoc->GetAllocator()), cropInfo, archiveDoc->GetAllocator());
            map.AddMember("Farm", farm, archiveDoc->GetAllocator());
        }
    }
    else {
        rapidjson::Value map(rapidjson::kObjectType);
        rapidjson::Value farm(rapidjson::kObjectType);
        farm.AddMember(rapidjson::Value(positionKey.c_str(), archiveDoc->GetAllocator()), cropInfo, archiveDoc->GetAllocator());
        map.AddMember("Farm", farm, archiveDoc->GetAllocator());
        (*archiveDoc).AddMember("Map", map, archiveDoc->GetAllocator());
    }
}

void Crop::interact()
{
	
}

void Crop::init()
{
	
}

void Crop::clear()
{
	sprite_ = nullptr;
}


void Crop::settle()
{
	
}
