#include "Land.h"

USING_NS_CC;

Land::Land(MapLayer* parent, const Vec<int>& pos) : MapObject(pos), parent_(parent),Water(false),Fertilizer(false)
{
	
}

Land::~Land()
{
	// �����������
	if (crop_) {
		delete crop_;
		crop_ = nullptr;
	}
}


::MapObject* Land::create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos)
{
	Land* land = new Land(parent);
	
	// ��������Ƿ����
	if (val.HasMember("HasCrop") && val["HasCrop"].GetBool()) {
		// �����������
		if (val.HasMember("CropInfo") && val["CropInfo"].IsObject()) {
			// ���� CropInfo ��ֵ
			rapidjson::Value& cropInfo = val["CropInfo"];
			land->crop_ = Crop::create(cropInfo, parent);
		}
	}
	else {
		land->crop_ = nullptr; // û������
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
	Land* land = new Land(parent);
	land->crop_ = nullptr; // û������

	// ������λ�ú���Ϣ����浵���ڴ棩
	land->saveToArchive(position);

	return land; 
}

void Land::saveToArchive(const cocos2d::Vec2& position) {
	DocumentManager* docManager = DocumentManager::getInstance();
	rapidjson::Document* archiveDoc = docManager->getArchiveDocument();

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
	else {
		landInfo.AddMember("CropInfo", rapidjson::Value(rapidjson::kObjectType), archiveDoc->GetAllocator());
	}

	if (archiveDoc->HasMember("Map")) {
		rapidjson::Value& map = (*archiveDoc)["Map"];
		if (map.HasMember("Farm")) {
			map["Farm"].AddMember(rapidjson::Value(positionKey.c_str(), archiveDoc->GetAllocator()), landInfo, archiveDoc->GetAllocator());
		}
		else {
			rapidjson::Value farm(rapidjson::kObjectType);
			farm.AddMember(rapidjson::Value(positionKey.c_str(), archiveDoc->GetAllocator()), landInfo, archiveDoc->GetAllocator());
			map.AddMember("Farm", farm, archiveDoc->GetAllocator());
		}
	}
	else {
		rapidjson::Value map(rapidjson::kObjectType);
		rapidjson::Value farm(rapidjson::kObjectType);
		farm.AddMember(rapidjson::Value(positionKey.c_str(), archiveDoc->GetAllocator()), landInfo, archiveDoc->GetAllocator());
		map.AddMember("Farm", farm, archiveDoc->GetAllocator());
		(*archiveDoc).AddMember("Map", map, archiveDoc->GetAllocator());
	}
}

void Land::init()
{
	info_.sprite = nullptr;
}


void Land::interact() {
	Character* character = Character::getInstance(); // ��ȡ����ʵ��

	if (!character) {
		CCLOG("No character instance available for interaction.");
		return;
	}

	// ��ȡ��ǰ�浵�ĵ�
	DocumentManager* docManager = DocumentManager::getInstance();
	rapidjson::Document* archiveDoc = docManager->getArchiveDocument();

	// ���������е���Ʒ
	if (character->hasItem(Character::ItemType::FERTILIZER)) {
		// ʩ���߼�
		CCLOG("Applying fertilizer to the land.");
		Fertilizer = true; // ���¸��ص�ʩ��״̬

		// ���� JSON �浵
		if (archiveDoc->HasMember("Map")) {
			auto& map = (*archiveDoc)["Map"];
			if (map.HasMember("Farm") && map["Farm"].HasMember("12 13")) {
				auto& landInfo = map["Farm"]["12 13"]["Info"];
				landInfo["Fertilizer"].SetBool(true); // ���·���״̬
			}
		}
	}
	else if (character->hasItem(Character::ItemType::SEED)) {
		// �����߼�
		CCLOG("Planting seeds in the land.");
		if (!crop_) {
			crop_ = Crop::createByPlayer(cocos2d::Vec2(12, 13), parent_); // �����µ��������
		}

		// ���� JSON �浵
		if (archiveDoc->HasMember("Map")) {
			auto& map = (*archiveDoc)["Map"];
			if (map.HasMember("Farm") && map["Farm"].HasMember("12 13")) {
				auto& landInfo = map["Farm"]["12 13"]["Info"];
				landInfo["HasCrop"].SetBool(true); // �����������״̬
			}
		}
	}
	else if (character->hasItem(Character::ItemType::WATERING_CAN)) {
		// ��ˮ�߼�
		CCLOG("Watering the crops.");
		if (crop_) {
			crop_->Water = true; // ��������Ľ�ˮ״̬

			// ���� JSON �浵
			if (archiveDoc->HasMember("Map")) {
				auto& map = (*archiveDoc)["Map"];
				if (map.HasMember("Farm") && map["Farm"].HasMember("12 13")) {
					auto& landInfo = map["Farm"]["12 13"]["Info"];
					landInfo["CropInfo"]["Water"].SetBool(true); // ���½�ˮ״̬
				}
			}
		}
	}
	else {
		CCLOG("No valid items to interact with the land.");
	}

}
void Land::clear()
{
	info_.sprite = nullptr;
}

void Land::pause()
{
	
}

void Land::resume()
{
	
}

void Land::settle()
{
	//������������������������ÿ�ս���settle
	if (this->crop_)
		(this->crop_)->settle();
}

bool Land::hasCollision()
{
	return false;
}
