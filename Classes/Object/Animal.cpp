#include"Animal.h"

Animal::Animal(MapLayer* parent, const Vec<int>& pos, std::string Type, int AnimalValue, int satiety, int length,float breedPro)
	:
 	type(Type),
	parent_(parent),
	MapObject(pos),
	value(AnimalValue),
	satiety(satiety),
	isSold(false),
	breedProbility(breedPro),
	isPause(false)
{
	info_.size = Vec<int>(length, length);
}

MapObject* Animal::create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos) {
	std::string AnimalType;
	int Value=50,Satiety=0,length=1;
	if (val.HasMember("AnimalType") && val["AnimalType"].IsString()) {
		AnimalType = val["AnimalType"].GetString();
	}

	if (val.HasMember("Value") && val["Value"].IsInt()) {
		Value = val["Value"].GetInt();
	}
	if (val.HasMember("Length") && val["Length"].IsInt()) {
		length = val["Length"].GetInt();
	}

	if (val.HasMember("Satiety") && val["Satiety"].IsInt()) {
		Satiety = val["Satiety"].GetInt();
	}
	Animal*animal= new Animal(parent, pos, AnimalType, Value, Satiety, length,0.2f);
	return animal;
}

void Animal::init() {
	DocumentManager* manager = DocumentManager::getInstance();
	rapidjson::Document* doc = manager->getDocument(manager->getPath(type));
	std::string plistFilePath = type + "Pls";
	MapLayer::loadPlist(plistFilePath);
	std::string frame_n_format = (*doc)["frame_format"].GetString();
	std::string spriteFrame = getFrameName(frame_n_format, 1);
	parent_->addPlayerSpriteWithDocument(info_, doc);
	defaultAction();
}

void Animal::defaultAction() {
	if (parent_ == nullptr) {
		CCLOG("dafaulAction->parent_ nullptr");
		return;
	}
	else
		patrolPath();
	
}

void Animal::patrolPath() {
	PlayerSprite* animalSprite = dynamic_cast<PlayerSprite*>(info_.sprite);

	animalSprite->schedule([=](float deltaTime) {
		// 下移动 
		animalSprite->move(PlayerSprite::MOVEMENT::W_DOWN, info_.position.Y() - 2);

		// 上移动
		animalSprite->scheduleOnce([=](float deltaTime) {
			animalSprite->move(PlayerSprite::MOVEMENT::W_UP, 5- info_.position.Y());
			}, 3.0f, "up_move_key");

		}, 6.0f, "down_move_key"); // 每6 秒循环一次
}


void Animal::interact() 
{
	MainCharacter* maincharacter = MainCharacter::getInstance();
	const Item* currentItem = maincharacter->getCurrentItem();
	if (!currentItem)	{
		sold();
	}
	else if (currentItem->type==ItemType::PUMPKIN) {
		satiety += 30;
	}
	else if (currentItem->type == ItemType::POTATO) {
		satiety += 20;
		breedProbility += 0.1f;
	}
	else if (currentItem->type == ItemType::CAULIFLOWER_SEED) {
		satiety += 50;
		breedProbility += 0.2f;
	}
	else if (currentItem->type == ItemType::CAULIFLOWER) {
		satiety -= 20;
		breedProbility -= 0.1f;                          
	}

	else
		CCLOG("NPCinteract:name->favorite:error");
}

bool Animal::change_archive_in_memory() {
	auto* docManager = DocumentManager::getInstance();
	auto* archiveDoc = docManager->getArchiveDocument();
	const Vec<int>& position = info_.position;
	// Convert position to a string to use as the map key
	std::string positionKey = std::to_string(position.X()) + " " + std::to_string(position.Y());
	if (archiveDoc->HasMember("Map") && (*archiveDoc)["Map"].HasMember("chicken_house")) {
		rapidjson::Value& chickenHouse = (*archiveDoc)["Map"]["chicken_house"];

		// 确保有对应的 positionKey
		if (chickenHouse.HasMember(positionKey.c_str())) {
			rapidjson::Value& AnimalInfo = chickenHouse[positionKey.c_str()];

			if (AnimalInfo.HasMember("Info") && AnimalInfo["Info"].IsObject()) {
				rapidjson::Value& info = AnimalInfo["Info"];

				// 更新数据
				if (info.HasMember("Satiety") && info["Satiety"].IsInt()) {
					info["Satiety"] = satiety;
				}
				if (info.HasMember("Value") && info["Value"].IsInt()) {
					info["Value"] = value;
				}
				return true;
			}
		}
	} 
	return false;
}

