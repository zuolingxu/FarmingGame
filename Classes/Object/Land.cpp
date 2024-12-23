#include "Land.h"
#include "MainCharacter.h"

USING_NS_CC;

Land::Land(const Vec<int>& pos, MapLayer* parent, bool Fertilizer)
    : MapObject(pos), parent(parent), crop(nullptr), Fertilizer(false) {
    info_.size = Vec<int>(1, 1);    // The land occupies a space of 1x1
}

Land::~Land() {
    if (crop) {
        delete crop;
        crop = nullptr;  // Delete the crop object if it exists
    }
}

//create by archive
MapObject* Land::create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos) {

    Land* land = new Land(pos, parent, false);

    if (val.HasMember("HasCrop") && val["HasCrop"].GetBool()) {
        // If there is crop information (CropInfo), create a Crop object
        if (val.HasMember("CropInfo") && val["CropInfo"].IsObject()) {
            rapidjson::Value& cropInfo = val["CropInfo"];
            land->crop = Crop::create(cropInfo, parent, pos);  // Create the Crop object using Crop::create
            land->crop->init();  // generate a picture
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

    land->init();
    return land;
}

// Map farm in memory
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
    if (!map.HasMember("farm")) {
        map.AddMember("farm", rapidjson::Value(rapidjson::kObjectType), archiveDoc->GetAllocator());
    }

    rapidjson::Value& farm = map["farm"];

    // Convert position to a string to use as the map key
    std::string positionKey = std::to_string(position.X()) + " " + std::to_string(position.Y());

    // Create a data structure for the current Land object
    rapidjson::Value landData(rapidjson::kObjectType);
    rapidjson::Value info(rapidjson::kObjectType);

    // Add the "Type": "Land" field to indicate this is a land object
    landData.AddMember("Type", rapidjson::Value("Land", archiveDoc->GetAllocator()), archiveDoc->GetAllocator());

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
    if (archiveDoc->HasMember("Map") && (*archiveDoc)["Map"].HasMember("farm")) {
        rapidjson::Value& farm = (*archiveDoc)["Map"]["farm"];
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
                    info.RemoveMember("CropInfo");

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

    DocumentManager* manager = DocumentManager::getInstance();

    std::string plistFilePath = manager->getPath("LandPls");  // The plist file path for the land's sprite
    
    // identify is water
    std::string is_water = "0";
    if (crop) {
        is_water = std::to_string(crop->getWater());
    }
    // init sprite
    std::string spriteframe = "Land-" +is_water+".png";  // Construct the sprite frame filename

    if (FileUtils::getInstance()->isFileExist(plistFilePath)) {
        // If the plist file exists, load it
        MapLayer::loadPlist(plistFilePath);
    }
    else {
        CCLOG("Error: Plist file %s not found!", plistFilePath.c_str());
        return; // Exit the function to avoid further errors
    }

    // Ensure parent is not null to avoid null pointer access
    if (parent != nullptr) {
        // Use the loaded plist to create the sprite, ensuring the sprite frame exists
        parent->addSpriteWithFrame(info_, spriteframe);
    }
    else {
        CCLOG("Error: parent is nullptr!");
    }

    if (crop) {
        crop->init();
    }
}

void Land::interact()
{
    MainCharacter* maincharacter = MainCharacter::getInstance();
    const Item* currentItem = maincharacter->getCurrentItem();

    if (crop) {
        if (crop->harvest_successful()) {
            delete crop;
            crop = nullptr;
        }
    }

    if (currentItem) {

        if (currentItem->type == ItemType::NONE) {

        }
        // haven t fertililzer,no crop
        else if (currentItem->type == ItemType::FERTILIZER && crop == nullptr && Fertilizer == 0) {
            if(MainCharacter::getInstance()->modifyItemQuantity(ItemType::FERTILIZER,-1)){
                MainCharacter::getInstance()->modifyEnergy(MainCharacter::getInstance()->Fertilizing_consumes_energy);
                Fertilizer = true;
            }
        }
        else if (currentItem->type == ItemType::CAULIFLOWER_SEED ||
            currentItem->type == ItemType::PUMPKIN_SEED ||
            currentItem->type == ItemType::POTATO_SEED)
             {
            std::string cropName;
            if (currentItem->type == ItemType::CAULIFLOWER_SEED) {
                cropName = "cauliflower";
            }
            else if (currentItem->type == ItemType::PUMPKIN_SEED) {
                cropName = "pumpkin";
            }
            else if (currentItem->type == ItemType::POTATO_SEED) {
                cropName = "potato";
            }

            //no crop , plant
            if (!crop) 
            {
               if(MainCharacter::getInstance()->modifyItemQuantity(currentItem->type, -1))
               {
                    crop = Crop::createByPlayer(info_.position, parent, cropName, Fertilizer);
                }
            }
             }
        // can watering only if have a crop and havent been watered
        else if (currentItem->type == ItemType::WATERING_CAN && crop && !(crop->getWater())) {

            MainCharacter::getInstance()->modifyEnergy(MainCharacter::getInstance()->Watering_crops_consumes_energy);
            crop->change_to_watered();

            change_archive_in_memory(info_.position);  // Update the archive data

            //change show
            std::string plistFilePath = DocumentManager::getInstance()->getPath("LandPls");  // The plist file path for the land's sprite
            std::string spriteframe = "Land-1.png";  // Construct the sprite frame filename next day water is false
            DocumentManager* manager = DocumentManager::getInstance();

            if (FileUtils::getInstance()->isFileExist(plistFilePath)) {
                // If the plist file exists, load it
                MapLayer::loadPlist(plistFilePath);
            }
            else {
                CCLOG("Error: Plist file %s not found!", plistFilePath.c_str());
                return; // Exit the function to avoid further errors
            }

            // Ensure parent is not null to avoid null pointer access
            if (parent != nullptr) {
                // Use the loaded plist to create the sprite and update the sprite frame
                parent->changeWithSingleFrame(info_.sprite, spriteframe);
            }
            else {
                CCLOG("Error: parent is nullptr!");
            }
        }

    }

    init();  //generate picture
}

void Land::clear() {
    if (crop)
    {
        crop->clear();
    }

    info_.sprite = nullptr;  // Clear the sprite information
}

void Land::pause() {
    // Pause any activities if necessary
}

void Land::resume() {
    // Resume any activities if necessary
}

void Land::settle() {
    if (crop) {
        crop->settle();  // Settle the crop if there is one
    }
    change_archive_in_memory(info_.position);  // Update the archive data

    if (crop) {
        crop->init();       // generate sprite
    }

    DocumentManager* manager = DocumentManager::getInstance();
    rapidjson::Document* doc = manager->getDocument(manager->getPath("Land"));
    std::string plistFilePath = (*doc)["plistpath"].GetString();  // The plist file path for the land's sprite
    std::string spriteframe = "Land-0.png";  // Construct the sprite frame filename next day water is false
  

    if (FileUtils::getInstance()->isFileExist(plistFilePath)) {
        // If the plist file exists, load it
        MapLayer::loadPlist(plistFilePath);
    }
    else {
        CCLOG("Error: Plist file %s not found!", plistFilePath.c_str());
        return; // Exit the function to avoid further errors
    }

    // Ensure parent is not null to avoid null pointer access
    if (parent != nullptr) {
        // Use the loaded plist to create the sprite and update the sprite frame
        parent->changeWithSingleFrame(info_.sprite, spriteframe);
    }
    else {
        CCLOG("Error: parent is nullptr!");
    }
}

bool Land::hasCollision() {
    return false;  // No collision with land
}
