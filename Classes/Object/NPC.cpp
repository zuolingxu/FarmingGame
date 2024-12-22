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
	return new NPC(parent, pos, npcName, emo, length,false);
}

void NPC::interact()
{
	MainCharacter* maincharacter = MainCharacter::getInstance();
	const Item* currentItem = maincharacter->getCurrentItem();
	PlayerSprite* npcSprite = dynamic_cast<PlayerSprite*>(info_.sprite);
	ItemType favorite;
	auto it = Favorite.find(name);  // ���Ҷ�Ӧ�ĳ���
	UILogic::getInstance()->refreshNpcUI(name,100);
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
		favorite = it->second;
		if (currentItem->type == favorite) {
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
	{		npcSprite->schedule([=](float deltaTime) {
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
	else if (name == "Abigail") {
		;
	}
	else if (name == "Caroline") {
		;
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
	auto it = Scene.find(name);  // ���Ҷ�Ӧ�ĳ���
	if (it != Scene.end())
		sceneKey = it->second;
	else
		CCLOG("change_archive_in_memory:name->scene error");
	if (archiveDoc->HasMember("Map") && (*archiveDoc)["Map"].HasMember(sceneKey.c_str())) {
		rapidjson::Value& town = (*archiveDoc)["Map"]["Town"];

		// ȷ�� Town ���ж�Ӧ�� positionKey
		if (town.HasMember(positionKey.c_str())) {
			rapidjson::Value& NPCInfo = town[positionKey.c_str()];

			// ȷ�� NPCInfo �а��� "Info" �����Ƕ���
			if (NPCInfo.HasMember("Info") && NPCInfo["Info"].IsObject()) {
				rapidjson::Value& info = NPCInfo["Info"];

				// ����� "emotion" ���ͣ��������
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











/*
	if (name == "Haley")
		// ����֡����
	{
		cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(name + ".plist", name + ".png");

		// ����һ�� Vector �洢 SpriteFrame ����
		cocos2d::Vector<cocos2d::SpriteFrame*> frames;

		// ������֪�� plist �е�֡������ "chicken-1.png", "chicken-2.png"
		for (int i = 1; i <= 2; i++) {
			std::string frameName = name + "-" + std::to_string(i) + ".png"; // ��̬����֡��
			frames.pushBack(cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
		}

		// ����һ�� Animation ����
		auto animation = cocos2d::Animation::createWithSpriteFrames(frames, 0.1f); // ÿ֡��ʱ���� 0.1f

		// ����һ�� Animate ����
		auto animate = cocos2d::Animate::create(animation);

		// ��ȡһ�����鲢���Ŷ���
		auto animalSprite = cocos2d::Sprite::createWithSpriteFrameName("Haley-1.png"); // ��ʼ������
		animalSprite->setPosition(cocos2d::Vec2(40, 2)); // ���ó�ʼλ��

		// �����ƶ�����
		auto moveAction = cocos2d::MoveTo::create(10.0f, cocos2d::Vec2(10, 2));  // 2�뵽���µ�λ��

		// ʹ�� Spawn ��ͬʱִ���ƶ��Ͷ���
		auto moveAndAnimate = cocos2d::Spawn::create(moveAction, animate, nullptr);

		// ����������ƶ��Ͷ���
		animalSprite->runAction(moveAndAnimate);
		//parent->addChild();
	}  // ��������ӵ���ǰ����
	*/