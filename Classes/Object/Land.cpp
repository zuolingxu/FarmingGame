#include "Land.h"

USING_NS_CC;

Land::Land(MapLayer* parent, const Vec<int>& pos) : MapObject(pos), parent_(parent), Water(false), Fertilizer(false) {}

Land::~Land() {
    if (crop_) {
        delete crop_;
        crop_ = nullptr;
    }
}

MapObject* Land::create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos) {
    Land* land = new Land(parent, pos);

    if (val.HasMember("HasCrop") && val["HasCrop"].GetBool()) {
        if (val.HasMember("CropInfo") && val["CropInfo"].IsObject()) {
            rapidjson::Value& cropInfo = val["CropInfo"];
            land->crop_ = Crop::create(cropInfo, parent,pos);
        }
    }

    if (val.HasMember("Water")) {
        land->Water = val["Water"].GetBool();
    }
    if (val.HasMember("Fertilizer")) {
        land->Fertilizer = val["Fertilizer"].GetBool();
    }

    return land;
}

MapObject* Land::createByPlayer(const cocos2d::Vec2& position, MapLayer* parent) {
    Land* land = new Land(parent, { static_cast<int>(position.x), static_cast<int>(position.y) });
    land->saveToArchive(position);
    return land;
}

void Land::saveToArchive(const cocos2d::Vec2& position) {
    auto* docManager = DocumentManager::getInstance();
    auto* archiveDoc = docManager->getArchiveDocument();

    std::string positionKey = std::to_string(static_cast<int>(position.x)) + " " + std::to_string(static_cast<int>(position.y));

    rapidjson::Value landInfo(rapidjson::kObjectType);
    landInfo.AddMember("Type", "Land", archiveDoc->GetAllocator());
    landInfo.AddMember("HasCrop", crop_ != nullptr, archiveDoc->GetAllocator());
    landInfo.AddMember("Water", Water, archiveDoc->GetAllocator());
    landInfo.AddMember("Fertilizer", Fertilizer, archiveDoc->GetAllocator());

    if (crop_) {
        rapidjson::Value cropInfo(rapidjson::kObjectType);
        cropInfo.AddMember("CropName", rapidjson::Value(crop_->getCropName().c_str(), archiveDoc->GetAllocator()), archiveDoc->GetAllocator());
        cropInfo.AddMember("LiveDay", crop_->getLiveDay(), archiveDoc->GetAllocator());
        cropInfo.AddMember("MaturationDay", crop_->getMaturationDay(), archiveDoc->GetAllocator());
        landInfo.AddMember("CropInfo", cropInfo, archiveDoc->GetAllocator());
    }

    if (!archiveDoc->HasMember("Map")) {
        rapidjson::Value map(rapidjson::kObjectType);
        archiveDoc->AddMember("Map", map, archiveDoc->GetAllocator());
    }

    rapidjson::Value& map = (*archiveDoc)["Map"];
    if (!map.HasMember("Farm")) {
        rapidjson::Value farm(rapidjson::kObjectType);
        map.AddMember("Farm", farm, archiveDoc->GetAllocator());
    }

    rapidjson::Value& farm = map["Farm"];
    farm.AddMember(rapidjson::Value(positionKey.c_str(), archiveDoc->GetAllocator()), landInfo, archiveDoc->GetAllocator());
}

void Land::init() {}

void Land::interact() {
    auto* character = Character::getInstance();

    if (character->hasItem(Character::ItemType::FERTILIZER)) {
        Fertilizer = true;
    } else if (character->hasItem(Character::ItemType::SEED)) {
        if (!crop_) {
            crop_ = Crop::createByPlayer(cocos2d::Vec2(12, 13), parent_,this);
        }
    } else if (character->hasItem(Character::ItemType::WATERING_CAN)) {
        if (crop_) {
            // Logic for watering crops
        }
    }
}

void Land::clear() {
    info_.sprite = nullptr;
}

void Land::pause() {}

void Land::resume() {}

void Land::settle() {
    if (crop_) crop_->settle();
}

bool Land::hasCollision() {
    return false;
}
