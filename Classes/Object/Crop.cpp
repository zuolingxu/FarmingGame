#include "Crop.h"
#include "Land.h"


Crop::Crop(Vec<int> position, MapLayer* parent, std::string CropName, bool Water, int LiveDay, int MaturationDay)
    : info{ nullptr,{1,1},position },        // 初始化 ObjectInfo
    parent(parent),     // 初始化 parent 指针
    CropName(std::move(CropName)), // 移动语义用于初始化 CropName
    Water(Water),       // 初始化 Water 状态
    LiveDay(LiveDay),   // 初始化 LiveDay
    MaturationDay(MaturationDay)  // 初始化 MaturationDay
{

}


Crop::~Crop()
{
	
}


Crop* Crop::create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& position) {
    std::string cropName = "unknown";  // 默认的作物名称
    int LiveDay = 1;                   // 默认的生长天数
    int MaturationDay = 7;             // 默认的成熟天数

    if (val.HasMember("CropName")) {
        cropName = val["CropName"].GetString();
    }
    if (val.HasMember("LiveDay")) {
        LiveDay = val["LiveDay"].GetInt();
    }
    if (val.HasMember("MaturationDay")) {
        MaturationDay = val["MaturationDay"].GetInt();
    }
    Crop* crop = new Crop(position, parent, std::move(cropName), false, LiveDay, MaturationDay);

    return crop;
}

Crop* Crop::createByPlayer(const Vec<int>& position, MapLayer* parent, Land* land, const std::string& CropName,bool Fertilizer) {
    int LiveDay = 1;
    if (Fertilizer)
        LiveDay++;
    // 使用传入的 CropName 来创建 Crop 对象
    Crop* crop = new Crop(position, parent,CropName, false, LiveDay, 7);  // 默认初始化 Water 为 false, MaturationDay 为 7
    return crop;
}

void Crop::interact()
{
	
}

void Crop::init()
{
    std::string plistFilePath = CropName+"Pls";
    std::string livedaystr = std::to_string(LiveDay - 1); // plant from 1 ,day png from 0
    std::string spriteframe = CropName + "-" + livedaystr + ".png";
    DocumentManager* manager = DocumentManager::getInstance();
    if (FileUtils::getInstance()->isFileExist(plistFilePath)) {
        // 如果路径有效，加载 Plist 文件
        MapLayer::loadPlist(manager->getPath(plistFilePath));
    }
    else {
        CCLOG("Error: Plist file %s not found!", plistFilePath.c_str());
        return; // 退出函数，避免后续操作出错
    }

    // 确保 parent 不为空，防止空指针访问
    if (parent != nullptr) {
        // 使用加载的 Plist 创建精灵，确保精灵帧文件存在
        parent->addSpriteWithFrame(info, spriteframe);
    }
    else {
        CCLOG("Error: parent is nullptr!");
    }
}

void Crop::clear()
{
	info.sprite = nullptr;
}


void Crop::settle()
{
    if (LiveDay < MaturationDay && Water) {
        LiveDay++;
        std::string plistFilePath = CropName + "Pls";
        std::string livedaystr = std::to_string(LiveDay - 1);
        std::string spriteframe = CropName + "-" + livedaystr + ".png";
        DocumentManager* manager = DocumentManager::getInstance();
        
        if (FileUtils::getInstance()->isFileExist(plistFilePath)) {
            // 如果路径有效，加载 Plist 文件
            MapLayer::loadPlist(manager->getPath(plistFilePath));
        }
        else {
            CCLOG("Error: Plist file %s not found!", plistFilePath.c_str());
            return; // 退出函数，避免后续操作出错
        }

        // 确保 parent 不为空，防止空指针访问
        if (parent != nullptr) {
            // 使用加载的 Plist 创建精灵，确保精灵帧文件存在
            parent->changeWithSingleFrame(info.sprite, spriteframe);
        }
        else {
            CCLOG("Error: parent is nullptr!");
        }
    }
       
    Water = 0;

    // crop's change_archive_in_memory be called by Land::settle()
}
