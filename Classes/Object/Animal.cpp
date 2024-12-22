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
		// ���ƶ� 
		animalSprite->move(PlayerSprite::MOVEMENT::W_DOWN, info_.position.Y() - 2);

		// ���ƶ�
		animalSprite->scheduleOnce([=](float deltaTime) {
			animalSprite->move(PlayerSprite::MOVEMENT::W_UP, 5- info_.position.Y());
			}, 3.0f, "up_move_key");

		}, 6.0f, "down_move_key"); // ÿ6 ��ѭ��һ��
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

		// ȷ���ж�Ӧ�� positionKey
		if (chickenHouse.HasMember(positionKey.c_str())) {
			rapidjson::Value& AnimalInfo = chickenHouse[positionKey.c_str()];

			if (AnimalInfo.HasMember("Info") && AnimalInfo["Info"].IsObject()) {
				rapidjson::Value& info = AnimalInfo["Info"];

				// ��������
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
		// ȷ���ж�Ӧ�� positionKey
		if (chickenHouse.HasMember(positionKey.c_str())) {
			// ɾ����λ�õĳ�Ա
			chickenHouse.RemoveMember(positionKey.c_str());
			CCLOG("Deleted position: %s", positionKey.c_str());
			return true;
		}
	}
	return false;
}

bool Animal::new_archive_in_memory(int x, int y) {
	//��ȡ�ļ��������ʹ浵
	auto* docManager = DocumentManager::getInstance();
	auto* archiveDoc = docManager->getArchiveDocument();
	
	//������Ϊ��ֵ����
	const Vec<int>& position = Vec<int>(x, y);
	std::string positionKey = std::to_string(position.X()) + " " + std::to_string(position.Y());
	if (archiveDoc->HasMember("Map") && (*archiveDoc)["Map"].HasMember("chicken_house")) {
		rapidjson::Value& chickenHouse = (*archiveDoc)["Map"]["chicken_house"];
		
		//����Ѿ��д浵��ֹͣ������ֹ������ͻ����ӡ��־�����쳣
		if (chickenHouse.HasMember(positionKey.c_str())) {
			CCLOG("has object already��%s", positionKey.c_str());
			return false;
		}
		else {
			// �����λ��û�д浵���ݣ������µĴ浵����
			rapidjson::Document::AllocatorType& allocator = archiveDoc->GetAllocator();

			// �����µĴ浵����
			rapidjson::Value newAnimalData(rapidjson::kObjectType);
			newAnimalData.AddMember("Type", "Animal", allocator);

			rapidjson::Value info(rapidjson::kObjectType);
			info.AddMember("AnimalType", "Chicken", allocator);
			info.AddMember("Value", 40, allocator);
			info.AddMember("Satiety", 10, allocator);
			info.AddMember("Length", 1, allocator);

			newAnimalData.AddMember("Info", info, allocator);

			// ���µĴ浵���ݲ��뵽 "chicken_house" ������
			chickenHouse.AddMember(rapidjson::Value(positionKey.c_str(), allocator), newAnimalData, allocator);

			// ��ӡ��־��ȷ�ϲ���
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
	// �������������Դ
	info_.sprite = nullptr;
}

void Animal::pause()
{
	isPause = true;                                 //�ı�״̬������ֹͣ�˶�
}

void Animal::resume() 
{
	isPause = false;                                //�ı�״̬�����������˶�
}

void Animal::settle()
{
	if (satiety < -20)
		delete_archive_in_memory();                 //���ڲ�ι������������
	else
		change_archive_in_memory();                 //���¶���״̬
	if(isSold)
		delete_archive_in_memory();                 //ɾ���������Ķ���
	breed();                                        //���з�ֳ����
}

bool Animal::hasCollision() {
	return false;
}

