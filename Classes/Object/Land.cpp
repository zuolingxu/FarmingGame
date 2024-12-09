#include "Land.h"

USING_NS_CC;

Land::Land(MapLayer* parent) : parent_(parent),Water(false),Fertilizer(false)
{
	
}

Land::~Land()
{
	// 清理作物对象
	if (crop_) {
		delete crop_;
		crop_ = nullptr;
	}
}


::Object* Land::create(rapidjson::Value& val, MapLayer* parent)
{
	Land* land = new Land(parent);
	
	// 检查作物是否存在
	if (val.HasMember("HasCrop") && val["HasCrop"].GetBool()) {
		// 创建作物对象
		if (val.HasMember("CropInfo") && val["CropInfo"].IsObject()) {
			// 传入 CropInfo 的值
			rapidjson::Value& cropInfo = val["CropInfo"];
			land->crop_ = Crop::create(cropInfo, parent);
		}
	}
	else {
		land->crop_ = nullptr; // 没有作物
	}

	if (val.HasMember("Water")) {
		land->Water = val["Water"].GetBool();
	}
	if (val.HasMember("Fertilizer")) {
		land->Fertilizer = val["Fertilizer"].GetBool();
	}

	return land;
}

::Object* Land::createByPlayer(const cocos2d::Vec2& position, MapLayer* parent) {
	Land* land = new Land(parent);
	land->crop_ = nullptr; // 没有作物

	// 将耕地位置和信息加入存档（内存）
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
	Character* character = Character::getInstance(); // 获取主角实例

	if (!character) {
		CCLOG("No character instance available for interaction.");
		return;
	}

	// 获取当前存档文档
	DocumentManager* docManager = DocumentManager::getInstance();
	rapidjson::Document* archiveDoc = docManager->getArchiveDocument();

	// 检查人物持有的物品
	if (character->hasItem(Character::ItemType::FERTILIZER)) {
		// 施肥逻辑
		CCLOG("Applying fertilizer to the land.");
		Fertilizer = true; // 更新耕地的施肥状态

		// 更新 JSON 存档
		if (archiveDoc->HasMember("Map")) {
			auto& map = (*archiveDoc)["Map"];
			if (map.HasMember("Farm") && map["Farm"].HasMember("12 13")) {
				auto& landInfo = map["Farm"]["12 13"]["Info"];
				landInfo["Fertilizer"].SetBool(true); // 更新肥料状态
			}
		}
	}
	else if (character->hasItem(Character::ItemType::SEED)) {
		// 播种逻辑
		CCLOG("Planting seeds in the land.");
		if (!crop_) {
			crop_ = Crop::createByPlayer(cocos2d::Vec2(12, 13), parent_); // 创建新的作物对象
		}

		// 更新 JSON 存档
		if (archiveDoc->HasMember("Map")) {
			auto& map = (*archiveDoc)["Map"];
			if (map.HasMember("Farm") && map["Farm"].HasMember("12 13")) {
				auto& landInfo = map["Farm"]["12 13"]["Info"];
				landInfo["HasCrop"].SetBool(true); // 更新作物存在状态
			}
		}
	}
	else if (character->hasItem(Character::ItemType::WATERING_CAN)) {
		// 浇水逻辑
		CCLOG("Watering the crops.");
		if (crop_) {
			crop_->Water = true; // 更新作物的浇水状态

			// 更新 JSON 存档
			if (archiveDoc->HasMember("Map")) {
				auto& map = (*archiveDoc)["Map"];
				if (map.HasMember("Farm") && map["Farm"].HasMember("12 13")) {
					auto& landInfo = map["Farm"]["12 13"]["Info"];
					landInfo["CropInfo"]["Water"].SetBool(true); // 更新浇水状态
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
	//若耕地上有作物则调用作物的每日结算settle
	if (this->crop_)
		(this->crop_)->settle();
}

bool Land::hasCollision()
{
	return false;
}
