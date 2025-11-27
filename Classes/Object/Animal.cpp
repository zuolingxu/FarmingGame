#include "Animal.h"

std::set<int> Animal::tag = {};

Animal::Animal(MapLayer* parent, const Vec<int>& pos, std::string Type, int AnimalValue, int satiety, int length, float breedPro)
	:
	type(Type),
	parent_(parent),
	MapObject(pos),
	value(AnimalValue),
	satiety(satiety),
	isSold(false),
	breedProbility(breedPro),
	isPause(false),
	defaultPos(pos)
{
	info_.size = Vec<int>(length, length);
}

MapObject* Animal::create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos) {
	std::string AnimalType;
	int Value = 50, Satiety = 0, length = 1;
	tag.insert(pos.X());
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
	Animal* animal = new Animal(parent, pos, AnimalType, Value, Satiety, length, 0.2f);
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
		CCLOG("defaultAction->parent_ nullptr");
		return;
	}
	else
		patrolPath();       // Start moving
}

// TODO: 策略模式重构

void Animal::patrolPath() {
	PlayerSprite* animalSprite = dynamic_cast<PlayerSprite*>(info_.sprite);

	animalSprite->schedule([=](float deltaTime) {
		// Move down
		animalSprite->move(PlayerSprite::MOVEMENT::W_DOWN, info_.position.Y() - 2);

		// Move up
		animalSprite->scheduleOnce([=](float deltaTime) {
			animalSprite->move(PlayerSprite::MOVEMENT::W_UP, 5 - info_.position.Y());
			}, 3.0f, "up_move_key");

		}, 6.0f, "down_move_key"); // Repeat every 6 seconds
}

void Animal::interact()
{
	MainCharacter* maincharacter = MainCharacter::getInstance();
	const Item* currentItem = maincharacter->getCurrentItem();

	if (!currentItem)                                      // If no item is held, treat as selling the animal
		sold();
	else if (currentItem->type == ItemType::PUMPKIN) {       // Feed to increase satiety
		satiety += 30;
	}
	else if (currentItem->type == ItemType::POTATO) {
		satiety += 20;
		breedProbility += 0.1f;                              // Special food increases the breeding probability
	}
	else if (currentItem->type == ItemType::CAULIFLOWER_SEED) {
		satiety += 50;
		breedProbility += 0.2f;
	}
	else if (currentItem->type == ItemType::CAULIFLOWER) {
		satiety -= 20;
		breedProbility -= 0.1f;                            // Unfit food may have side effects
	}
	else {
		sold();
	}
}

