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
{
    // 获取其他管理器实例
    initTasks();
    bagItems_ = new std::vector<Item>();
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
    refreshBagUI();
    bindBagEvents(); 
}

void UILogic::initTaskBarNode(cocos2d::Node* taskBarNode)
{
    taskBarNode_ = taskBarNode;
    bindTaskBarEvents(); 
    updateTaskUI();      
}

void UILogic::initNpcNode(cocos2d::Node* npcNode)
{
    npcNode_ = npcNode;
    bindNpcEvents();
}

void UILogic::initLoadArchiveNode(cocos2d::Node* loadArchiveNode)
{
    loadArchiveNode_ = loadArchiveNode;
    refreshArchiveUI();
}

void UILogic::initShopNode(cocos2d::Node* shopNode)
{
    shopNode_ = shopNode;
    bindShopEvents();
}

void UILogic::initTimeNode(cocos2d::Node* timeNode)
{
    timeNode_ = timeNode;
}

void UILogic::initManufactureNode(cocos2d::Node* manufactureNode)
{
    manufactureNode_ = manufactureNode;
    bindManufactureEvents();
}

void UILogic::initPopupNode(cocos2d::Node* popupNode)
{
    popupNode_ = popupNode;
    bindPopupEvents();
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

    auto closeButton = dynamic_cast<ui::Button*>(taskBarNode_->getChildByName("CloseButton"));
    if (closeButton) {
        closeButton->addTouchEventListener(CC_CALLBACK_2(UILogic::onCloseButtonClicked, this));
    }
}

void UILogic::bindNpcEvents()
{
    if (!npcNode_) return;

    auto closeButton = dynamic_cast<ui::Button*>(npcNode_->getChildByName("CloseButton"));
    if (closeButton)
    {
        closeButton->addTouchEventListener(CC_CALLBACK_2(UILogic::onCloseButtonClicked, this));
    }
}

void UILogic::bindLoadArchiveEvents()
{
    if (!loadArchiveNode_) return;

    for (int i = 0; i < UI_num; ++i)
    {
        auto archive = dynamic_cast<ui::Button*>(loadArchiveNode_->getChildByName("Save_" + std::to_string(i+1)));
        if (archive) {
            archive->addTouchEventListener(CC_CALLBACK_2(UILogic::onArchiveClicked, this));
        }
    }
}

void UILogic::bindShopEvents()
{
    if (!shopNode_) return;

    auto cauliflower = dynamic_cast<ui::Button*>(shopNode_->getChildByName("cauliflower"));
    if (cauliflower)
    {
        cauliflower->addTouchEventListener(CC_CALLBACK_2(UILogic::onCauliflowerClicked, this));
    }
    auto potato = dynamic_cast<ui::Button*>(shopNode_->getChildByName("potato"));
    if (potato)
    {
        potato->addTouchEventListener(CC_CALLBACK_2(UILogic::onPotatoClicked, this));
    }
    auto pumpkin = dynamic_cast<ui::Button*>(shopNode_->getChildByName("pumpkin"));
    if (pumpkin)
    {
        pumpkin->addTouchEventListener(CC_CALLBACK_2(UILogic::onPumpkinClicked, this));
    }
    auto sell_cauliflower = dynamic_cast<ui::Button*>(shopNode_->getChildByName("sell_cauliflower"));
    if (sell_cauliflower)
    {
        sell_cauliflower->addTouchEventListener(CC_CALLBACK_2(UILogic::onSellCauliflowerClicked, this));
    }
    auto sell_potato = dynamic_cast<ui::Button*>(shopNode_->getChildByName("sell_potato"));
    if (sell_potato)
    {
        sell_potato->addTouchEventListener(CC_CALLBACK_2(UILogic::onSellPotatoClicked, this));
    }
    auto sell_pumpkin = dynamic_cast<ui::Button*>(shopNode_->getChildByName("sell_pumpkin"));
    if (sell_pumpkin)
    {
        sell_pumpkin->addTouchEventListener(CC_CALLBACK_2(UILogic::onSellPumpkinClicked, this));
    }
}

