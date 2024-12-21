#include "Mineral.h"
#include "MainCharacter.h"
#include "HelperClasses.h"
#include "TimeManager.h"

Mineral::Mineral(const Vec<int>& pos, MapLayer* parent, bool im, const std::string& type)
    : MapObject(pos), parent(parent), isMined(im), mineralType(type) {
    info_.size = Vec<int>(1, 1);  // Ĭ�Ͽ�ʯռ��һ��1x1�ĸ���

    CCLOG("11");
}

Mineral::~Mineral() {
    // ������Դ��ȷ����ʯ������ȷɾ��
    clear();
}

// �Ӵ浵������ʯ����
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

    // �������������ڣ����ؿ�ʯ�ľ���
    if (parent != nullptr) {
        // �����Ƿ��Ѿ��ھ��������Ƿ���ʾ��ʯ
        if (isMined) {
            // �����ʯ�ѱ��ھ�info_.sprite=nullptr������ʾ��ʯ��
            info_.sprite = nullptr;
        }
        else {
            // �����ʯû�б��ھ򣬴����Ӧ�Ŀ�ʯ��������
            parent->addSpriteWithFrame(info_, spriteFrame);
        }
    }
}

void Mineral::interact() {
    // �����ʯ�Ѿ����ھ������ٴλ���
    if (isMined) {
        CCLOG("The mineral has already been mined.");
        return;
    }
    if (MainCharacter::getInstance()->getCurrentItemType() != ItemType::PICKAXE)
        return;
    // ����������������Ʒ
    if ((MainCharacter::getInstance()->modifyEnergy(MainCharacter::getInstance()->Getting_mineral_consumes_energy)) != 1)
        return;
    // �ھ��ʯ
    // ����ʯ���Ϊ���ھ�
    isMined = true;

    // ִ���ھ�����󣬸��´浵
    update_ismined_Archive(info_.position, 1);

    // ��ͼƬ�ǿ�
    if (parent != nullptr) {
        isMined = 1;
        update_ismined_Archive(info_.position, 1);

        // ��ʯ���ھ�
        info_.sprite->removeFromParentAndCleanup(true);
        info_.sprite = nullptr;
    }


    ItemType it = Item::stringToItemType(mineralType);
    MainCharacter::getInstance()->modifyItemQuantity(it, 1);
}

void Mineral::clear() {
    // �����ʯ�ľ���������Դ
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

        // �������������ڣ����ؿ�ʯ�ľ���
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
    // ��ȡ�浵�������͵�ǰ�浵�ĵ�
    auto* docManager = DocumentManager::getInstance();
    auto* archiveDoc = docManager->getArchiveDocument();

    if (!archiveDoc) {
        CCLOG("Archive document is null!");
        return;
    }

    // ��ȡMap�ڵ�
    rapidjson::Value& map = (*archiveDoc)["Map"];
    if (map.HasMember("Mine")) {
        rapidjson::Value& mine = map["Mine"];

        // ��ȡ��ʯ��λ����Ϊkey
        std::string positionKey = std::to_string(position.X()) + " " + std::to_string(position.Y());

        // ����λ���Ƿ���ڿ�ʯ����
        if (mine.HasMember(positionKey.c_str())) {
            rapidjson::Value& mineData = mine[positionKey.c_str()];
            rapidjson::Value& info = mineData["Info"];

            // ����isMined��ֵ
            if (info.HasMember("IsMined")) {
                info["IsMined"].SetBool(isMinedValue);  // �޸�IsMined����ֵΪ�����isMinedValue
            }
        }
        else {
            CCLOG("Error: No mine data found for the given position!");
        }
    }
}