// Update animal parameters in the archive
bool Animal::change_archive_in_memory() {
	auto* docManager = DocumentManager::getInstance();
	auto* archiveDoc = docManager->getArchiveDocument();
	// Convert position to a string to use as the map key
	std::string positionKey = std::to_string(defaultPos.X()) + " " + std::to_string(defaultPos.Y());
	if (archiveDoc->HasMember("Map") && (*archiveDoc)["Map"].HasMember("chicken_house")) {
		rapidjson::Value& chickenHouse = (*archiveDoc)["Map"]["chicken_house"];

		// Ensure the corresponding positionKey exists
		if (chickenHouse.HasMember(positionKey.c_str())) {
			rapidjson::Value& AnimalInfo = chickenHouse[positionKey.c_str()];

			if (AnimalInfo.HasMember("Info") && AnimalInfo["Info"].IsObject()) {
				rapidjson::Value& info = AnimalInfo["Info"];

				// Update data
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

// Delete the animal from the archive if it has been sold or starved to death
bool Animal::delete_archive_in_memory() {
	// Get the DocumentManager instance and the current archive document
	auto* docManager = DocumentManager::getInstance();
	auto* archiveDoc = docManager->getArchiveDocument();

	// Convert position to a string to use as the map key
	std::string positionKey = std::to_string(defaultPos.X()) + " " + std::to_string(defaultPos.Y());
	if (archiveDoc->HasMember("Map") && (*archiveDoc)["Map"].HasMember("chicken_house")) {
		rapidjson::Value& chickenHouse = (*archiveDoc)["Map"]["chicken_house"];
		// Ensure the corresponding positionKey exists
		if (chickenHouse.HasMember(positionKey.c_str())) {
			// Remove the member at this position
			chickenHouse.RemoveMember(positionKey.c_str());
			CCLOG("Deleted position: %s", positionKey.c_str());
			// Check if deletion was successful
			if (!chickenHouse.HasMember(positionKey.c_str())) {
				CCLOG("Successfully deleted position: %s", positionKey.c_str());
				return true;
			}
			else {
				CCLOG("Failed to delete position: %s", positionKey.c_str());
			}
			return true;
		}
	}
	return false;
}

// Add new animals (e.g., from breeding) to the archive
bool Animal::new_archive_in_memory(int x, int y) {
	// Get the document manager and archive document
	auto* docManager = DocumentManager::getInstance();
	auto* archiveDoc = docManager->getArchiveDocument();

	// Use the coordinates as the key to find the position
	const Vec<int>& position = Vec<int>(x, y);
	std::string positionKey = std::to_string(position.X()) + " " + std::to_string(position.Y());
	if (archiveDoc->HasMember("Map") && (*archiveDoc)["Map"].HasMember("chicken_house")) {
		rapidjson::Value& chickenHouse = (*archiveDoc)["Map"]["chicken_house"];

		// If there's already an entry for this position, stop to avoid conflict and log the issue
		if (chickenHouse.HasMember(positionKey.c_str())) {
			CCLOG("has object already��%s", positionKey.c_str());
			return false;
		}
		else {
			// If no data exists for this position, insert new data
			rapidjson::Document::AllocatorType& allocator = archiveDoc->GetAllocator();

			// Create new data for the animal
			rapidjson::Value newAnimalData(rapidjson::kObjectType);
			newAnimalData.AddMember("Type", "Animal", allocator);

			rapidjson::Value info(rapidjson::kObjectType);
			info.AddMember("AnimalType", "Chicken", allocator);
			info.AddMember("Value", 40, allocator);
			info.AddMember("Satiety", 10, allocator);
			info.AddMember("Length", 1, allocator);

			newAnimalData.AddMember("Info", info, allocator);

			// Add the new data to the "chicken_house" object
			chickenHouse.AddMember(rapidjson::Value(positionKey.c_str(), allocator), newAnimalData, allocator);

			// Log the insertion
			CCLOG("Inserted new archive for position: %s", positionKey.c_str());

			return true;
		}
	}
	else
		CCLOG("No such map for %s", type);
	return false;
}

void Animal::breed() {
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	float isBreed = 0.0f;//distribution(engine);
	if (isBreed > 0.2)                                  // Randomly decide if a new animal is born
		return;
	if (tag.size() <= 6) {                                // Max of six animals
		for (int i : lifePoints)
			if (!tag.count(i)) {
				tag.insert(i);                           // Find a suitable position for refresh
				Animal* newAnimal = new Animal(parent_, { i, 8 }, type, 50, 20, 1, 0.1f);
				new_archive_in_memory(i, 8);
				return;
			}
	}
}

void Animal::sold()
{
	if (!isSold) {
		isSold = 1;
		auto it = tag.find(info_.position.X());
		if (it != tag.end())
			tag.erase(info_.position.X());            // Remove the tag of the position where the sold animal was located
		else
			CCLOG("%s,sell function error", type);
		MainCharacter::getInstance()->modifyMoney(value);  // Earn money
		parent_->removeSpriteFromLayer(info_.sprite);
	}
	else
		CCLOG("Animal Sale Error, repeat sale: %s", type);
}

void Animal::clear()
{
	// Clear the sprite and related resources
	info_.sprite = nullptr;
}

void Animal::pause()
{
	isPause = true;                                 // Change state to stop movement
}

void Animal::resume()
{
	isPause = false;                                // Change state to resume movement
}

void Animal::settle()
{
	if (satiety < -20)
		delete_archive_in_memory();                 // Animal dies from starvation
	else
		change_archive_in_memory();                 // Update animal state
	if (isSold) {
		parent_->removeObject(info_);
		delete_archive_in_memory();
	}                                              // Delete sold animals
	breed();                                        // Perform breeding
}

bool Animal::hasCollision() {
	return false;
}
