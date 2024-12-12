#include "Crop.h"
#include "Land.h"
#include "MainCharacter.h"
#include "TimeManager.h"
#include "HelperClasses.h"

Crop::Crop(Vec<int> position, MapLayer* parent, std::string CropName, bool Water, int LiveDay, int MaturationDay)
    : info{ nullptr, {1, 1}, position },  // Initialize ObjectInfo
    parent(parent), CropName(std::move(CropName)), Water(Water), LiveDay(LiveDay),
    MaturationDay(MaturationDay)
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
    Crop* crop = new Crop(position, parent, cropName, false, LiveDay, MaturationDay);

    return crop;
}

Crop* Crop::createByPlayer(const Vec<int>& position, MapLayer* parent, const std::string& CropName, bool Fertilizer) {
    int LiveDay = 1;
    if (Fertilizer) LiveDay++;  // If fertilizer is used, increase the growth days by 1
    // Create the Crop object using the provided CropName
    
    DocumentManager* manager = DocumentManager::getInstance();
    rapidjson::Document* doc = manager->getDocument(manager->getPath(CropName));
    int MaturationDay = (*doc)["MaturationDay"].GetInt();

    Crop* crop = new Crop(position, parent, CropName, false, LiveDay, MaturationDay);
    return crop;
}

//harvest
bool Crop::harvest_successful() {
    if (LiveDay == MaturationDay) {
        // Remove the sprite from the scene
        info.sprite->removeFromParent();
        info.sprite = nullptr;

        // Create the corresponding ItemType based on the uppercase CropName
        ItemType harvestedItemType = Item::stringToItemType(CropName);

        // Add the harvested crop to the character's inventory
        MainCharacter* character = MainCharacter::getInstance();
        character->modifyItemQuantity(harvestedItemType, 1);

        return true;
    }
    return false;
}

void Crop::init() {
    DocumentManager* manager = DocumentManager::getInstance();

    rapidjson::Document* doc = manager->getDocument(manager->getPath(CropName));
    std::string plistFilePath = (*doc)["plistpath"].GetString();

    std::string frame_n_format = (*doc)["frame_format"].GetString();
    std::string spriteframe = getFrameName(frame_n_format, LiveDay);      // Plant days are from 1, 0 is plant wilt

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
    std::string currentSeason = TimeManager::getInstance()->getSeason();  // Get current season from TimeManager
    int deltaliveday = 0;
    if (currentSeason == "spring") {
        deltaliveday = 1;  // Spring: 1 LiveDay per day
    }
    else if (currentSeason == "autumn") {
        deltaliveday = 2;  // Autumn: 2 LiveDays per day
    }
    if (LiveDay < MaturationDay && Water) {
        LiveDay+=deltaliveday;  // Increment the LiveDay if the crop hasn't matured and has been watered
        if (LiveDay > MaturationDay)
            LiveDay = MaturationDay;

        DocumentManager* manager = DocumentManager::getInstance();

        rapidjson::Document* doc = manager->getDocument(manager->getPath(CropName));
        std::string plistFilePath = (*doc)["plistpath"].GetString();

        std::string frame_n_format = (*doc)["frame_format"].GetString();
        std::string spriteframe = getFrameName(frame_n_format, LiveDay);      // Plant days are from 1, 0 is plant wilt

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

    Water = false;  // Reset the Water status after updating the crop's growth

    // The crop's change_archive_in_memory will be called by Land::settle()
}
