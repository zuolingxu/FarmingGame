#include <format>
#include "DocumentManager.h"
#include "SceneManager.h"
#include "json/document.h"
#include "UILogic.h"

USING_NS_CC;
#ifdef _MSC_VER
#undef GetObject
#endif

// 静态实例初始化
UILogic* UILogic::instance_ = new UILogic();

UILogic* UILogic::getInstance()
{
    if (!instance_)
    {
        instance_ = new UILogic();
    }
    return instance_;
}

UILogic::UILogic()
    : startScreenNode_(nullptr)
    , bagNode_(nullptr)
    , taskBarNode_(nullptr)
    , mainCharacter_(nullptr)
{
//    // 获取其他管理器实例
    saveManager_ = DocumentManager::getInstance();
    mainCharacter_ = MainCharacter::getInstance();
}

UILogic::~UILogic()
{

}

void UILogic::initStartScreenNode(cocos2d::Node* startScreenNode)
{
    startScreenNode_ = startScreenNode;
    bindStartScreenEvents(); 
}

void UILogic::initBagNode(cocos2d::Node* bagNode)
{
    bagNode_ = bagNode;
    bindBagEvents(); 
    refreshBagUI();  
}

void UILogic::initTaskBarNode(cocos2d::Node* taskBarNode)
{
    taskBarNode_ = taskBarNode;
    bindTaskBarEvents(); 
    updateTaskUI();      
}

void UILogic::bindStartScreenEvents()
{
    if (!startScreenNode_) return;

    auto newButton = dynamic_cast<ui::Button*>(startScreenNode_->getChildByName("NewButton"));
    if (newButton) {
        newButton->addTouchEventListener(CC_CALLBACK_2(UILogic::onNewButtonClicked, this));
    }

    auto loadButton = dynamic_cast<ui::Button*>(startScreenNode_->getChildByName("LoadButton"));
    if (loadButton) {
        loadButton->addTouchEventListener(CC_CALLBACK_2(UILogic::onLoadButtonClicked, this));
    }

    auto exitButton = dynamic_cast<ui::Button*>(startScreenNode_->getChildByName("ExitButton"));
    if (exitButton) {
        exitButton->addTouchEventListener(CC_CALLBACK_2(UILogic::onExitButtonClicked, this));
    }
}

void UILogic::bindBagEvents()
{
    if (!bagNode_) return;

    const int numSlots = 24;
    for (int i = 0; i < numSlots; ++i)
    {
        auto slot = dynamic_cast<ui::Button*>(bagNode_->getChildByName("Slot_" + std::to_string(i)));
        if (slot) {
            slot->addTouchEventListener(CC_CALLBACK_2(UILogic::onBagSlotClicked, this));
        }
    }
}


void UILogic::bindTaskBarEvents()
{
    if (!taskBarNode_) return;

    auto closeButton = dynamic_cast<ui::Button*>(bagNode_->getChildByName("CloseButton"));
    if (closeButton)
    {
        closeButton->addTouchEventListener(CC_CALLBACK_2(UILogic::onCloseBagButtonClicked, this));
    }
}

void UILogic::onNewButtonClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type)
{
    int i = 1;
    while (!DocumentManager::getInstance()->createArchiveDocument(i)) { i++; }
    //rapidjson::Value& val = (*DocumentManager::getInstance()->getConfigDocument())["Archive"];
    //for (auto& i : val.GetObject()) {
    //    i.name.GetString();
    //    i.value.GetObject();
    //}
}

void UILogic::onLoadButtonClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type)
{
//    if (type != ui::Widget::TouchEventType::ENDED) return;
//
//    CCLOG("Load Button Clicked");
//    if (saveManager_->hasSave())
//    {
//        bool loaded = saveManager_->loadSave();
//        if (loaded)
//        {
//            CCLOG("Save loaded.");
//            std::string mapName = saveManager_->getSavedMapName();
//            std::string playerPos = saveManager_->getSavedPlayerPosition();
//            sceneManager_->switchToMap(mapName, playerPos);
//            startScreenNode_->setVisible(false);
//
//            // 重新加载数据并刷新UI
//            loadDataFromSave();
//            refreshBagUI();
//            updateTaskUI();
//        }
//        else
//        {
//            CCLOG("Failed to load save.");
//        }
//    }
//    else
//    {
//        CCLOG("No save found.");
//    }
}

void UILogic::onExitButtonClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type)
{

    CCLOG("Exit Button Clicked");
    Director::getInstance()->end();

}

