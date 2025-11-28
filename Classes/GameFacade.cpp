#include "GameFacade.h"
#include "Display/SceneManager.h"
#include "Display/UIManager.h" // 引入UIManager头文件
#include "HelperClasses/DocumentManager.h"
#include "HelperClasses/TimeManager.h"
#include "UILogic/MainCharacter.h"

// 初始化静态单例实例
GameFacade* GameFacade::instance_ = nullptr;

GameFacade* GameFacade::getInstance()
{
    if (instance_ == nullptr) {
        instance_ = new GameFacade();
    }
    return instance_;
}

GameFacade::GameFacade()
{
    // 获取核心子系统的单例实例
    sceneManager_ = SceneManager::getInstance();
    timeManager_ = TimeManager::getInstance();
    docManager_ = DocumentManager::getInstance();
    uiManager_ = UIManager::getInstance(); // 初始化UIManager
}

void GameFacade::startGame()
{
    // 启动游戏，进入标题场景
    sceneManager_->goToTitleScene();
}

void GameFacade::loadGame(int archiveNumber)
{
    // 加载指定存档
    docManager_->load(archiveNumber);
    // 进入农场场景
    sceneManager_->goToFarmScene();
}

void GameFacade::changeScene(const std::string& mapName, const std::string& position)
{
    // 封装场景切换逻辑
    sceneManager_->changeMap(mapName, position);
}

void GameFacade::showUI(const std::string& uiName, bool isBaseLayer)
{
    // 委托给UIManager处理
    uiManager_->showUILayer(uiName, isBaseLayer);
}

void GameFacade::hideUI(const std::string& uiName)
{
    // 委托给UIManager处理
    uiManager_->hideUILayer(uiName);
}

void GameFacade::goToNextDay()
{
    // 封装进入下一天的逻辑
    timeManager_->goToNextDay();
}
