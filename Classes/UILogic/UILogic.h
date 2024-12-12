#pragma once
#include "ui/CocosGUI.h"
#include"HelperClasses.h"
#include"MainCharacter.h"
#include <string>
#include <vector>
#include "cocos2d.h"



struct Task {
    std::string description;
    bool completed;
    Task(const std::string& desc = "", bool done = false)
        : description(desc), completed(done) {}
};


class UILogic : public cocos2d::Ref
{
public:
    static UILogic* getInstance();

    void initStartScreenNode(cocos2d::Node* startScreenNode);

    void initBagNode(cocos2d::Node* bagNode);

    void initTaskBarNode(cocos2d::Node* taskBarNode);

    void refreshBagUI();

    void updateTaskUI();

    void updateBagItems(std::vector<Item> bagitem);

    void useItemFromBag(int slotIndex);

    void completeTask(int taskIndex);


private:
    UILogic();
    ~UILogic();

    void bindStartScreenEvents();

    void bindBagEvents();

    void bindTaskBarEvents();

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



