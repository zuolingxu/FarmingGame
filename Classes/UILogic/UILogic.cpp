#include"UILogic.h"
#include "DocumentManager.h"
#include "SceneManager.h"

USING_NS_CC;

// ��̬ʵ����ʼ��
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
//    : startScreenNode_(nullptr)
//    , bagNode_(nullptr)
//    , taskBarNode_(nullptr)
//    , saveManager_(nullptr)
//    , sceneManager_(nullptr)
//    , mainCharacter_(nullptr)
{
//    // ��ȡ����������ʵ���������Ϊ������
//    saveManager_ = SaveManager::getInstance();
//    sceneManager_ = SceneManager::getInstance();
//    mainCharacter_ = MainCharacter::getInstance();
//
//    // �Ӵ浵���س�ʼ���ݣ������Ҫ��
//    loadDataFromSave();
}

UILogic::~UILogic()
{
    // ����ʱ�������ݣ���ѡ��
    //saveDataToSave();
}

void UILogic::initStartScreenNode(cocos2d::Node* startScreenNode)
{
    startScreenNode_ = startScreenNode;
    bindStartScreenEvents(); // �󶨿�ʼ���水ť�¼�
}

void UILogic::initBagNode(cocos2d::Node* bagNode)
{
    bagNode_ = bagNode;
    bindBagEvents(); // �󶨱���UI�¼�
    refreshBagUI();  // ˢ�±�����ʾ
}

void UILogic::initTaskBarNode(cocos2d::Node* taskBarNode)
{
    taskBarNode_ = taskBarNode;
    bindTaskBarEvents(); // ������ť�¼�
    updateTaskUI();      // ˢ��������ʾ
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

    // �������ӵ���¼�
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

    // �رձ�����ť
    auto closeButton = dynamic_cast<ui::Button*>(bagNode_->getChildByName("CloseButton"));
    if (closeButton)
    {
        closeButton->addTouchEventListener(CC_CALLBACK_2(UILogic::onCloseBagButtonClicked, this));
    }
    // ����ťһ����updateTaskUI�д���������ֻ����updateTaskUI�а󶨼���
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
//        // �л���ͼ������ "Map1"
//        sceneManager_->switchToMap("Map1", "default");
//        // ���ؿ�ʼ����ڵ�
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
//            // ���¼������ݲ�ˢ��UI
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

    // TODO:������Ʒ���ظ�MainCharacter


    // ʹ�ú��������
    item.quantity -= 1;
    if (item.quantity <= 0) {
        bagItems_.erase(bagItems_.begin() + slotIndex);
    }

    // ˢ�±���UI
    refreshBagUI();
}

void UILogic::onTaskItemClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;

    auto button = dynamic_cast<ui::Button*>(sender);
    if (!button) return;

    std::string taskName = button->getName(); // "Task_x"
    int taskIndex = std::stoi(taskName.substr(5));

    // ����Ӧ����ɲſɵ�
    if (!tasks_[taskIndex].completed) {
        return;
    }

    tasks_.erase(tasks_.begin() + taskIndex);
    updateTaskUI();
}

void UILogic::refreshBagUI()
{
    // TODO:��saveManager��ȡ���µı������ݣ����浵�б�������Ʒ��Ϣ��
    //bagItems_ = saveManager_->getBagItems();

    if (!bagNode_) return;

    // ���������ʾ��ͼ�꣬����ֱ��ˢ��
    const int numSlots = 24;
    for (int i = 0; i < numSlots; ++i)
    {
        auto slot = dynamic_cast<ui::Button*>(bagNode_->getChildByName("Slot_" + std::to_string(i)));
        if (!slot) continue;

        slot->removeAllChildren(); // �Ƴ��ɵ�ͼ��������ı�

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
    // TODO:�� saveManager ��ȡ������������
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
    // ����Ƿ�����ͬ��Ʒ�ѵ�
    bool found = false;
    for (auto& bItem : bagItems_) {
        if (bItem.type == item.type && bItem.iconPath == item.iconPath)
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
    //TODO: �Ӵ浵���������ر�������������
    //bagItems_ = saveManager_->getBagItems();
    //tasks_ = saveManager_->getTasks();
    // �����Ҫ����UI������ڳ�ʼ�����������refreshBagUI��updateTaskUI
}

void UILogic::saveDataToSave()
{
    // TODO:����ǰ�������������ݱ��浽�浵
    // saveManager_->setBagItems(bagItems_);
    // saveManager_->setTasks(tasks_);
    // saveManager_->saveArchive();
}
