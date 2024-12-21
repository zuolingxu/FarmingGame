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
	MapLayer* map = MapLayer::createWithDocument(doc);
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
		Node* fish = UILayer::createUILayer(UILayerType::FISHING);
		instance_->permanent_node_->addChild(start_screen, BACK_UI_ZORDER);
		instance_->permanent_node_->addChild(bag, BACK_UI_ZORDER);
		instance_->permanent_node_->addChild(task_bar, BACK_UI_ZORDER);
		instance_->permanent_node_->addChild(npc, BACK_UI_ZORDER);
		instance_->permanent_node_->addChild(load_archive, BACK_UI_ZORDER);
		instance_->permanent_node_->addChild(time, BACK_UI_ZORDER);
		instance_->permanent_node_->addChild(shop, BACK_UI_ZORDER);
		instance_->permanent_node_->addChild(manufacture, BACK_UI_ZORDER);
		instance_->permanent_node_->addChild(fish, BACK_UI_ZORDER);
		UILogic* uilogic = UILogic::getInstance();
		uilogic->initStartScreenNode(start_screen);
		uilogic->initStartScreenNode(bag);
		uilogic->initTaskBarNode(task_bar);
		uilogic->initNpcNode(npc);
		uilogic->initLoadArchiveNode(load_archive);
		uilogic->initShopNode(shop);
		uilogic->initManufactureNode(manufacture);
		uilogic->initFishNode(fish);
	}
	return instance_;
}

void SceneManager::createMaps()
{
	if (DocumentManager::getInstance()->getArchiveDocument() != nullptr && map_.size() <= 1){
		TimeManager::getInstance();
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
	if (DocumentManager::getInstance()->getArchiveDocument() == nullptr && !map_.size() > 1){
		TimeManager::cleanup();
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


void SceneManager::hideUILayer(const std::string& UI_name)
{
	if (UI_name == "ALL") {
		for (auto layer : permanent_node_->getChildren()) {
			layer->setVisible(false);
			layer->setLocalZOrder(BACK_UI_ZORDER);
			layer->pause();
		}
		current_UI_name_.clear();
	}
	else if (UI_name == "CURRENT") {
		if (!current_UI_name_.empty()) {
			Node* layer = permanent_node_->getChildByName(current_UI_name_);
			if (layer != nullptr) {
				layer->setVisible(false);
				layer->setLocalZOrder(BACK_UI_ZORDER);
				layer->pause();
			}
			current_UI_name_.clear();
		}
	}
	else {
		Node* layer = permanent_node_->getChildByName(UI_name);
		if (layer != nullptr) {
			layer->setVisible(false);
			layer->setLocalZOrder(BACK_UI_ZORDER);
			layer->pause();
			current_UI_name_.clear();
		}
	}

}

void SceneManager::showUILayer(const std::string& UI_name, bool base)
{
	Node* layer = permanent_node_->getChildByName(UI_name);
	if (layer != nullptr) {
		if (!base) {
			hideUILayer("CURRENT");
			current_UI_name_ = UI_name;
		}
		layer->setVisible(true);
		layer->setLocalZOrder(FRONT_UI_ZORDER + base ? -1 : 0);
		layer->resume();
	}
}


void SceneManager::NextMap(const std::string& map_name, const std::string& pos , const float interval) const
{
	auto loader = Director::getInstance()->getScheduler();
	auto call_back = std::make_shared<NextMapCallBack>(map_name, pos);

	auto functionCallback = std::function<void(float)>([call_back](float dt) {
		(*call_back)();  
		});
	loader->schedule(functionCallback, instance_, interval, 3, 0.0f, false, "loading");
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
	next->addChild(getInstance()->permanent_node_, FRONT_UI_ZORDER);
	getInstance()->hideUILayer("ALL");
	if (map_name == "introduction") 
	{
		getInstance()->showUILayer("startscreen", true);
	}
	else 
	{
		getInstance()->showUILayer("bag", true);
		getInstance()->showUILayer("time", true);
	}

	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, next));
	loading_per = 100.0f;
}