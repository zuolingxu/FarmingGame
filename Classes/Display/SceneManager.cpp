#include "SceneManager.h"
#include "HelperClasses.h"
#include "audio/include/AudioEngine.h"
#include "ui/UILoadingBar.h"

USING_NS_CC;

SceneManager* SceneManager::instance_ = new SceneManager;

SceneManager::SceneManager() : director_(Director::getInstance())
{
	// TODO: load StartUI

	permanent_node_ = Node::create();
	permanent_node_->retain();
	permanent_node_->setName("UI");

	// TODO: createUILayer();
}

SceneManager::~SceneManager()
{
	permanent_node_->release();
}

void SceneManager::createMapWithDocument(rapidjson::Document* doc)
{
	std::string name = (*doc)["Name"].GetString();
	DocumentManager* manager = DocumentManager::getInstance();
	std::string tmx_path = (*doc)["TMX"].GetString();
	if (manager->hasDocument(tmx_path))
	{
		tmx_path = manager->getPath(tmx_path);
	}

	rapidjson::Value* const_object = nullptr, *archive_object = nullptr;
	if (doc->HasMember("ObjectList"))
	{
		 const_object = &(*doc)["ObjectList"];
	}
	rapidjson::Document* archive_doc = manager->getArchiveDocument();
	if ((*archive_doc)["Map"].HasMember(name.c_str()))
	{
		archive_object = &(*archive_doc)["Map"][name.c_str()];
	}

	MapLayer* map = MapLayer::createWithDocument(tmx_path, const_object, archive_object);
	map->retain();
	map_.emplace(name, map);
}

SceneManager* SceneManager::getInstance()
{
	return instance_;
}

void SceneManager::createMaps()
{
	DocumentManager* manager = DocumentManager::getInstance();
	const rapidjson::Document* doc = manager->getDocument(manager->getPath("global"));
	for (const rapidjson::Value& layer : (*doc)["Map"].GetArray())
	{
		createMapWithDocument(manager->getDocument(manager->getPath(layer.GetString())));
	}
}

void SceneManager::clearMaps()
{
	for (auto& map : map_)
	{
		map.second->release();
	}
	map_.clear();
}


void SceneManager::settle()
{
	for (auto& map : map_)
	{
		map.second->settle();
	}
}


void SceneManager::hideUILayer() const
{
	for (auto layer : permanent_node_->getChildren())
	{
		layer->setVisible(false);
		layer->pause();
		layer->setLocalZOrder(-10);
	}
	permanent_node_->setVisible(false);
	permanent_node_->setLocalZOrder(-10);
}

void SceneManager::changeUILayer(const std::string& UI_name) const
{
	for (auto layer : permanent_node_->getChildren())
	{
		if (layer->getName() != UI_name)
		{
			layer->setVisible(false);
			layer->pause();
			layer->setLocalZOrder(-10);
		}
		else
		{
			layer->setVisible(true);
			layer->resume();
			layer->setLocalZOrder(10);
		}
	}
}

void SceneManager::showUILayer(const std::string& UI_name) const 
{
	permanent_node_->setVisible(true);
	Node* layer = permanent_node_->getChildByName(UI_name);
	layer->setVisible(true);
	layer->resume();
	layer->setLocalZOrder(10);
	permanent_node_->setLocalZOrder(10);
}


void SceneManager::NextMap(const std::string& map_name, const std::string& pos) const
{
	Scene* temp = Scene::create();
	const auto background = cocos2d::LayerColor::create(cocos2d::Color4B::BLACK);
	temp -> addChild(background);

	auto loadingBar = ui::LoadingBar::create(DocumentManager::getInstance()->getPath("loading_bar"));
	loadingBar->setDirection(ui::LoadingBar::Direction::RIGHT);
	loadingBar->setScale(2.0f, 2.0f);
	temp->addChild(loadingBar);

	if (Director::getInstance()->getRunningScene() == nullptr)
	{
		Director::getInstance()->runWithScene(temp);
	}
	else
	{
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, temp));
	}


	if (map_.contains(map_name))
	{
		Scene* next = Scene::create();
		loadingBar->setPercent(40.0f);
		PlayerSprite* main_player = nullptr;
		if (pos != "default")
		{
			main_player = PlayerSprite::create();
			main_player->setPosition(toVec2(pos));
		}
		loadingBar->setPercent(50.0f);

		Node* map =  map_.at(map_name)->toFront(main_player);
		if (pos == "default")
		{
			map->getEventDispatcher()->removeEventListenersForTarget(map, true);
		}
		next->addChild(map,0);
		loadingBar->setPercent(90.0f);

		permanent_node_->setParent(nullptr);
		next->addChild(permanent_node_, -10);
		loadingBar->setPercent(100.0f);
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, next));
	}
	else
	{
		throw std::logic_error("Map not found, The Archive or game set may be corrupted. Download it again.");
	}
}
