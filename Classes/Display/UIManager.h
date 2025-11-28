#pragma once

#include "cocos2d.h"
#include <string>

class UIManager {
public:
    // 单例
    UIManager(const UIManager&) = delete;
    UIManager(UIManager&&) = delete;
    UIManager& operator=(const UIManager&) = delete;
    UIManager& operator=(UIManager&&) = delete;

    static UIManager* getInstance();

    // 初始化所有UI层
    void initUIs();

    // 获取包含所有UI层的根节点
    cocos2d::Node* getUINode();

    // 显示指定的UI层
    void showUILayer(const std::string& UI_name, bool base = false);

    // 隐藏UI层
    void hideUILayer(const std::string& UI_name);

private:
    UIManager();
    ~UIManager();

    static UIManager* instance_;

    std::string current_UI_name_;
    cocos2d::Node* permanent_node_ = nullptr;
};
