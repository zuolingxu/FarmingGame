#include "Land.h"
#include "MainCharacter.h"

USING_NS_CC;

Land::Land(const Vec<int>& pos, MapLayer* parent,  bool Fertilizer)
    : MapObject(pos), parent(parent), crop(nullptr),Fertilizer(false) {
    info_.size = Vec<int>(1, 1);    //固定植物占据体积1，1
}

Land::~Land() {
    if (crop) {
        delete crop;
        crop = nullptr;
    }
}

MapObject* Land::create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos) {
    
    Land* land = new Land( pos, parent, false); 

    if (val.HasMember("HasCrop") && val["HasCrop"].GetBool()) {
        // 如果有作物信息（CropInfo），创建 Crop 对象
        if (val.HasMember("CropInfo") && val["CropInfo"].IsObject()) {
            rapidjson::Value& cropInfo = val["CropInfo"];
            land->crop = Crop::create(cropInfo, parent, pos);  // 使用 Crop::create 创建 Crop 对象
        }
    }

    // 处理是否有肥料（Fertilizer）
    if (val.HasMember("Fertilizer")) {
        land->Fertilizer = val["Fertilizer"].GetBool();  // 获取肥料状态
    }

    return land;  // 返回创建的 Land 对象
}


MapObject* Land::createByPlayer(const Vec<int>& position, MapLayer* parent) 
{
    Land* land = new Land( position, parent, false);

    land->create_archive_in_memory(position);

    return land;
}

void Land::create_archive_in_memory(const Vec<int>& position) {
    // 获取 DocumentManager 和当前档案文档
    auto* docManager = DocumentManager::getInstance();
    auto* archiveDoc = docManager->getArchiveDocument();

    // 确保文档存在
    if (!archiveDoc) {
        CCLOG("Archive document is null!");
        return;
    }

    // 获取 Map 节点（或创建 Farm 节点）
    rapidjson::Value& map = (*archiveDoc)["Map"];
    if (!map.HasMember("Farm")) {
        map.AddMember("Farm", rapidjson::Value(rapidjson::kObjectType), archiveDoc->GetAllocator());
    }

    rapidjson::Value& farm = map["Farm"];

    // 将 position 转换为字符串，以便作为 Map 的键
    std::string positionKey = std::to_string(position.X()) + " " + std::to_string(position.Y());

    // 创建当前 Land 对象的数据结构
    rapidjson::Value landData(rapidjson::kObjectType);
    rapidjson::Value info(rapidjson::kObjectType);

    // 记录是否有作物
    info.AddMember("HasCrop", crop != nullptr, archiveDoc->GetAllocator());

    // 记录是否施肥
    info.AddMember("Fertilizer", Fertilizer, archiveDoc->GetAllocator());

    // 记录 Crop 信息（如果有作物）
    if (crop != nullptr) {
        rapidjson::Value cropInfo(rapidjson::kObjectType);

        cropInfo.AddMember("CropName", rapidjson::Value(crop->getCropName().c_str(), archiveDoc->GetAllocator()), archiveDoc->GetAllocator());
        cropInfo.AddMember("Water", crop->getWater(), archiveDoc->GetAllocator());
        cropInfo.AddMember("LiveDay", crop->getLiveDay(), archiveDoc->GetAllocator());
        cropInfo.AddMember("MaturationDay", crop->getMaturationDay(), archiveDoc->GetAllocator());

        info.AddMember("CropInfo", cropInfo, archiveDoc->GetAllocator());
    }

    // 将 info 数据存储到 landData 中
    landData.AddMember("Info", info, archiveDoc->GetAllocator());

    // 将该位置的 Land 数据存储到 "Farm" 下的对应位置
    farm.AddMember(rapidjson::Value(positionKey.c_str(), archiveDoc->GetAllocator()), landData, archiveDoc->GetAllocator());
}

void Land::change_archive_in_memory(const Vec<int>& position) {
    // 获取 DocumentManager 实例并获取当前存档文档
    auto* docManager = DocumentManager::getInstance();
    auto* archiveDoc = docManager->getArchiveDocument();

    // 将 position 转换为字符串，作为 Map 中的键
    std::string positionKey = std::to_string(position.X()) + " " + std::to_string(position.Y());

    // 查找该位置的 Land 对象（通过 positionKey）
    if (archiveDoc->HasMember("Map") && (*archiveDoc)["Map"].HasMember("Farm")) {
        rapidjson::Value& farm = (*archiveDoc)["Map"]["Farm"];
        if (farm.HasMember(positionKey.c_str())) {
            rapidjson::Value& landInfo = farm[positionKey.c_str()];

            // 更新 Fertilizer 状态
            if (landInfo.HasMember("Info") && landInfo["Info"].IsObject()) {
                rapidjson::Value& info = landInfo["Info"];

                // 更新 Fertilizer 状态
                if (info.HasMember("Fertilizer")) {
                    info["Fertilizer"].SetBool(Fertilizer);  // 根据当前 Fertilizer 状态进行更新
                }

                // 更新 HasCrop 状态
                bool hasCrop = (crop != nullptr);  // 如果 crop 不为空，表示当前 Land 有作物
                if (info.HasMember("HasCrop")) {
                    info["HasCrop"].SetBool(hasCrop);
                }
                else {
                    info.AddMember("HasCrop", hasCrop, archiveDoc->GetAllocator());
                }

                // 如果有作物，更新作物信息
                if (hasCrop && crop != nullptr) {
                    rapidjson::Value cropInfo(rapidjson::kObjectType);
                    cropInfo.AddMember("CropName", rapidjson::Value(crop->getCropName().c_str(), archiveDoc->GetAllocator()), archiveDoc->GetAllocator());
                    cropInfo.AddMember("Water", crop->getWater(), archiveDoc->GetAllocator());
                    cropInfo.AddMember("LiveDay", crop->getLiveDay(), archiveDoc->GetAllocator());
                    cropInfo.AddMember("MaturationDay", crop->getMaturationDay(), archiveDoc->GetAllocator());

                    info.AddMember("CropInfo", cropInfo, archiveDoc->GetAllocator());
                }
                else {
                    // 没有作物时，移除 CropInfo
                    if (info.HasMember("CropInfo")) {
                        info.RemoveMember("CropInfo");
                    }
                }
            }
        }
    }
}


void Land::init() 
{

}

void Land::interact() {
    MainCharacter* maincharacter = MainCharacter::getInstance();
    const MainCharacter::Item* currentItem = maincharacter->getCurrentItem();

    if (currentItem) {
        if (currentItem->type == MainCharacter::ItemType::FERTILIZER) {
            Fertilizer = true;  
        }
    }

}

void Land::clear() 
{
    info_.sprite = nullptr;
}

void Land::pause() {}

void Land::resume() {}

void Land::settle() {
    if (crop) crop ->settle();
    change_archive_in_memory(info_.position);
}

bool Land::hasCollision() {
    return false;
}
