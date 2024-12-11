#include"UILogic.h"
#include "DocumentManager.h"
#include "SceneManager.h"

USING_NS_CC;

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
    , saveManager_(nullptr)
    , sceneManager_(nullptr)
    , mainCharacter_(nullptr)
{
    // 获取其他管理器实例（假设皆为单例）
    saveManager_ = DocumentManager::getInstance();
    sceneManager_ = SceneManager::getInstance();
    mainCharacter_ = MainCharacter::getInstance();
}

UILogic::~UILogic()
{

}

void UILogic::initStartScreenNode(cocos2d::Node* startScreenNode)
{
    startScreenNode_ = startScreenNode;
    bindStartScreenEvents(); // 绑定开始界面按钮事件
}

void UILogic::initBagNode(cocos2d::Node* bagNode)
{
    bagNode_ = bagNode;
    bindBagEvents(); // 绑定背包UI事件
    refreshBagUI();  // 刷新背包显示
}

void UILogic::initTaskBarNode(cocos2d::Node* taskBarNode)
{
    taskBarNode_ = taskBarNode;
    bindTaskBarEvents(); // 绑定任务按钮事件
    updateTaskUI();      // 刷新任务显示
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

    // 背包格子点击事件
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

    // 关闭背包按钮
    auto closeButton = dynamic_cast<ui::Button*>(bagNode_->getChildByName("CloseButton"));
    if (closeButton)
    {
        closeButton->addTouchEventListener(CC_CALLBACK_2(UILogic::onCloseBagButtonClicked, this));
    }
    // 任务按钮一般在updateTaskUI中创建，这里只需在updateTaskUI中绑定即可
}

void UILogic::onNewButtonClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type)
{
//    if (type != ui::Widget::TouchEventType::ENDED) return;
//
//    CCLOG("New Button Clicked");
//    bool success = saveManager_->createNewSave();
//    if (success)
//    {
//        CCLOG("New save created.");
//        // 切换地图，比如 "Map1"
//        sceneManager_->switchToMap("Map1", "default");
//        // 隐藏开始界面节点
//        startScreenNode_->setVisible(false);
//    }
//    else
//    {
//        CCLOG("Failed to create new save.");
//    }
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

    // 刷新背包UI
    refreshBagUI();
    // TODO:将该物品返回给MainCharacter
}

void UILogic::onTaskItemClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;

    auto button = dynamic_cast<ui::Button*>(sender);
    if (!button) return;

    std::string taskName = button->getName(); // "Task_x"
    int taskIndex = std::stoi(taskName.substr(5));

    // 任务应已完成才可点
    if (!tasks_[taskIndex].completed) {
        return;
    }

    tasks_.erase(tasks_.begin() + taskIndex);
    updateTaskUI();
}

void UILogic::refreshBagUI()
{
    // TODO:从saveManager获取最新的背包数据（若存档中保存了物品信息）
    //bagItems_ = saveManager_->getBagItems();

    if (!bagNode_) return;

    // 清除数量显示和图标，这里直接刷新
    const int numSlots = 24;
    for (int i = 0; i < numSlots; ++i)
    {
        auto slot = dynamic_cast<ui::Button*>(bagNode_->getChildByName("Slot_" + std::to_string(i)));
        if (!slot) continue;

        slot->removeAllChildren(); // 移除旧的图标和数量文本

        if (i < (int)bagItems_.size())
        {
            const auto& item = bagItems_[i];
            if (!item.iconPath.empty())
            {
                auto itemSprite = Sprite::create(item.iconPath);
                if (itemSprite)
                {
                    itemSprite->setPosition(Vec2(slot->getContentSize().width / 2,slot->getContentSize().height / 2));
                    slot->addChild(itemSprite);
                }

                if (item.quantity > 1)
                {
                    auto quantityLabel = ui::Text::create(std::to_string(item.quantity), "Arial", 20);
                    quantityLabel->setTextColor(Color4B::WHITE);
                    quantityLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
                    quantityLabel->setPosition(Vec2(slot->getContentSize().width - 5, 5));
                    slot->addChild(quantityLabel);
                }
            }
        }
    }
}

void UILogic::updateTaskUI()
{
    // TODO:从 saveManager 获取最新任务数据
    //tasks_ = saveManager_->getTasks();

    if (!taskBarNode_) return;

    taskBarNode_->removeAllChildren();

    for (int i = 0; i < (int)tasks_.size(); ++i)
    {
        const Task& task = tasks_[i];
        auto taskButton = ui::Button::create("image/TaskButtonNormal.png", "image/TaskButtonSelected.png");
        taskButton->setPosition(Vec2(100, 100 + i * 50));
        taskButton->setName("Task_" + std::to_string(i));

        auto taskLabel = ui::Text::create(task.description, "Arial", 20);
        taskLabel->setPosition(Vec2(taskButton->getContentSize().width / 2,taskButton->getContentSize().height / 2));
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

void UILogic::addItemToBag(const Item& item)
{
    // 检查是否有相同物品堆叠
    bool found = false;
    for (auto& bItem : bagItems_) {
        if (bItem.name == item.name && bItem.iconPath == item.iconPath)
        {
            bItem.quantity += item.quantity;
            found = true;
            break;
        }
    }

    if (!found) {
        bagItems_.push_back(item);
    }

    refreshBagUI();
}

void UILogic::completeTask(int taskIndex)
{
    if (taskIndex < 0 || taskIndex >= (int)tasks_.size()) {
        return;
    }

    tasks_[taskIndex].completed = true;
    updateTaskUI();
}

void UILogic::loadDataFromSave()
{
    //TODO: 从存档管理器加载背包和任务数据
    //bagItems_ = saveManager_->getBagItems();
    //tasks_ = saveManager_->getTasks();
    // 如果需要更新UI则可以在初始化结束后调用refreshBagUI和updateTaskUI
}

void UILogic::saveDataToSave()
{
    // TODO:将当前背包和任务数据保存到存档
    // saveManager_->setBagItems(bagItems_);
    // saveManager_->setTasks(tasks_);
    // saveManager_->saveArchive();
}
