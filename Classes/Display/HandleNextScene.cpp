#include "SceneManager.h"
#undef private

#include "HandleNextScene.h"
#include "DocumentManager.h"
#include "UIManager.h"
#include "HelperClasses.h"
#include "TimeManager.h"
#include "PlayerSprite.h"
#include <stdexcept>

USING_NS_CC;

static constexpr int FRONT_UI_ZORDER = 10;
static constexpr int BACKGROUND_ZORDER = -30;
static constexpr int MAP_ZORDER = 0;

// 构造链
NextMapCallBack::NextMapCallBack(std::string map_name_, std::string pos_)
    : map_name(std::move(map_name_))
    , pos(std::move(pos_))
{
    // 创建处理器实例并链接
    head_ = new StartHandler();
    NextSceneHandler* createH = new CreateHandler();
    NextSceneHandler* renderH = new RenderHandler();
    NextSceneHandler* assembleH = new AssembleHandler();

    head_->setNext(createH);
    createH->setNext(renderH);
    renderH->setNext(assembleH);

    current_ = head_;
}

void NextMapCallBack::operator()()
{
    if (current_) {
        current_->handle(*this);
        current_ = current_->next();
    }
    if (loading_bar) {
        loading_bar->setPercent(loading_per);
    }
}

// StartHandler: 创建加载场景
void StartHandler::handle(NextMapCallBack& ctx)
{
    Scene* loading_scene = Scene::create();
    auto background = LayerColor::create(Color4B(Color3B(255, 248, 220)));
    loading_scene->addChild(background, BACKGROUND_ZORDER);

    ctx.loading_bar = ui::LoadingBar::create(DocumentManager::getInstance()->getPath("loading_bar"));
    ctx.loading_bar->setColor(Color3B(255, 165, 0));
    ctx.loading_bar->setDirection(ui::LoadingBar::Direction::LEFT);
    ctx.loading_bar->setScale(1.0f, 1.0f);
    ctx.loading_bar->setPosition(Director::getInstance()->getWinSize() / 2);
    loading_scene->addChild(ctx.loading_bar, 10);

    if (Director::getInstance()->getRunningScene() == nullptr) {
        Director::getInstance()->runWithScene(loading_scene);
    } else {
        SceneManager::getInstance()->map_.at(SceneManager::getInstance()->current_map_name_)->toBack();
        Director::getInstance()->replaceScene(loading_scene);
    }
    ctx.loading_per = 5.0f;
}

// CreateHandler: 创建或清理地图
void CreateHandler::handle(NextMapCallBack& ctx)
{
    SceneManager::getInstance()->createMaps();
    SceneManager::getInstance()->clearMaps();
    if (ctx.map_name == "introduction" && !SceneManager::getInstance()->map_.contains("introduction")) {
        DocumentManager* manager = DocumentManager::getInstance();
        rapidjson::Document* introduction = manager->getDocument(manager->getPath("introduction"));
        SceneManager::getInstance()->createMapWithDocument(introduction);
    }
    ctx.loading_per = 45.0f;
}

// RenderHandler: 前置目标地图
void RenderHandler::handle(NextMapCallBack& ctx)
{
    if (SceneManager::getInstance()->map_.contains(ctx.map_name)) {
        PlayerSprite* main_player = nullptr;
        if (ctx.pos != "default") {
            DocumentManager* manager = DocumentManager::getInstance();
            rapidjson::Document* doc = manager->getDocument(manager->getPath("Player"));
            main_player = PlayerSprite::create(doc, toVec2(ctx.pos), { 1, 1 }, (*manager->getConfigDocument())["always_run"].GetBool());
        }

        ctx.next_map = SceneManager::getInstance()->map_.at(ctx.map_name)->toFront(main_player);
        SceneManager::getInstance()->current_map_name_ = ctx.map_name;
        if (ctx.pos == "default") {
            ctx.next_map->getEventDispatcher()->removeEventListenersForTarget(ctx.next_map);
        }
    } else {
        throw std::logic_error("Map not found, The Archive or game set may be corrupted. Download it again.");
    }
    ctx.loading_per = 85.0f;
}

// AssembleHandler: 组装最终场景并替换
void AssembleHandler::handle(NextMapCallBack& ctx)
{
    Scene* next = Scene::create();
    next->addChild(ctx.next_map, MAP_ZORDER);
    ctx.next_map->release();

    UIManager* uiManager = UIManager::getInstance();
    Node* uiNode = uiManager->getUINode();
    uiNode->setParent(nullptr);
    next->addChild(uiNode, FRONT_UI_ZORDER);

    uiManager->hideUILayer("ALL");
    if (ctx.map_name == "introduction") {
        uiManager->showUILayer("startscreen", true);
    } else {
        uiManager->showUILayer("bag", true);
        uiManager->showUILayer("time", true);
    }

    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, next));
    ctx.loading_per = 100.0f;
}
