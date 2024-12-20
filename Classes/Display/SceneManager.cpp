#include "SceneManager.h"
#include "HelperClasses.h"
#include "audio/include/AudioEngine.h"
#include "ui/UILoadingBar.h"
#include "UILogic.h"
#include "UILayer.h"

USING_NS_CC;

static constexpr int FRONT_UI_ZORDER = 10;
static constexpr int BACK_UI_ZORDER = -10;
static constexpr int BACKGROUND_ZORDER = -30;
static constexpr int MAP_ZORDER = 0;

SceneManager* SceneManager::instance_ = new SceneManager;

SceneManager::SceneManager() : director_(Director::getInstance()) {}

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

	bool create_able = false; 
	if (name == "farm") {
		create_able = true;
	}
	MapLayer* map = MapLayer::createWithDocument(tmx_path, backGroundColor, const_object, archive_object, create_able);
	map->retain();
	map_.emplace(name, map);
}

SceneManager* SceneManager::getInstance()
{
	if (instance_->permanent_node_ == nullptr) {
		instance_->permanent_node_ = Node::create();
		instance_->permanent_node_->retain();
		instance_->permanent_node_->setName("UI");
		Node* start_screen = UILayer::createUILayer(UILayerType::START_SCREEN);
		Node* bag = UILayer::createUILayer(UILayerType::BAG);
		Node* task_bar = UILayer::createUILayer(UILayerType::TASK_BAR);
		Node* npc = UILayer::createUILayer(UILayerType::NPC);
		Node* load_archive = UILayer::createUILayer(UILayerType::LOAD_ARCHIVE);
		Node* time = UILayer::createUILayer(UILayerType::TIME);
		Node* shop = UILayer::createUILayer(UILayerType::SHOP);
		Node* manufacture = UILayer::createUILayer(UILayerType::MANUFACTURE);
		instance_->permanent_node_->addChild(start_screen, BACK_UI_ZORDER);
		instance_->permanent_node_->addChild(bag, BACK_UI_ZORDER);
		instance_->permanent_node_->addChild(task_bar, BACK_UI_ZORDER);
		instance_->permanent_node_->addChild(npc, BACK_UI_ZORDER);
		instance_->permanent_node_->addChild(load_archive, BACK_UI_ZORDER);
		instance_->permanent_node_->addChild(time, BACK_UI_ZORDER);
		instance_->permanent_node_->addChild(shop, BACK_UI_ZORDER);
		instance_->permanent_node_->addChild(manufacture, BACK_UI_ZORDER);
		UILogic* uilogic = UILogic::getInstance();
		uilogic->initStartScreenNode(start_screen);
		uilogic->initStartScreenNode(bag);
		uilogic->initTaskBarNode(task_bar);
		uilogic->initNpcNode(npc);
		uilogic->initLoadArchiveNode(load_archive);
		uilogic->initShopNode(shop);
		uilogic->initManufactureNode(manufacture);
	}
	return instance_;
}

void SceneManager::createMaps()
{
	TimeManager::getInstance();
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
	// TODO: clearMainCharacter
}


void SceneManager::settle()
{
	for (auto& map : map_)
	{
		map.second->settle();
	}
}


//void SceneManager::hideUILayer() const
//{
//	for (auto layer : permanent_node_->getChildren())
//	{
//		layer->setVisible(false);
//		layer->pause();
//		layer->setLocalZOrder(BACK_UI_ZORDER);
//	}
//	permanent_node_->setVisible(false);
//	permanent_node_->setLocalZOrder(BACK_UI_ZORDER);
//}

void SceneManager::showUILayer(const std::string& UI_name) const 
{
	permanent_node_->setVisible(true);
	// map_.at(current_map_name_)->pause();

	permanent_node_->setLocalZOrder(FRONT_UI_ZORDER);
	for (auto layer : permanent_node_->getChildren())
	{
		if (layer->getName() == UI_name)
		{
			layer->setVisible(true);
			layer->resume();
			layer->setLocalZOrder(FRONT_UI_ZORDER);
		}
		else
		{
			layer->setVisible(false);
			layer->pause();
			layer->setLocalZOrder(BACK_UI_ZORDER);
		}
	}
}


void SceneManager::NextMap(const std::string& map_name, const std::string& pos) const
{
	auto loader = Director::getInstance()->getScheduler();
	auto call_back = std::make_shared<NextMapCallBack>(map_name, pos);

	auto functionCallback = std::function<void(float)>([call_back](float dt) {
		(*call_back)();  
		});
	loader->schedule(functionCallback, instance_, 0.02f, 3, 0.0f, false, "loading");
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
	Scene* loading_scene = Scene::create();
	auto background = cocos2d::LayerColor::create(cocos2d::Color4B(Color3B(255, 248, 220)));
	loading_scene->addChild(background, BACKGROUND_ZORDER);

	loading_bar = ui::LoadingBar::create(DocumentManager::getInstance()->getPath("loading_bar"));
	loading_bar->setColor(Color3B(255, 165, 0));
	loading_bar->setDirection(ui::LoadingBar::Direction::LEFT);
	loading_bar->setScale(1.0f, 1.0f);
	loading_bar->setPosition(Director::getInstance()->getWinSize() / 2);
	loading_scene->addChild(loading_bar, 10);

	if (Director::getInstance()->getRunningScene() == nullptr)
	{
		Director::getInstance()->runWithScene(loading_scene);
	}
	else
	{
		Director::getInstance()->replaceScene(loading_scene);
		getInstance()->map_.at(getInstance()->current_map_name_)->toBack();
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
			rapidjson::Document* doc = manager->getDocument(manager->getPath("Player"));
			main_player = PlayerSprite::create
			(doc, toVec2(pos), {1,1}, (*manager->getConfigDocument())["always_run"].GetBool());
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
	if (map_name == "introduction") 
	{
		getInstance()->showUILayer("taskbar");
	}
	else 
	{
		getInstance()->showUILayer("time");
	}

	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, next));
	loading_per = 100.0f;
}