void UILogic::bindManufactureEvents()
{
    if (!manufactureNode_) return;

    auto box = dynamic_cast<ui::Button*>(manufactureNode_->getChildByName("box"));
    auto fertilizer = dynamic_cast<ui::Button*>(box->getChildByName("fertilizer"));
    if (fertilizer)
    {
        fertilizer->addTouchEventListener(CC_CALLBACK_2(UILogic::onFertilizerClicked, this));
    }

    auto soup = dynamic_cast<ui::Button*>(box->getChildByName("soup"));
    if (soup)
    {
        soup->addTouchEventListener(CC_CALLBACK_2(UILogic::onSoupClicked, this));
    }
}

void UILogic::bindPopupEvents()
{
    if (!popupNode_) return;

    auto sell = dynamic_cast<ui::Button*>(manufactureNode_->getChildByName("choosebox1"));
    if (sell)
    {
        sell->addTouchEventListener(CC_CALLBACK_2(UILogic::onSellButtonClicked, this));
    }

    auto feed = dynamic_cast<ui::Button*>(manufactureNode_->getChildByName("choosebox2"));
    if (feed)
    {
        feed->addTouchEventListener(CC_CALLBACK_2(UILogic::onFeedButtonClicked, this));
    }
}

void UILogic::onNewButtonClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type)
{
    int i = 1;
    while (!DocumentManager::getInstance()->createArchiveDocument(i)) { i++; }
    SceneManager::getInstance()->NextMap("player_house", "14 2");
    refreshArchiveUI();
    updateBagItems(MainCharacter::getInstance()->getInventory());
}

void UILogic::onLoadButtonClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type)
{
    SceneManager::getInstance()->showUILayer("loadarchive");

}

void UILogic::onExitButtonClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type)
{

    Director::getInstance()->end();

}

void UILogic::onCloseButtonClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;

    if (bagNode_)
    {
        bagNode_->setVisible(false);
    }

    if (npcNode_) {
        npcNode_->setVisible(false);
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

void UILogic::onArchiveClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) return;

    auto button = dynamic_cast<ui::Button*>(sender);
    if (!button) return;

    std::string name = button->getName(); // "Save_x"
    int saveIndex = std::stoi(name.substr(5));

    LoadArchive(saveIndex);
}

void UILogic::onCauliflowerClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) return;

    auto button = dynamic_cast<ui::Button*>(sender);
    if (!button) return;

    if (MainCharacter::getInstance()->modifyMoney(-50)) {
        MainCharacter::getInstance()->modifyItemQuantity(ItemType::CAULIFLOWER_SEED, 1);
    }
}

void UILogic::onPotatoClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) return;

    auto button = dynamic_cast<ui::Button*>(sender);
    if (!button) return;
    if (MainCharacter::getInstance()->modifyMoney(-30)) {
        MainCharacter::getInstance()->modifyItemQuantity(ItemType::POTATO_SEED, 1);
    }
}

void UILogic::onPumpkinClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) return;

    auto button = dynamic_cast<ui::Button*>(sender);
    if (!button) return;

    if (MainCharacter::getInstance()->modifyMoney(-80)) {
        MainCharacter::getInstance()->modifyItemQuantity(ItemType::PUMPKIN_SEED, 1);
    }
}

void UILogic::onSellCauliflowerClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) return;

    auto button = dynamic_cast<ui::Button*>(sender);
    if (!button) return;

    if (MainCharacter::getInstance()->modifyItemQuantity(ItemType::CAULIFLOWER, -1)) {
        MainCharacter::getInstance()->modifyMoney(100);
    }
    else {
        return;
    }
}

void UILogic::onSellPotatoClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) return;

    auto button = dynamic_cast<ui::Button*>(sender);
    if (!button) return;

    if (MainCharacter::getInstance()->modifyItemQuantity(ItemType::POTATO, -1)) {
        MainCharacter::getInstance()->modifyMoney(60);
    }
    else {
        return;
    }
}

void UILogic::onSellPumpkinClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) return;

    auto button = dynamic_cast<ui::Button*>(sender);
    if (!button) return;

    if (MainCharacter::getInstance()->modifyItemQuantity(ItemType::PUMPKIN, -1)) {
        MainCharacter::getInstance()->modifyMoney(160);
    }
    else {
        return;
    }
}

