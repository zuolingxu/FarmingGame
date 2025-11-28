#include "SceneManager.h"
#include "DocumentManager.h"
#include "HelperClasses.h"
#include "TimeManager.h"
#include "UIManager.h" // Add UIManager header
#include "cocos2d.h"
#include "ui/UILoadingBar.h"


USING_NS_CC;

static constexpr int FRONT_UI_ZORDER = 10;
static constexpr int BACK_UI_ZORDER = -10;
static constexpr int BACKGROUND_ZORDER = -30;
static constexpr int MAP_ZORDER = 0;

SceneManager* SceneManager::instance_ = new SceneManager;

SceneManager::SceneManager()
    : director_(Director::getInstance())
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::createMapWithDocument(rapidjson::Document* doc)
{
    std::string name = (*doc)["Name"].GetString();
    MapLayer* map = MapLayer::createWithDocument(doc);
    map->retain();
    map_.emplace(name, map);
}

// TODO：此处是构建SceneManager单例的地方，后续可以考虑用构建者模式重构（但只有这一个单例的话意义不大）
SceneManager* SceneManager::getInstance()
{
    // Initialize UIs when SceneManager is first accessed
    UIManager::getInstance()->initUIs();
    return instance_;
}

void SceneManager::createMaps()
{
    if (DocumentManager::getInstance()->getArchiveDocument() != nullptr && map_.size() <= 1) {
        TimeManager::getInstance();
        MainCharacter::getInstance();
        DocumentManager* manager = DocumentManager::getInstance();
        const rapidjson::Document* doc = manager->getDocument(manager->getPath("global"));
        for (const rapidjson::Value& layer : (*doc)["Map"].GetArray()) {
            createMapWithDocument(manager->getDocument(manager->getPath(layer.GetString())));
        }
    }
}

void SceneManager::clearMaps()
{
    if (DocumentManager::getInstance()->getArchiveDocument() == nullptr && map_.size() > 1) {
        for (auto& map : map_) {
            map.second->clearObjects();
            map.second->release();
        }
        MainCharacter::cleanup();
        TimeManager::cleanup();
        map_.clear();
    }
}

void SceneManager::settle()
{
    for (auto& map : map_) {
        map.second->settle();
    }
}

void SceneManager::NextMap(const std::string& map_name, const std::string& pos, const float interval) const
{
    auto loader = Director::getInstance()->getScheduler();
    auto call_back = std::make_shared<NextMapCallBack>(map_name, pos);

    auto functionCallback = std::function<void(float)>([call_back](float dt) {
        (*call_back)();
    });
    loader->schedule(functionCallback, instance_, interval, 3, 0.0f, false, "loading");
}

SceneManager::NextMapCallBack::NextMapCallBack(std::string map_name, std::string pos)
    : map_name(std::move(map_name))
    , pos(std::move(pos))
{
}

// TODO： 责任链
// 原先是依赖loading_per的数值范围来划分阶段，现在可以考虑用状态机或者责任链模式来实现更清晰的函数调用
// 这样可以避免对loading_per数值的硬编码依赖，使代码更易于维护和扩展
void SceneManager::NextMapCallBack::operator()()
{
    if (loading_per < 1.0f) {
        start();
    } else if (loading_per < 10.0f) {
        create();
    } else if (loading_per < 80.0f) {
        render();
    } else if (loading_per < 100.0f) {
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

    if (Director::getInstance()->getRunningScene() == nullptr) {
        Director::getInstance()->runWithScene(loading_scene);
    } else {
        getInstance()->map_.at(getInstance()->current_map_name_)->toBack();
        Director::getInstance()->replaceScene(loading_scene);
    }
    loading_per = 5.0f;
}

void SceneManager::NextMapCallBack::create()
{
    getInstance()->createMaps();
    getInstance()->clearMaps();
    if (map_name == "introduction" && !getInstance()->map_.contains("introduction")) {
        DocumentManager* manager = DocumentManager::getInstance();
        rapidjson::Document* introduction = manager->getDocument(manager->getPath("introduction"));
        getInstance()->createMapWithDocument(introduction);
    }
    loading_per = 45.0f;
}

void SceneManager::NextMapCallBack::render()
{
    if (getInstance()->map_.contains(map_name)) {
        PlayerSprite* main_player = nullptr;
        if (pos != "default") {
            DocumentManager* manager = DocumentManager::getInstance();
            rapidjson::Document* doc = manager->getDocument(manager->getPath("Player"));
            main_player = PlayerSprite::create(doc, toVec2(pos), { 1, 1 }, (*manager->getConfigDocument())["always_run"].GetBool());
        }

        next_map = getInstance()->map_.at(map_name)->toFront(main_player);
        getInstance()->current_map_name_ = map_name;
        if (pos == "default") {
            next_map->getEventDispatcher()->removeEventListenersForTarget(next_map);
        }
    } else {
        throw std::logic_error("Map not found, The Archive or game set may be corrupted. Download it again.");
    }

    loading_per = 85.0f;
}

void SceneManager::NextMapCallBack::assemble()
{
    Scene* next = Scene::create();
    next->addChild(next_map, MAP_ZORDER);

    next_map->release();

    UIManager* uiManager = UIManager::getInstance();
    Node* uiNode = uiManager->getUINode();
    uiNode->setParent(nullptr);
    next->addChild(uiNode, FRONT_UI_ZORDER);

    uiManager->hideUILayer("ALL");
    if (map_name == "introduction") {
        uiManager->showUILayer("startscreen", true);
    } else {
        uiManager->showUILayer("bag", true);
        uiManager->showUILayer("time", true);
    }

    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, next));
    loading_per = 100.0f;
}

// TODO: 提取为外观模式