bool Animal::delete_archive_in_memory() {
	// Get the DocumentManager instance and the current archive document
	auto* docManager = DocumentManager::getInstance();
	auto* archiveDoc = docManager->getArchiveDocument();
	const Vec<int>& position = info_.position;
	// Convert position to a string to use as the map key
	std::string positionKey = std::to_string(position.X()) + " " + std::to_string(position.Y());
	if (archiveDoc->HasMember("Map") && (*archiveDoc)["Map"].HasMember("chicken_house")) {
		rapidjson::Value& chickenHouse = (*archiveDoc)["Map"]["chicken_house"];
		// 确保有对应的 positionKey
		if (chickenHouse.HasMember(positionKey.c_str())) {
			// 删除该位置的成员
			chickenHouse.RemoveMember(positionKey.c_str());
			CCLOG("Deleted position: %s", positionKey.c_str());
			return true;
		}
	}
	return false;
}

bool Animal::new_archive_in_memory(int x, int y) {
	//获取文件管理器和存档
	auto* docManager = DocumentManager::getInstance();
	auto* archiveDoc = docManager->getArchiveDocument();
	
	//坐标作为键值查找
	const Vec<int>& position = Vec<int>(x, y);
	std::string positionKey = std::to_string(position.X()) + " " + std::to_string(position.Y());
	if (archiveDoc->HasMember("Map") && (*archiveDoc)["Map"].HasMember("chicken_house")) {
		rapidjson::Value& chickenHouse = (*archiveDoc)["Map"]["chicken_house"];
		
		//如果已经有存档，停止操做防止发生冲突，打印日志报告异常
		if (chickenHouse.HasMember(positionKey.c_str())) {
			CCLOG("has object already，%s", positionKey.c_str());
			return false;
		}
		else {
			// 如果该位置没有存档数据，插入新的存档数据
			rapidjson::Document::AllocatorType& allocator = archiveDoc->GetAllocator();

			// 创建新的存档数据
			rapidjson::Value newAnimalData(rapidjson::kObjectType);
			newAnimalData.AddMember("Type", "Animal", allocator);

			rapidjson::Value info(rapidjson::kObjectType);
			info.AddMember("AnimalType", "Chicken", allocator);
			info.AddMember("Value", 40, allocator);
			info.AddMember("Satiety", 10, allocator);
			info.AddMember("Length", 1, allocator);

			newAnimalData.AddMember("Info", info, allocator);

			// 将新的存档数据插入到 "chicken_house" 对象中
			chickenHouse.AddMember(rapidjson::Value(positionKey.c_str(), allocator), newAnimalData, allocator);

			// 打印日志，确认插入
			CCLOG("Inserted new archive for position: %s", positionKey.c_str());

			return true;
		}
	}
	else 
		CCLOG("No such map for %s",type);
	return false;
}

void Animal::breed() {
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	float isBreed = 0.0f;//distribution(engine);
	if (isBreed < breedProbility)
	{
		std::uniform_int_distribution<int> distribution(2, 6);
		int newX = distribution(engine), newY = distribution(engine);
		new_archive_in_memory(newX, newY);
	}
}

void Animal::sold() 
{
	if (!isSold) {
		isSold = 1;
		MainCharacter::getInstance()->modifyMoney(value);  
		parent_->removeObject(info_);
	}
	else
		CCLOG("Animal Sale Error,repeat sale: %s", type);
}

void Animal::clear() 
{
	// 清除精灵和相关资源
	info_.sprite = nullptr;
}

void Animal::pause()
{
	isPause = true;                                 //改变状态参数，停止运动
}

void Animal::resume() 
{
	isPause = false;                                //改变状态参数，继续运动
}

void Animal::settle()
{
	if (satiety < -20)
		delete_archive_in_memory();                 //长期不喂养，动物死亡
	else
		change_archive_in_memory();                 //更新动物状态
	if(isSold)
		delete_archive_in_memory();                 //删除被卖掉的动物
	breed();                                        //进行繁殖操作
}

bool Animal::hasCollision() {
	return false;
}

