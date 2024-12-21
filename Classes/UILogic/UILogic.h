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

    void initNpcNode(cocos2d::Node* npcNode);

    void initLoadArchiveNode(cocos2d::Node* loadArchiveNode);

    void initTimeNode(cocos2d::Node* timeNode);

    void initShopNode(cocos2d::Node* shopNode);

    void initManufactureNode(cocos2d::Node* manufactureNode);

    void initFishNode(cocos2d::Node* fishNode);

    void refreshBagUI();

    void updateTaskUI();

    void updateBagItems(std::vector<Item>* bagitem);

    void useItemFromBag(int slotIndex);

    void completeTask(int taskIndex);

    void refreshArchiveUI();

    void LoadArchive(int saveIndex);

    void refreshTimeUI(int day,float hour);

    void refreshPowerUI(int power);

    void refreshMoneyUI(int money);

    void refreshNpcUI(std::string name);

    void refreshFishUI();
private:
    UILogic();
    ~UILogic();

    void bindStartScreenEvents();

    void bindBagEvents();

    void bindTaskBarEvents();

    void bindNpcEvents();

    void bindLoadArchiveEvents();

    void bindShopEvents();

    void bindManufactureEvents();

    void bindFishEvents();

    void onNewButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onLoadButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onExitButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onCloseTaskBarClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onCloseNpcClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onCloseFishClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onBagSlotClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onTaskItemClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onArchiveClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onCauliflowerClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onPotatoClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onPumpkinClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onSellCauliflowerClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onSellPotatoClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onSellPumpkinClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onFertilizerClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onSoupClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //the logic of tasks
    void initTasks();

    static UILogic* instance_;

    int UI_num = 0;
    cocos2d::Node* startScreenNode_;
    cocos2d::Node* bagNode_;
    cocos2d::Node* taskBarNode_;
    cocos2d::Node* npcNode_;
    cocos2d::Node* loadArchiveNode_;
    cocos2d::Node* timeNode_;
    cocos2d::Node* shopNode_;
    cocos2d::Node* manufactureNode_;
    cocos2d::Node* fishNode_;

    std::vector<Item>* bagItems_;
    std::vector<Task> tasks_;
};



