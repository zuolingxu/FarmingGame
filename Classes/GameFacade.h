// FarmingGame/Classes/GameFacade.h

#pragma once

#include <string>

// 前置声明以减少头文件依赖
class SceneManager;
class TimeManager;
class DocumentManager;
class UIManager; // 新增 UIManager
class MainCharacter;

// GameFacade 是一个单例，作为游戏引擎的统一接口
class GameFacade {
public:
    // 获取单例实例
    static GameFacade* getInstance();

    // 删除拷贝和移动构造，确保单例唯一性
    GameFacade(const GameFacade&) = delete;
    GameFacade(GameFacade&&) = delete;
    GameFacade& operator=(const GameFacade&) = delete;
    GameFacade& operator=(const GameFacade&&) = delete;

    // --- 高级业务流程接口 ---

    // 启动游戏，加载初始场景
    void startGame();

    // 加载指定存档并进入游戏
    void loadGame(int archiveNumber);

    // 切换场景
    void changeScene(const std::string& mapName, const std::string& position = "default");

    // 显示UI层
    void showUI(const std::string& uiName, bool isBaseLayer = false);

    // 隐藏UI层
    void hideUI(const std::string& uiName);

    // 进入下一天（睡眠/时间耗尽）
    void goToNextDay();

private:
    // 私有构造函数
    GameFacade();
    ~GameFacade() = default;

    // 单例实例指针
    static GameFacade* instance_;

    // 持有对核心子系统的引用
    SceneManager* sceneManager_;
    TimeManager* timeManager_;
    DocumentManager* docManager_;
    UIManager* uiManager_; // 新增 uiManager_
};
