#include "Land.h"
#include "MainCharacter.h"

USING_NS_CC;

Land::Land(const Vec<int>& pos, MapLayer* parent, bool Fertilizer)
    : MapObject(pos), parent(parent), crop(nullptr), Fertilizer(false) {
    info_.size = Vec<int>(1, 1);    // The plant occupies a space of 1x1
}

Land::~Land() {
    if (crop) {
        delete crop;
        crop = nullptr;  // Delete the crop object if it exists
    }
}

MapObject* Land::create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos) {

    Land* land = new Land(pos, parent, false);

    if (val.HasMember("HasCrop") && val["HasCrop"].GetBool()) {
        // If there is crop information (CropInfo), create a Crop object
        if (val.HasMember("CropInfo") && val["CropInfo"].IsObject()) {
            rapidjson::Value& cropInfo = val["CropInfo"];
            land->crop = Crop::create(cropInfo, parent, pos);  // Create the Crop object using Crop::create
        }
    }

    // Process whether there is fertilizer (Fertilizer)
    if (val.HasMember("Fertilizer")) {
        land->Fertilizer = val["Fertilizer"].GetBool();  // Get the fertilizer status
    }

    return land;  // Return the created Land object
}

MapObject* Land::createByPlayer(const Vec<int>& position, MapLayer* parent) {
    Land* land = new Land(position, parent, false);

    land->create_archive_in_memory(position);  // Create and save the land data in memory

    return land;
}

void Land::create_archive_in_memory(const Vec<int>& position) {
    // Get the DocumentManager and the current archive document
    auto* docManager = DocumentManager::getInstance();
    auto* archiveDoc = docManager->getArchiveDocument();

    // Ensure the document exists
    if (!archiveDoc) {
        CCLOG("Archive document is null!");
        return;
    }

    // Get the Map node (or create a Farm node)
    rapidjson::Value& map = (*archiveDoc)["Map"];
    if (!map.HasMember("Farm")) {
        map.AddMember("Farm", rapidjson::Value(rapidjson::kObjectType), archiveDoc->GetAllocator());
    }

    rapidjson::Value& farm = map["Farm"];

    // Convert position to a string to use as the map key
    std::string positionKey = std::to_string(position.X()) + " " + std::to_string(position.Y());

    // Create a data structure for the current Land object
    rapidjson::Value landData(rapidjson::kObjectType);
    rapidjson::Value info(rapidjson::kObjectType);

    // Record whether there is a crop
    info.AddMember("HasCrop", crop != nullptr, archiveDoc->GetAllocator());

    // Record whether fertilizer is used
    info.AddMember("Fertilizer", Fertilizer, archiveDoc->GetAllocator());

    // Record crop information if there is a crop
    if (crop != nullptr) {
        rapidjson::Value cropInfo(rapidjson::kObjectType);

        cropInfo.AddMember("CropName", rapidjson::Value(crop->getCropName().c_str(), archiveDoc->GetAllocator()), archiveDoc->GetAllocator());
        cropInfo.AddMember("Water", crop->getWater(), archiveDoc->GetAllocator());
        cropInfo.AddMember("LiveDay", crop->getLiveDay(), archiveDoc->GetAllocator());
        cropInfo.AddMember("MaturationDay", crop->getMaturationDay(), archiveDoc->GetAllocator());

        info.AddMember("CropInfo", cropInfo, archiveDoc->GetAllocator());
    }

    // Store the info data into landData
    landData.AddMember("Info", info, archiveDoc->GetAllocator());

    // Store the Land data at the corresponding position under "Farm"
    farm.AddMember(rapidjson::Value(positionKey.c_str(), archiveDoc->GetAllocator()), landData, archiveDoc->GetAllocator());
}

void Land::change_archive_in_memory(const Vec<int>& position) {
    // Get the DocumentManager instance and the current archive document
    auto* docManager = DocumentManager::getInstance();
    auto* archiveDoc = docManager->getArchiveDocument();

    // Convert position to a string to use as the map key
    std::string positionKey = std::to_string(position.X()) + " " + std::to_string(position.Y());

    // Find the corresponding Land object using positionKey
    if (archiveDoc->HasMember("Map") && (*archiveDoc)["Map"].HasMember("Farm")) {
        rapidjson::Value& farm = (*archiveDoc)["Map"]["Farm"];
        if (farm.HasMember(positionKey.c_str())) {
            rapidjson::Value& landInfo = farm[positionKey.c_str()];

            // Update Fertilizer status
            if (landInfo.HasMember("Info") && landInfo["Info"].IsObject()) {
                rapidjson::Value& info = landInfo["Info"];

                // Update Fertilizer status
                if (info.HasMember("Fertilizer")) {
                    info["Fertilizer"].SetBool(Fertilizer);  // Update based on the current Fertilizer status
                }

                // Update HasCrop status
                bool hasCrop = (crop != nullptr);  // If crop is not null, the Land has a crop
                if (info.HasMember("HasCrop")) {
                    info["HasCrop"].SetBool(hasCrop);
                }
                else {
                    info.AddMember("HasCrop", hasCrop, archiveDoc->GetAllocator());
                }

                // If there is a crop, update the crop information
                if (hasCrop && crop != nullptr) {
                    rapidjson::Value cropInfo(rapidjson::kObjectType);
                    cropInfo.AddMember("CropName", rapidjson::Value(crop->getCropName().c_str(), archiveDoc->GetAllocator()), archiveDoc->GetAllocator());
                    cropInfo.AddMember("Water", crop->getWater(), archiveDoc->GetAllocator());
                    cropInfo.AddMember("LiveDay", crop->getLiveDay(), archiveDoc->GetAllocator());
                    cropInfo.AddMember("MaturationDay", crop->getMaturationDay(), archiveDoc->GetAllocator());

                    info.AddMember("CropInfo", cropInfo, archiveDoc->GetAllocator());
                }
                else {
                    // If there is no crop, remove CropInfo
                    if (info.HasMember("CropInfo")) {
                        info.RemoveMember("CropInfo");
                    }
                }
            }
        }
    }
}

void Land::init() {
    // Initialize the land (if needed)
}

void Land::interact() {
    MainCharacter* maincharacter = MainCharacter::getInstance();
    const Item* currentItem = maincharacter->getCurrentItem();

    if (currentItem) {
        if (currentItem->type == ItemType::FERTILIZER) {
            Fertilizer = true;  // Apply fertilizer when the player interacts with the land
        }
    }
}

void Land::clear() {
    info_.sprite = nullptr;  // Clear the sprite information
}

void Land::pause() {
    // Pause any activities if necessary
}

void Land::resume() {
    // Resume any activities if necessary
}

void Land::settle() {
    if (crop) crop->settle();  // Settle the crop if there is one
    change_archive_in_memory(info_.position);  // Update the archive data
}

bool Land::hasCollision() {
    return false;  // No collision with land
}
