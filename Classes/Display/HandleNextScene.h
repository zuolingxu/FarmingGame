#pragma once
#include <string>
#include <memory>
#include "cocos2d.h"
#include "ui/UILoadingBar.h"

class SceneManager; // forward
class DocumentManager;
class UIManager;
class PlayerSprite;

// 前向声明辅助转换
cocos2d::Vec2 toVec2(const std::string& str);

// 抽象处理器
class NextSceneHandler {
public:
    NextSceneHandler() = default;
    virtual ~NextSceneHandler() = default;
    void setNext(NextSceneHandler* n) { next_ = n; }
    NextSceneHandler* next() const { return next_; }
    virtual void handle(class NextMapCallBack& ctx) = 0;
protected:
    NextSceneHandler* next_ = nullptr;
};

// 回调上下文：负责调度链
class NextMapCallBack {
public:
    NextMapCallBack(std::string map_name, std::string pos);
    void operator()(); // 每次调度推进一个处理器

    std::string map_name;
    std::string pos;
    float loading_per = 0.0f;
    cocos2d::ui::LoadingBar* loading_bar = nullptr;
    cocos2d::Node* next_map = nullptr;

private:
    NextSceneHandler* head_ = nullptr;
    NextSceneHandler* current_ = nullptr;
};

// 具体处理器：四个阶段
class StartHandler : public NextSceneHandler {
public:
    void handle(NextMapCallBack& ctx) override;
};

class CreateHandler : public NextSceneHandler {
public:
    void handle(NextMapCallBack& ctx) override;
};

class RenderHandler : public NextSceneHandler {
public:
    void handle(NextMapCallBack& ctx) override;
};

class AssembleHandler : public NextSceneHandler {
public:
    void handle(NextMapCallBack& ctx) override;
};
