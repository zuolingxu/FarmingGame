#include "Crop.h"
#include "Land.h"
#include "MainCharacter.h"

Crop::Crop(Vec<int> position, MapLayer* parent, std::string CropName, bool Water, int LiveDay, int MaturationDay)
    : info{ nullptr, {1, 1}, position },  // Initialize ObjectInfo
    parent(parent),     // Initialize parent pointer
    CropName(std::move(CropName)), // Use move semantics to initialize CropName
    Water(Water),       // Initialize Water status
    LiveDay(LiveDay),   // Initialize LiveDay
    MaturationDay(MaturationDay)  // Initialize MaturationDay
{

}

Crop::~Crop() {
    // Destructor (empty for now)
}

//create by archive
Crop* Crop::create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& position) {
    std::string cropName = "unknown";  // Default crop name
    int LiveDay = 1;                   // Default growth days
    int MaturationDay = 7;             // Default maturation days

    if (val.HasMember("CropName")) {
        cropName = val["CropName"].GetString();  // Get crop name from JSON
    }
    if (val.HasMember("LiveDay")) {
        LiveDay = val["LiveDay"].GetInt();  // Get growth days from JSON
    }
    if (val.HasMember("MaturationDay")) {
        MaturationDay = val["MaturationDay"].GetInt();  // Get maturation days from JSON
    }
    Crop* crop = new Crop(position, parent, std::move(cropName), false, LiveDay, MaturationDay);

    return crop;
}

Crop* Crop::createByPlayer(const Vec<int>& position, MapLayer* parent, const std::string& CropName, bool Fertilizer) {
    int LiveDay = 1;
    if (Fertilizer) LiveDay++;  // If fertilizer is used, increase the growth days by 1
    // Create the Crop object using the provided CropName
    Crop* crop = new Crop(position, parent, CropName, false, LiveDay, 7);  // Default: Water = false, MaturationDay = 7
    return crop;
}

//harvest
bool Crop::harvest_successful() {
    if (LiveDay == MaturationDay) {
        // Remove the sprite from the scene
        info.sprite->removeFromParent();
        info.sprite = nullptr;

        // Create the corresponding ItemType based on the uppercase CropName
        ItemType harvestedItemType;

        if (CropName == "cauliflower") {
            harvestedItemType = ItemType::CAULIFLOWER;
        }
        else if (CropName == "pumkin") {
            harvestedItemType = ItemType::PUMPKIN;
        }
        else if (CropName == "potato") {
            harvestedItemType = ItemType::POTATO;
        }
        else {
            // Handle the case where the crop name is not recognized
            return false;
        }

        // Add the harvested crop to the character's inventory
        MainCharacter* character = MainCharacter::getInstance();
        character->modifyItemQuantity(harvestedItemType, 1);

        return true;
    }
    return false;
}

void Crop::init() {
    std::string plistFilePath = CropName + "Pls";  // The plist file path for the crop's sprite
    std::string livedaystr = std::to_string(LiveDay - 1); // Plants start from day 1, so day 0 corresponds to LiveDay - 1
    std::string spriteframe = CropName + "-" + livedaystr + ".png";  // Construct the sprite frame filename
    DocumentManager* manager = DocumentManager::getInstance();

    if (FileUtils::getInstance()->isFileExist(plistFilePath)) {
        // If the plist file exists, load it
        MapLayer::loadPlist(manager->getPath(plistFilePath));
    }
    else {
        CCLOG("Error: Plist file %s not found!", plistFilePath.c_str());
        return; // Exit the function to avoid further errors
    }

    // Ensure parent is not null to avoid null pointer access
    if (parent != nullptr) {
        // Use the loaded plist to create the sprite, ensuring the sprite frame exists
        parent->addSpriteWithFrame(info, spriteframe);
    }
    else {
        CCLOG("Error: parent is nullptr!");
    }
}

void Crop::clear() {
    info.sprite = nullptr;  // Clear the sprite reference
}

void Crop::settle() {
    if (LiveDay < MaturationDay && Water) {
        LiveDay++;  // Increment the LiveDay if the crop hasn't matured and has been watered
        std::string plistFilePath = CropName + "Pls";  // The plist file path for the crop's sprite
        std::string livedaystr = std::to_string(LiveDay - 1);  // Plant days are from 1, so we adjust for zero-based index
        std::string spriteframe = CropName + "-" + livedaystr + ".png";  // Construct the sprite frame filename
        DocumentManager* manager = DocumentManager::getInstance();

        if (FileUtils::getInstance()->isFileExist(plistFilePath)) {
            // If the plist file exists, load it
            MapLayer::loadPlist(manager->getPath(plistFilePath));
        }
        else {
            CCLOG("Error: Plist file %s not found!", plistFilePath.c_str());
            return; // Exit the function to avoid further errors
        }

        // Ensure parent is not null to avoid null pointer access
        if (parent != nullptr) {
            // Use the loaded plist to create the sprite and update the sprite frame
            parent->changeWithSingleFrame(info.sprite, spriteframe);
        }
        else {
            CCLOG("Error: parent is nullptr!");
        }
    }

    Water = 0;  // Reset the Water status after updating the crop's growth

    // The crop's change_archive_in_memory will be called by Land::settle()
}