void UILogic::onCloseBagButtonClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;

    if (bagNode_)
    {
        bagNode_->setVisible(false);
    }
}

void UILogic::onBagSlotClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;

    auto button = dynamic_cast<ui::Button*>(sender);
    if (!button) return;

    std::string name = button->getName(); // "Slot_x"
    int slotIndex = std::stoi(name.substr(5));

    useItemFromBag(slotIndex);
}

void UILogic::useItemFromBag(int slotIndex)
{

    auto& item = bagItems_[slotIndex];
    if (item.quantity <= 0)
    {
        CCLOG("This slot is empty.");
        return;
    }

    refreshBagUI();

    // TODO:将该物品返回给MainCharacter
    mainCharacter_->setCurrentItem(item.type);
}

void UILogic::onTaskItemClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;

    auto button = dynamic_cast<ui::Button*>(sender);
    if (!button) return;

    std::string taskName = button->getName(); // "Task_x"
    int taskIndex = std::stoi(taskName.substr(5));

    if (!tasks_[taskIndex].completed) {
        return;
    }

    tasks_.erase(tasks_.begin() + taskIndex);
    updateTaskUI();
}

void UILogic::updateBagItems(std::vector<Item> bagitem) {
    bagItems_ = bagitem;
}

void UILogic::refreshBagUI()
{

    if (!bagNode_) return;

    const int numSlots = 24;
    const int columns = 12; // 每行12个
    const int rows = 2; // 2行
    const float slotSize = 20.0f; // 每个格子的大小

    for (int i = 0; i < numSlots; ++i)
    {
        int row = i / columns;
        int col = i % columns;

        auto slot = dynamic_cast<ui::Button*>(bagNode_->getChildByName("Slot_" + std::to_string(i)));
        if (!slot) continue;

        slot->removeAllChildren(); 

        if (i < (int)bagItems_.size())
        {
            const auto& item = bagItems_[i];
            if (!item.iconPath.empty())
            {
                auto itemSprite = Sprite::create(item.iconPath);
                if (itemSprite)
                {
                    itemSprite->setContentSize(Size(slotSize, slotSize));
                    itemSprite->setPosition(Vec2(col * slotSize + slotSize / 2 + 120, row * slotSize + slotSize / 2 ));
                    slot->addChild(itemSprite);
                }

                if (item.quantity > 1)
                {
                    auto quantityLabel = ui::Text::create(std::to_string(item.quantity), "Arial", 10);
                    quantityLabel->setTextColor(Color4B::WHITE);
                    quantityLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
                    quantityLabel->setPosition(Vec2(col * slotSize + slotSize / 2 + 125, row * slotSize + slotSize / 2 -5));
                    slot->addChild(quantityLabel);
                }
            }
        }
    }
}

void UILogic::updateTaskUI()
{

    if (!taskBarNode_) return;

    taskBarNode_->removeAllChildren();

    for (int i = 0; i < (int)tasks_.size(); ++i)
    {
        const Task& task = tasks_[i];
        auto taskButton = ui::Button::create("image/textBox..png", "image/textBox..png");
        taskButton->setScale9Enabled(true);
        taskButton->setContentSize(Size(360, 50));
        taskButton->setPosition(Vec2(240, 100 + i * 50));
        taskButton->setName("Task_" + std::to_string(i));

        auto taskLabel = ui::Text::create(task.description, "Arial", 10);
        taskLabel->setPosition(Vec2(180,25));
        taskButton->addChild(taskLabel);

        if (task.completed)
        {
            taskButton->setEnabled(true);
            taskButton->addTouchEventListener(CC_CALLBACK_2(UILogic::onTaskItemClicked, this));
        }
        else
        {
            taskButton->setEnabled(false);
            taskButton->setBright(false);
        }

        taskBarNode_->addChild(taskButton);
    }
}

void UILogic::completeTask(int taskIndex)
{
    if (taskIndex < 0 || taskIndex >= (int)tasks_.size()) {
        return;
    }

    tasks_[taskIndex].completed = true;
    updateTaskUI();
}

void UILogic::initTasks() {
    tasks_.push_back(Task("TASK:Plant a seed by yourself!", true));
    tasks_.push_back(Task("TASK:Say hello to Haley in the town!", true));
    tasks_.push_back(Task("TASK:Join a festival with the residents in the town!", true));
    tasks_.push_back(Task("TASK:Catch a fish by the beach!", true));
}