void UILogic::onFertilizerClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) return;

    auto button = dynamic_cast<ui::Button*>(sender);
    if (!button) return;
    if (MainCharacter::getInstance()->modifyItemQuantity(ItemType::ROCK, -2)) {
        MainCharacter::getInstance()->modifyItemQuantity(ItemType::FERTILIZER, 1);
    }
    else {
        return;
    }
}

void UILogic::onSoupClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) return;

    auto button = dynamic_cast<ui::Button*>(sender);
    if (!button) return;

    if (MainCharacter::getInstance()->modifyItemQuantity(ItemType::CAULIFLOWER, -2)) {
        MainCharacter::getInstance()->modifyItemQuantity(ItemType::SOUP, 1);
    }
    else {
        return;
    }
}

void UILogic::onSellButtonClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) return;

    auto button = dynamic_cast<ui::Button*>(sender);
    if (!button) return;

    MainCharacter::getInstance()->modifyMoney(100);

}

void UILogic::onFeedButtonClicked(cocos2d::Ref* sender, ui::Widget::TouchEventType type) {

}

void UILogic::useItemFromBag(int slotIndex)
{

    auto& item = (*bagItems_)[slotIndex];

    // TODO:将该物品返回给MainCharacter
    MainCharacter::getInstance()->setCurrentItem(item.type);
    for (int i = 0; i < 24; ++i) {
        auto slot = dynamic_cast<ui::Button*>(bagNode_->getChildByName("Slot_" + std::to_string(i)));
        auto selected = slot->getChildByName("selected");
        if (selected) {
            slot->removeChildByName("selected");
        }
    }

    auto slot = bagNode_->getChildByName("Slot_" + std::to_string(slotIndex));
    auto selected = ui::Button::create("image/selected_tile.png", "image/selected_tile.png");
    selected->setScale9Enabled(true);
    selected->setContentSize(Size(20, 20));
    selected->setPosition(Vec2(10, 10));
    selected->setName("selected");
    slot->addChild(selected);
}

