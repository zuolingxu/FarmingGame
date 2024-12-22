#pragma once
#include "ui/CocosGUI.h"
#include"HelperClasses.h"
#include"MainCharacter.h"
#include <string>
#include <vector>
#include "cocos2d.h"


//the struct of task
struct Task {
    std::string description;
    bool completed;
    bool clicked;
    Task(const std::string& desc = "", bool done = false)
        : description(desc), completed(done),clicked(done) {}
};


class UILogic : public cocos2d::Ref
{
public:
    static UILogic* getInstance();//get the instance of UILogic

    //initialize the UI nodes
    void initStartScreenNode(cocos2d::Node* startScreenNode);

    void initBagNode(cocos2d::Node* bagNode);

    void initTaskBarNode(cocos2d::Node* taskBarNode);

    void initNpcNode(cocos2d::Node* npcNode);

    void initLoadArchiveNode(cocos2d::Node* loadArchiveNode);

    void initTimeNode(cocos2d::Node* timeNode);

    void initShopNode(cocos2d::Node* shopNode);

    void initManufactureNode(cocos2d::Node* manufactureNode);

    void initFishNode(cocos2d::Node* fishNode);

    //give the UI nodes to the MainCharacter and other classes
    void refreshBagUI();

    //update the taskbarUI
    void updateTaskUI();

    //update the bag items and refresh the bagUI
    void updateBagItems(std::vector<Item>* bagitem);

    //give the MainCharacter the bagitem that is clicked
    void useItemFromBag(int slotIndex);

    //mark the completed task as completed
    void completeTask(int taskIndex);

    //update the loadarchiveUI
    void refreshArchiveUI();

    //load the archive and enter the game
    void LoadArchive(int saveIndex);

    //accept the time from TimeManager and update the timeUI
    void refreshTimeUI(int day,float hour);

    //accept the power from MainCharacter and update the timeUI
    void refreshPowerUI(int power);

    //accept the money from MainCharacter and update the timeUI
    void refreshMoneyUI(int money);

    //accept the level from MainCharacter  and update the timeUI
    void refreshLevelUI(int level);

    //accept the name and favorability from npc and update the npcUI
    void refreshNpcUI(std::string name,int favor);

    //update the fishUI 
    void refreshFishUI();
private:
    UILogic();
    ~UILogic();

    //bind the eventlisteners to the buttons
    void bindStartScreenEvents();

    void bindBagEvents();

    void bindTaskBarEvents();

    void bindNpcEvents();

    void bindLoadArchiveEvents();

    void bindShopEvents();

    void bindManufactureEvents();

    void bindFishEvents();

    //the callback functions for the buttons
    //create a new archive and enter the game
    void onNewButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //load the loadarchiveUI
    void onLoadButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //exit the game
    void onExitButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //close the taskbarUI
    void onCloseTaskBarClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //close the npcUI
    void onCloseNpcClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //close the fishUI
    void onCloseFishClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //choose the bagitem int the clicked bag slot and give it to MainCharacter
    void onBagSlotClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //if a task is completed,click it and set it invisible
    void onTaskItemClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //load a exist archive and enter the game
    void onArchiveClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //buy a cauliflower seed and reduce money
    void onCauliflowerClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //buy a potato seed and reduce money
    void onPotatoClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //buy a pumpkin seed and reduce money
    void onPumpkinClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //sell a cauliflower and add money
    void onSellCauliflowerClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //sell a potato and add money
    void onSellPotatoClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //sell a pumpkin and add money
    void onSellPumpkinClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //add a fertilizer and reduce rock quantity
    void onFertilizerClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //add a soup and reduce cauliflower quantity
    void onSoupClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    //the logic of tasks
    void initTasks();
    //the logic of npc sentences
    void initSentence();

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
    std::vector<std::string> Sentence;
};



