#include "SceneManager.h"
#include "HandleNextScene.h"
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
    // repeat=3 -> 执行4次，正好对应四个处理器（Start/Create/Render/Assemble）
    loader->schedule(functionCallback, instance_, interval, 3, 0.0f, false, "loading");
}

// TODO: 提取为外观模式