void UILogic::LoadArchive(int saveIndex) {
    DocumentManager::getInstance()->loadArchiveDocument(saveIndex);
    MainCharacter::getInstance();
    SceneManager::getInstance() -> NextMap("player_house", "14 2");
    refreshArchiveUI();
    bindLoadArchiveEvents();
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

void UILogic::updateBagItems(std::vector<Item>* bagitem) {
    bagItems_ = bagitem;

    for (int i = 0; i < 24; i++) {
        auto slot = dynamic_cast<ui::Button*>(bagNode_->getChildByName("Slot_" + std::to_string(i)));
        slot->setEnabled(true);
    }

    for (size_t i = (*bagItems_).size(); i < 24; i++) {
        auto slot = dynamic_cast<ui::Button*>(bagNode_->getChildByName("Slot_" + std::to_string(i)));
        slot->setEnabled(false);
    }
    refreshBagUI();
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

        if (i < (int)(*bagItems_).size())
        {
            const auto& item = (*bagItems_)[i];
            if (!item.iconPath.empty())
            {
                auto itemSprite = Sprite::create(item.iconPath);
                if (itemSprite)
                {
                    itemSprite->setPosition(Vec2( slotSize / 2 ,  slotSize / 2 ));
                    slot->addChild(itemSprite);
                }

                if (item.quantity > 1)
                {
                    auto quantityLabel = ui::Text::create(std::to_string(item.quantity), "fonts/arial.ttf", 8);
                    quantityLabel->setColor(Color3B(0,0,0));
                    quantityLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
                    quantityLabel->setPosition(Vec2( slotSize / 2 + 8,  0));
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
        taskButton->setPosition(Vec2(240, 250 - i * 50));
        taskButton->setName("Task_" + std::to_string(i));

        auto taskLabel = ui::Text::create(task.description, "fonts/Marker Felt.ttf", 10);
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

void UILogic::refreshArchiveUI() {
    if (!loadArchiveNode_) return;

    loadArchiveNode_->removeAllChildren();

    rapidjson::Value& val = (*DocumentManager::getInstance()->getConfigDocument())["Archive"];


    UI_num = val.GetObject().MemberCount();
    for (int i = 1 ; i <= UI_num; i++)
    {
        auto& save = val[("Save_" + std::to_string(i)).c_str()];
        auto archiveButton = ui::Button::create("image/textBox..png", "image/textBox..png");
        archiveButton->setScale9Enabled(true);
        archiveButton->setContentSize(Size(360, 50));
        float y = 300 - i * 50;
        archiveButton->setPosition(Vec2(240, y));
        archiveButton->setName("Save_" + std::to_string(i));

        auto archiveLabel = ui::Text::create("Save_" + std::to_string(i) + "\n" + "Day:"+std::to_string(save["day"].GetInt())+
            "  Money:"+std::to_string(save["money"].GetInt()), "fonts/Marker Felt.ttf", 10);
        archiveLabel->setPosition(Vec2(190, 25));
        archiveButton->addChild(archiveLabel);

        loadArchiveNode_->addChild(archiveButton);
    }
    bindLoadArchiveEvents();
}

void UILogic::refreshTimeUI(int day_,float hour_) {
    if (!timeNode_) return;

    auto displayer= dynamic_cast<ui::Button*>(timeNode_->getChildByName("displayer"));
    displayer->removeChild(displayer->getChildByName("day"));
    displayer->removeChild(displayer->getChildByName("hour"));

    auto day = ui::Text::create("Day: "+std::to_string(day_), "fonts/Marker Felt.ttf", 10);
    day->setPosition(Vec2(48, 35));
    day->setColor(Color3B(93, 59, 23));
    day->setName("day");
    displayer->addChild(day);

    int Hour = static_cast<int>(hour_);
    auto hour = ui::Text::create(std::to_string(Hour)+":00", "fonts/Marker Felt.ttf", 10);
    hour->setPosition(Vec2(48, 25));
    hour->setColor(Color3B(93, 59, 23));
    hour->setName("hour");
    displayer->addChild(hour);
}

void UILogic::refreshPowerUI(int power_) {
    if (!timeNode_) return;

    auto displayer = dynamic_cast<ui::Button*>(timeNode_->getChildByName("displayer"));
    displayer->removeChild(displayer->getChildByName("power"));

    auto power = ui::Text::create("Power:"+std::to_string(power_), "fonts/Marker Felt.ttf", 10);
    power->setPosition(Vec2(53, 65));
    power->setColor(Color3B(93, 59, 23));
    power->setName("power");
    displayer->addChild(power);
}

void UILogic::refreshMoneyUI(int money_) {
    if (!timeNode_) return;

    auto displayer = dynamic_cast<ui::Button*>(timeNode_->getChildByName("displayer"));
    displayer->removeChild(displayer->getChildByName("money"));

    auto money = ui::Text::create("Money:" + std::to_string(money_), "fonts/Marker Felt.ttf", 10);
    money->setPosition(Vec2(45, 7));
    money->setColor(Color3B(93, 59, 23));
    money->setName("money");
    displayer->addChild(money);
}


void UILogic::refreshNpcUI(std::string name) {
    if (!npcNode_) return;

    if (!(name == "abigail" || name == "caroline" || name == "haley")) {
        return;
    }
    npcNode_->removeChild(npcNode_->getChildByName("potrait"));

    if (name == "abigail") {
        auto potrait = ui::Button::create("image/abi.png", "image/abi.png");
        potrait->setPosition(Vec2(30, 120));
        potrait->setScale9Enabled(true);
        potrait->setContentSize(Size(40, 40));
        potrait->setName("portrait");
        npcNode_->addChild(potrait);
    }
    else if (name == "caroline") {
        auto potrait = ui::Button::create("image/caro.png", "image/caro.png");
        potrait->setPosition(Vec2(30, 120));
        potrait->setScale9Enabled(true);
        potrait->setContentSize(Size(40, 40));
        potrait->setName("portrait");
        npcNode_->addChild(potrait);
    }
    else {
        auto potrait = ui::Button::create("image/hal.png", "image/hal.png");
        potrait->setPosition(Vec2(30, 120));
        potrait->setScale9Enabled(true);
        potrait->setContentSize(Size(40, 40));
        potrait->setName("portrait");
        npcNode_->addChild(potrait);
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
    tasks_.push_back(Task("TASK:Plant a seed by yourself!", false));
    tasks_.push_back(Task("TASK:Say hello to Haley in the town!", false));
    tasks_.push_back(Task("TASK:Join a festival with the residents in the town!", false));
    tasks_.push_back(Task("TASK:Catch a fish by the beach!", false));
}
