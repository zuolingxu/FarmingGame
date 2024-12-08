#include "SceneManager.h"

#include <poly2tri/sweep/advancing_front.h>

#include "HelperClasses.h"
#include "audio/include/AudioEngine.h"
#include "ui/UILoadingBar.h"

USING_NS_CC;

static constexpr int FRONT_UI_ZORDER = 10;
static constexpr int BACK_UI_ZORDER = -10;
static constexpr int BACKGROUND_ZORDER = -30;
static constexpr int MAP_ZORDER = 0;
static constexpr float SCALE_FACTOR = 1.5f;

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

	// default background is black
	Color3B backGroundColor = {0,0,0};
	if (doc->HasMember("BackGroundColor"))
	{
		backGroundColor = Color3B((*doc)["BackGroundColor"][0].GetInt(),
			(*doc)["BackGroundColor"][1].GetInt(), (*doc)["BackGroundColor"][2].GetInt());
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

	MapLayer* map = MapLayer::createWithDocument(tmx_path, backGroundColor, const_object, archive_object);
	map->retain();
	map_.emplace(name, map);
}

SceneManager* SceneManager::getInstance()
{
	return instance_;
}

void SceneManager::createMaps()
{
	if (DocumentManager::getInstance()->getArchiveDocument() != nullptr && map_.empty()){
		DocumentManager* manager = DocumentManager::getInstance();
		const rapidjson::Document* doc = manager->getDocument(manager->getPath("global"));
		for (const rapidjson::Value& layer : (*doc)["Map"].GetArray())
		{
			createMapWithDocument(manager->getDocument(manager->getPath(layer.GetString())));
		}
	}
}

void SceneManager::clearMaps()
{
	if (DocumentManager::getInstance()->getArchiveDocument() == nullptr && !map_.empty()){
		for (auto& map : map_)
		{
			map.second->clearObjects();
			map.second->release();
		}
		map_.clear();
	}
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
		layer->setLocalZOrder(BACK_UI_ZORDER);
	}
	permanent_node_->setVisible(false);
	permanent_node_->setLocalZOrder(BACK_UI_ZORDER);
}

void SceneManager::changeUILayer(const std::string& UI_name) const
{
	for (auto layer : permanent_node_->getChildren())
	{
		if (layer->getName() != UI_name)
		{
			layer->setVisible(false);
			layer->pause();
			layer->setLocalZOrder(BACK_UI_ZORDER);
		}
		else
		{
			layer->setVisible(true);
			layer->resume();
			layer->setLocalZOrder(FRONT_UI_ZORDER);
		}
	}
}

void SceneManager::showUILayer(const std::string& UI_name) const 
{
	permanent_node_->setVisible(true);
	Node* layer = permanent_node_->getChildByName(UI_name);
	map_.at(current_map_name_)->pause();
	if (layer != nullptr){
		layer->setVisible(true);
		layer->resume();
		layer->setLocalZOrder(FRONT_UI_ZORDER);
		permanent_node_->setLocalZOrder(FRONT_UI_ZORDER);
	}
}


void SceneManager::NextMap(const std::string& map_name, const std::string& pos) const
{
	auto loader = Director::getInstance()->getScheduler();
	auto call_back = std::make_shared<NextMapCallBack>(map_name, pos);

	auto functionCallback = std::function<void(float)>([call_back](float dt) {
		(*call_back)();  
		});
	loader->schedule(functionCallback, instance_, 0.0f, 3, 0.0f, false, "loading");
}


SceneManager::NextMapCallBack::NextMapCallBack(std::string map_name, std::string pos) :
	map_name(std::move(map_name)), pos(std::move(pos)) {}

void SceneManager::NextMapCallBack::operator()()
{
	if (loading_per < 1.0f)
	{
		start();
	}
	else if (loading_per < 10.0f)
	{
		create();
	}
	else if (loading_per < 80.0f)
	{
		render();
	}
	else if (loading_per < 100.0f)
	{
		assemble();
	}
	loading_bar->setPercent(loading_per);
}

void SceneManager::NextMapCallBack::start()
{
	loading_scene = Scene::create();
	auto background = cocos2d::LayerColor::create(cocos2d::Color4B(Color3B(255, 248, 220)));
	loading_scene->addChild(background, BACKGROUND_ZORDER);

	loading_bar = ui::LoadingBar::create(DocumentManager::getInstance()->getPath("loading_bar"));
	loading_bar->setColor(Color3B(255, 165, 0));
	loading_bar->setDirection(ui::LoadingBar::Direction::LEFT);
	loading_bar->setScale(1.0f, 1.0f);
	loading_bar->setPosition(Director::getInstance()->getWinSizeInPixels() / 2);
	loading_scene->addChild(loading_bar, 10);

	if (Director::getInstance()->getRunningScene() == nullptr)
	{
		Director::getInstance()->runWithScene(loading_scene);
	}
	else
	{
		getInstance()->map_.at(getInstance()->current_map_name_)->toBack();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, loading_scene));
	}
	loading_per = 5.0f;
}


void SceneManager::NextMapCallBack::create()
{
	getInstance()->createMaps();
	getInstance()->clearMaps();
	if (map_name == "introduction" && !getInstance()->map_.contains("introduction"))
	{
		DocumentManager* manager = DocumentManager::getInstance();
		rapidjson::Document* introduction = manager->getDocument(manager->getPath("introduction"));
		getInstance()->createMapWithDocument(introduction);
	}
	loading_per = 45.0f;
}

void SceneManager::NextMapCallBack::render()
{
	if (getInstance()->map_.contains(map_name))
	{
		PlayerSprite* main_player = nullptr;
		if (pos != "default")
		{
			DocumentManager* manager = DocumentManager::getInstance();
			rapidjson::Document* doc = manager->getDocument(manager->getPath("player"));
			main_player = PlayerSprite::create
			(doc, (*manager->getConfigDocument())["always_run"].GetBool());
			main_player->setPosition(toPixel(toVec2(pos)));
		}

		next_map = getInstance()->map_.at(map_name)->toFront(main_player);
		getInstance()->current_map_name_ = map_name;
		if (pos == "default")
		{
			next_map->getEventDispatcher()->removeEventListenersForTarget(next_map);
		}
	}
	else
	{
		throw std::logic_error("Map not found, The Archive or game set may be corrupted. Download it again.");
	}

	loading_per = 85.0f;
}

void SceneManager::NextMapCallBack::assemble()
{
	Scene* next = Scene::create();
	next->addChild(next_map, MAP_ZORDER);

	next_map->release();
	getInstance()->permanent_node_->setParent(nullptr);
	next->addChild(getInstance()->permanent_node_, BACK_UI_ZORDER);

	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, next));
	loading_per = 100.0f;
}