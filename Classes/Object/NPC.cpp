#include "NPC.h"
NPC::NPC(MapLayer* parent, const Vec<int>& pos, std::string npcName, int emo, int length,bool pause)
	: MapObject(pos), parent_(parent), name(npcName), emotion(emo),isPaused(pause)
{
	info_.size = Vec<int>(2, 2);
}

NPC::~NPC() = default;

MapObject* NPC::create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos)
{
	std::string npcName;
	int emo = 1, length = 1;
	if (val.HasMember("Name") && val["Name"].IsString()) {
		npcName = val["Name"].GetString();
	}
	if (val.HasMember("Length") && val["Length"].IsInt()) {
		length = val["Length"].GetInt();
	}
	if (val.HasMember("Emotion") && val["Emotion"].IsInt()) {
		length = val["Emotion"].GetInt();
	}
	return new NPC(parent, pos, npcName, emo, length,false);
}

void NPC::interact()
{
	MainCharacter* maincharacter = MainCharacter::getInstance();
	const Item* currentItem = maincharacter->getCurrentItem();
	PlayerSprite* npcSprite = dynamic_cast<PlayerSprite*>(info_.sprite);
	ItemType favorite;
	auto it = Favorite.find(name);  // 查找对应的场景

	UILogic::getInstance()->refreshNpcUI(name,emotion);
	SceneManager::getInstance()->showUILayer("npc");

	// SceneManager::getInstance()->hideUILayer("npc");
	if (!currentItem)
	{
		pause();
		npcSprite->schedule([=](float deltaTime) {
			npcSprite->move(PlayerSprite::MOVEMENT::STAY, 3);
			resume(); 
			}, 3.0f, "stay_key");

	}
	else if (it != Favorite.end()) {
		MainCharacter* mc = MainCharacter::getInstance();
		ItemType cit = mc->getCurrentItemType();
		if (cit == ItemType::CAULIFLOWER_SEED || cit == ItemType::PUMPKIN_SEED || cit == ItemType::FISH) {
			//give
			mc->modifyItemQuantity(mc->getCurrentItemType(), -1);
			// emotion
			favorite = it->second;
			if (currentItem->type == favorite) {
				emotion += 20;
			}
			else
				emotion += 10;
		}
		
	}
	else
		CCLOG("NPCinteract:name->favorite:error");
	
}

void NPC::init()
{
	DocumentManager* manager = DocumentManager::getInstance();
	rapidjson::Document* doc = manager->getDocument(manager->getPath(name));
	std::string plistFilePath = name+"Pls";
	MapLayer::loadPlist(plistFilePath);
	std::string frame_n_format = (*doc)["frame_format"].GetString();
	std::string spriteFrame = getFrameName(frame_n_format, 1);
	parent_->addPlayerSpriteWithDocument(info_, doc);

	defaultAction();
}

void NPC::defaultAction()
{
	if (parent_ == nullptr) {
		CCLOG("dafaulAction->parent_ nullptr");
		return;
	}
	PlayerSprite* npcSprite = dynamic_cast<PlayerSprite*>(info_.sprite);
	if (name == "Haley")
	{
		npcSprite->schedule([=](float deltaTime) {
			if (isPaused) {
				return;
			}

			npcSprite->move(PlayerSprite::MOVEMENT::W_LEFT, 6);

			npcSprite->scheduleOnce([=](float deltaTime) {
				if (isPaused) {
					return;
				}

				npcSprite->move(PlayerSprite::MOVEMENT::W_RIGHT, 6);
				}, 4.0f, "right_move_key");

			}, 11.0f, "left_move_key");
	}
	else if (name == "Caroline") {
		npcSprite->move(PlayerSprite::MOVEMENT::W_DOWN, 3);
		npcSprite->schedule([=](float deltaTime) {
			npcSprite->move(PlayerSprite::MOVEMENT::W_UP, 2);

			npcSprite->scheduleOnce([=](float deltaTime) {
				npcSprite->move(PlayerSprite::MOVEMENT::W_DOWN, 2);
				}, 4.0f, "down_move_key");

			}, 8.0f, "up_move_key");
	}
}

              
void NPC::change_archive_in_memory() {
	// Get the DocumentManager instance and the current archive document
	auto* docManager = DocumentManager::getInstance();
	auto* archiveDoc = docManager->getArchiveDocument();
	const Vec<int>& position = info_.position;
	// Convert position to a string to use as the map key
	std::string positionKey = std::to_string(position.X()) + " " + std::to_string(position.Y());
	std::string sceneKey ;
	auto it = Scene.find(name);  // 查找对应的场景
	if (it != Scene.end())
		sceneKey = it->second;
	else
		CCLOG("change_archive_in_memory:name->scene error");
	if (archiveDoc->HasMember("Map") && (*archiveDoc)["Map"].HasMember(sceneKey.c_str())) {
		rapidjson::Value& town = (*archiveDoc)["Map"]["Town"];

		// 确保 Town 中有对应的 positionKey
		if (town.HasMember(positionKey.c_str())) {
			rapidjson::Value& NPCInfo = town[positionKey.c_str()];

			// 确保 NPCInfo 中包含 "Info" 并且是对象
			if (NPCInfo.HasMember("Info") && NPCInfo["Info"].IsObject()) {
				rapidjson::Value& info = NPCInfo["Info"];

				// 如果有 "emotion" 整型，则更新它
				if (info.HasMember("emotion") && info["emotion"].IsInt()) {
					info["emotion"] = emotion;
				}
			}
		}
	}

}

void NPC::clear()
{
	PlayerSprite* npcSprite = dynamic_cast<PlayerSprite*>(info_.sprite);
	if (npcSprite != nullptr)
	{
		npcSprite->unschedule("right_move_key");
		npcSprite->unschedule("left_move_key");
		npcSprite->unschedule("stay_key");
	}
	info_.sprite = nullptr;
}

void NPC::pause()
{
	isPaused = true;
}

void NPC::resume()
{
	isPaused = false;
}


void NPC::settle()
{
	change_archive_in_memory();
}

bool NPC::hasCollision()
{
	return false;
}






