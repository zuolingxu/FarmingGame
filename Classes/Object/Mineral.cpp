#include "Mineral.h"
#include "MainCharacter.h"
#include "HelperClasses.h"
#include "TimeManager.h"

Mineral::Mineral(const Vec<int>& pos, MapLayer* parent, bool im, const std::string& type)
    : MapObject(pos), parent(parent), isMined(im), mineralType(type) {
    info_.size = Vec<int>(1, 1);  // 默认矿石占据一个1x1的格子

    CCLOG("11");
}

Mineral::~Mineral() {
    // 清理资源，确保矿石对象被正确删除
    clear();
}

// 从存档创建矿石对象
MapObject* Mineral::create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos) {
    
    // Check if the mineral has been mined already
    if (val.HasMember("IsMined")&&val.HasMember("MineralType")) {
        bool im = val["IsMined"].GetBool();
        std::string mineralType = val["MineralType"].GetString();
      
        // Create a new Mineral object using the position and parent map layer
        Mineral* mineral = new Mineral(pos, parent, im, mineralType);
        return mineral;
    }
    else {
        return nullptr;
    }
}

void Mineral::init() {

    DocumentManager* manager = DocumentManager::getInstance();

    rapidjson::Document* doc = manager->getDocument(manager->getPath(mineralType));

    std::string plistFilePath = (*doc)["plistpath"].GetString();
    std::string spriteFrame = (*doc)["frame_format"].GetString();

    MapLayer::loadPlist(plistFilePath);

    // 如果父级对象存在，加载矿石的精灵
    if (parent != nullptr) {
        // 根据是否已经挖掘来决定是否显示矿石
        if (isMined) {
            // 如果矿石已被挖掘，info_.sprite=nullptr（不显示矿石）
            info_.sprite = nullptr;
        }
        else {
            // 如果矿石没有被挖掘，传入对应的矿石类型纹理
            parent->addSpriteWithFrame(info_, spriteFrame);
        }
    }
}

void Mineral::interact() {
    // 如果矿石已经被挖掘，则不能再次互动
    if (isMined) {
        CCLOG("The mineral has already been mined.");
        return;
    }
    if (MainCharacter::getInstance()->getCurrentItemType() != ItemType::PICKAXE)
        return;
    // 减少体力，增加物品
    if ((MainCharacter::getInstance()->modifyEnergy(MainCharacter::getInstance()->Getting_mineral_consumes_energy)) != 1)
        return;
    // 挖掘矿石
    // 将矿石标记为已挖掘
    isMined = true;

    // 执行挖掘操作后，更新存档
    update_ismined_Archive(info_.position, 1);

    // 让图片是空
    if (parent != nullptr) {
        isMined = 1;
        update_ismined_Archive(info_.position, 1);

        // 矿石被挖掘
        info_.sprite->removeFromParentAndCleanup(true);
        info_.sprite = nullptr;
    }


    ItemType it = Item::stringToItemType(mineralType);
    MainCharacter::getInstance()->modifyItemQuantity(it, 1);
}

void Mineral::clear() {
    // 清除矿石的精灵和相关资源
    info_.sprite = nullptr;
}

void Mineral::pause() {

}

void Mineral::resume() {

}

void Mineral::settle() 
{
    //every REFRESH_MINERAL , refresh mineral
    int day = TimeManager::getInstance()->getCurrentDay();
    if (day%TimeManager::getInstance()->REFRESH_MINERAL==0)
    {
        DocumentManager* manager = DocumentManager::getInstance();

        rapidjson::Document* doc = manager->getDocument(manager->getPath(mineralType));

        std::string plistFilePath = (*doc)["plistpath"].GetString();
        std::string spriteFrame = (*doc)["frame_format"].GetString();

        MapLayer::loadPlist(plistFilePath);

        // 如果父级对象存在，加载矿石的精灵
        if (parent != nullptr) {

            isMined = 0;
            update_ismined_Archive(info_.position, 0);

            if (info_.sprite) {
                // now sprite has been displayed
            }
            else {
                parent->addSpriteWithFrame(info_, spriteFrame);
            }

        }
    }
}

bool Mineral::hasCollision() {
    return true;
}

void Mineral::update_ismined_Archive(const Vec<int>& position, bool isMinedValue) {
    // 获取存档管理器和当前存档文档
    auto* docManager = DocumentManager::getInstance();
    auto* archiveDoc = docManager->getArchiveDocument();

    if (!archiveDoc) {
        CCLOG("Archive document is null!");
        return;
    }

    // 获取Map节点
    rapidjson::Value& map = (*archiveDoc)["Map"];
    if (map.HasMember("Mine")) {
        rapidjson::Value& mine = map["Mine"];

        // 获取矿石的位置作为key
        std::string positionKey = std::to_string(position.X()) + " " + std::to_string(position.Y());

        // 检查该位置是否存在矿石数据
        if (mine.HasMember(positionKey.c_str())) {
            rapidjson::Value& mineData = mine[positionKey.c_str()];
            rapidjson::Value& info = mineData["Info"];

            // 更新isMined的值
            if (info.HasMember("IsMined")) {
                info["IsMined"].SetBool(isMinedValue);  // 修改IsMined键的值为传入的isMinedValue
            }
        }
        else {
            CCLOG("Error: No mine data found for the given position!");
        }
    }
}
