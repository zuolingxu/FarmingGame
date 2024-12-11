#pragma once
#include "ui/CocosGUI.h"
#include"HelperClasses.h"
#include"MainCharacter.h"
#include <string>
#include <vector>
#include "cocos2d.h"


// 简单的任务结构体
struct Task {
    std::string description;
    bool completed;
    Task(const std::string& desc = "", bool done = false)
        : description(desc), completed(done) {}
};


class UILogic : public cocos2d::Ref
{
public:
    // 获取单例实例
    static UILogic* getInstance();

    // 设置开始界面节点（START_SCREEN类型），Node由UILayer返回
    void initStartScreenNode(cocos2d::Node* startScreenNode);

    // 设置背包节点（BAG类型），Node由UILayer返回
    void initBagNode(cocos2d::Node* bagNode);

    // 设置任务栏节点（TASK_BAR类型），Node由UILayer返回
    void initTaskBarNode(cocos2d::Node* taskBarNode);

    // 刷新背包UI
    void refreshBagUI();

    // 刷新任务UI
    void updateTaskUI();

    //refresh bag items
    void updateBagItems(std::vector<Item> bagitem);

    // 在点击格子时调用
    void useItemFromBag(int slotIndex);

    //// 标记任务完成
    void completeTask(int taskIndex);


private:
    UILogic();
    ~UILogic();

    // 初始化事件绑定函数
    void bindStartScreenEvents();

    void bindBagEvents();

    void bindTaskBarEvents();

     //按钮点击事件回调函数
    void onNewButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onLoadButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onExitButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onCloseBagButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onBagSlotClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onTaskItemClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //the logic of tasks
    void initTasks();

    static UILogic* instance_;

    cocos2d::Node* startScreenNode_;
    cocos2d::Node* bagNode_;
    cocos2d::Node* taskBarNode_;

    std::vector<Item> bagItems_;
    std::vector<Task> tasks_;

    DocumentManager* saveManager_;
    MainCharacter* mainCharacter_;
};



