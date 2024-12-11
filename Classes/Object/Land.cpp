#include "Land.h"
#include "MainCharacter.h"

USING_NS_CC;

Land::Land(const Vec<int>& pos, MapLayer* parent,  bool Fertilizer)
    : MapObject(pos), parent(parent), crop(nullptr),Fertilizer(false) {
    info_.size = Vec<int>(1, 1);    //�̶�ֲ��ռ�����1��1
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
        // �����������Ϣ��CropInfo�������� Crop ����
        if (val.HasMember("CropInfo") && val["CropInfo"].IsObject()) {
            rapidjson::Value& cropInfo = val["CropInfo"];
            land->crop = Crop::create(cropInfo, parent, pos);  // ʹ�� Crop::create ���� Crop ����
        }
    }

    // �����Ƿ��з��ϣ�Fertilizer��
    if (val.HasMember("Fertilizer")) {
        land->Fertilizer = val["Fertilizer"].GetBool();  // ��ȡ����״̬
    }

    return land;  // ���ش����� Land ����
}


MapObject* Land::createByPlayer(const Vec<int>& position, MapLayer* parent) 
{
    Land* land = new Land( position, parent, false);

    land->create_archive_in_memory(position);

    return land;
}

void Land::create_archive_in_memory(const Vec<int>& position) {
    // ��ȡ DocumentManager �͵�ǰ�����ĵ�
    auto* docManager = DocumentManager::getInstance();
    auto* archiveDoc = docManager->getArchiveDocument();

    // ȷ���ĵ�����
    if (!archiveDoc) {
        CCLOG("Archive document is null!");
        return;
    }

    // ��ȡ Map �ڵ㣨�򴴽� Farm �ڵ㣩
    rapidjson::Value& map = (*archiveDoc)["Map"];
    if (!map.HasMember("Farm")) {
        map.AddMember("Farm", rapidjson::Value(rapidjson::kObjectType), archiveDoc->GetAllocator());
    }

    rapidjson::Value& farm = map["Farm"];

    // �� position ת��Ϊ�ַ������Ա���Ϊ Map �ļ�
    std::string positionKey = std::to_string(position.X()) + " " + std::to_string(position.Y());

    // ������ǰ Land ��������ݽṹ
    rapidjson::Value landData(rapidjson::kObjectType);
    rapidjson::Value info(rapidjson::kObjectType);

    // ��¼�Ƿ�������
    info.AddMember("HasCrop", crop != nullptr, archiveDoc->GetAllocator());

    // ��¼�Ƿ�ʩ��
    info.AddMember("Fertilizer", Fertilizer, archiveDoc->GetAllocator());

    // ��¼ Crop ��Ϣ����������
    if (crop != nullptr) {
        rapidjson::Value cropInfo(rapidjson::kObjectType);

        cropInfo.AddMember("CropName", rapidjson::Value(crop->getCropName().c_str(), archiveDoc->GetAllocator()), archiveDoc->GetAllocator());
        cropInfo.AddMember("Water", crop->getWater(), archiveDoc->GetAllocator());
        cropInfo.AddMember("LiveDay", crop->getLiveDay(), archiveDoc->GetAllocator());
        cropInfo.AddMember("MaturationDay", crop->getMaturationDay(), archiveDoc->GetAllocator());

        info.AddMember("CropInfo", cropInfo, archiveDoc->GetAllocator());
    }

    // �� info ���ݴ洢�� landData ��
    landData.AddMember("Info", info, archiveDoc->GetAllocator());

    // ����λ�õ� Land ���ݴ洢�� "Farm" �µĶ�Ӧλ��
    farm.AddMember(rapidjson::Value(positionKey.c_str(), archiveDoc->GetAllocator()), landData, archiveDoc->GetAllocator());
}

void Land::change_archive_in_memory(const Vec<int>& position) {
    // ��ȡ DocumentManager ʵ������ȡ��ǰ�浵�ĵ�
    auto* docManager = DocumentManager::getInstance();
    auto* archiveDoc = docManager->getArchiveDocument();

    // �� position ת��Ϊ�ַ�������Ϊ Map �еļ�
    std::string positionKey = std::to_string(position.X()) + " " + std::to_string(position.Y());

    // ���Ҹ�λ�õ� Land ����ͨ�� positionKey��
    if (archiveDoc->HasMember("Map") && (*archiveDoc)["Map"].HasMember("Farm")) {
        rapidjson::Value& farm = (*archiveDoc)["Map"]["Farm"];
        if (farm.HasMember(positionKey.c_str())) {
            rapidjson::Value& landInfo = farm[positionKey.c_str()];

            // ���� Fertilizer ״̬
            if (landInfo.HasMember("Info") && landInfo["Info"].IsObject()) {
                rapidjson::Value& info = landInfo["Info"];

                // ���� Fertilizer ״̬
                if (info.HasMember("Fertilizer")) {
                    info["Fertilizer"].SetBool(Fertilizer);  // ���ݵ�ǰ Fertilizer ״̬���и���
                }

                // ���� HasCrop ״̬
                bool hasCrop = (crop != nullptr);  // ��� crop ��Ϊ�գ���ʾ��ǰ Land ������
                if (info.HasMember("HasCrop")) {
                    info["HasCrop"].SetBool(hasCrop);
                }
                else {
                    info.AddMember("HasCrop", hasCrop, archiveDoc->GetAllocator());
                }

                // ������������������Ϣ
                if (hasCrop && crop != nullptr) {
                    rapidjson::Value cropInfo(rapidjson::kObjectType);
                    cropInfo.AddMember("CropName", rapidjson::Value(crop->getCropName().c_str(), archiveDoc->GetAllocator()), archiveDoc->GetAllocator());
                    cropInfo.AddMember("Water", crop->getWater(), archiveDoc->GetAllocator());
                    cropInfo.AddMember("LiveDay", crop->getLiveDay(), archiveDoc->GetAllocator());
                    cropInfo.AddMember("MaturationDay", crop->getMaturationDay(), archiveDoc->GetAllocator());

                    info.AddMember("CropInfo", cropInfo, archiveDoc->GetAllocator());
                }
                else {
                    // û������ʱ���Ƴ� CropInfo
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
