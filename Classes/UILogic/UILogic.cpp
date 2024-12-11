#include <format>
#include "DocumentManager.h"
#include "SceneManager.h"
#include "json/document.h"
#include "UILogic.h"

USING_NS_CC;
#ifdef _MSC_VER
#undef GetObject
#endif

// ��̬ʵ����ʼ��
UILogic* UILogic::instance_ = nullptr;

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
    , mainCharacter_(nullptr)
{
    // ��ȡ����������ʵ��
    saveManager_ = DocumentManager::getInstance();
    mainCharacter_ = MainCharacter::getInstance();
    initTasks();
}

UILogic::~UILogic()
{

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

    // ˢ�±���UI
    refreshBagUI();

    // TODO:������Ʒ���ظ�MainCharacter
    mainCharacter_->setCurrentItem(item.type);
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

void UILogic::updateBagItems(std::vector<Item> bagitem) {
    bagItems_ = bagitem;
}

void UILogic::refreshBagUI()
{

    if (!bagNode_) return;

    // ���������ʾ��ͼ�꣬����ֱ��ˢ��
    const int numSlots = 24;
    const int columns = 12; // ÿ��12��
    const int rows = 2; // 2��
    const float slotSize = 20.0f; // ÿ�����ӵĴ�С

    for (int i = 0; i < numSlots; ++i)
    {
        int row = i / columns;
        int col = i % columns;

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
    // TODO:�� saveManager ��ȡ������������
    //tasks_ = saveManager_->getTasks();

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
    tasks_.push_back(Task("TASK:Plant a seed by yourself!",true));
    tasks_.push_back(Task("TASK:Say hello to Haley in the town!",true));
    tasks_.push_back(Task("TASK:Join a festival with the residents in the town!",true));
    tasks_.push_back(Task("TASK:Catch a fish by the beach!",true));
}


