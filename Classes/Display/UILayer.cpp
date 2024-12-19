#include "DocumentManager.h"
#include "UILayer.h"
#include "UILogic.h"

USING_NS_CC;
#ifdef _MSC_VER
#undef GetObject
#endif

static UILogic* uilogic = nullptr;
Node* UILayer::createUILayer(UILayerType type)
{
    if (uilogic == nullptr) {
        uilogic = UILogic::getInstance();
    }

    UILayer* ret = new UILayer();
    if (ret && ret->initWithType(type))
    {
        ret->autorelease();
        switch (type)
        {
            case UILayerType::START_SCREEN:
                return ret->startScreenLayout_;
                break;
            case UILayerType::BAG:
                return ret->bagLayout_;
                break;
            case UILayerType::TASK_BAR:
                return ret->taskBarLayout_;
                break;
            case UILayerType::NPC:
                return ret->npcLayout_;
                break;
            case UILayerType::LOAD_ARCHIVE:
                return ret->loadArchiveLayout_;
                break;
            default:
                break;
        }
    }
    else
    {
        delete ret;
        return nullptr;
    }
}

bool UILayer::initWithType(UILayerType type)
{
    switch (type)
    {
        case UILayerType::START_SCREEN:
            createStartScreenLayout();
            uilogic->initStartScreenNode(startScreenLayout_);
            break;
        case UILayerType::BAG:
            createBagLayout();
            uilogic->initBagNode(bagLayout_);
            break;
        case UILayerType::TASK_BAR:
            createTaskBarLayout();
            uilogic->initTaskBarNode(taskBarLayout_);
            break;
        case UILayerType::NPC:
            createNpcLayout();
            uilogic->initNpcNode(npcLayout_);
            break;
        case UILayerType::LOAD_ARCHIVE:
            createLoadArchiveLayout();
            uilogic->initLoadArchiveNode(loadArchiveLayout_);
            break;
        default:
            break;
    }

    return true;
}

void UILayer::createTaskBarLayout()
{
    taskBarLayout_ = ui::Layout::create();
    taskBarLayout_->setContentSize(Director::getInstance()->getWinSize());
    taskBarLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    taskBarLayout_->setPosition(Vec2(0,0));
    taskBarLayout_->setName("taskbar");

    // 添加关 闭按钮
    auto closeButton = ui::Button::create("image/exit.png", "image/exit.png");
    closeButton->setPosition(Vec2(420, 300));
    closeButton->setName("CloseButton");
    taskBarLayout_->addChild(closeButton);

}

void UILayer::createBagLayout()
{
    bagLayout_ = ui::Layout::create();
    bagLayout_->setContentSize(Director::getInstance()->getWinSize());
    bagLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    bagLayout_->setPosition(Vec2(0,0)); 
    bagLayout_->setName("bag");

    // 添加背包格子
    const int numSlots = 24; // 有24个格子
    const int columns = 12; // 每行12个
    const int rows = 2; // 2行
    const float slotSize = 20.0f; // 每个格子的大小

    for (int i = 0; i < numSlots; ++i)
    {
        int row = i / columns;
        int col = i % columns;

        auto slot = ui::Button::create("image/frame-0.png", "image/frame-1.png");
        slot->setScale9Enabled(true);
        slot->setContentSize(Size(slotSize, slotSize));
        float x =  col * slotSize  + slotSize / 2 + 120;
        float y =  row * slotSize  + slotSize / 2  ;
        slot->setPosition(Vec2(x, y));
        slot->setName("Slot_" + std::to_string(i));

        bagLayout_->addChild(slot);
    }

}

void UILayer::createStartScreenLayout()
{
    startScreenLayout_ = ui::Layout::create();
    startScreenLayout_->setContentSize(Director::getInstance()->getWinSize());
    startScreenLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    startScreenLayout_->setPosition(Vec2(0, 0));
    startScreenLayout_->setName("startscreen");

    // 添加开始按钮
    auto startButton = ui::Button::create("image/TitleButtons-0-4.png", "image/TitleButtons-0-0.png");
    startButton->setPosition(Vec2(100,64));
    startButton->setName("NewButton");
    startScreenLayout_->addChild(startButton);

    // 添加加载按钮
    auto loadButton = ui::Button::create("image/TitleButtons-0-5.png", "image/TitleButtons-0-1.png");
    loadButton->setPosition(Vec2(228, 64));
    loadButton->setName("LoadButton");
    startScreenLayout_->addChild(loadButton);

    // 添加退出按钮
    auto exitButton = ui::Button::create("image/TitleButtons-0-7.png", "image/TitleButtons-0-3.png");
    exitButton->setPosition(Vec2(356, 64));
    exitButton->setName("ExitButton");
    startScreenLayout_->addChild(exitButton);
}

void UILayer::createNpcLayout()
{
    npcLayout_ = ui::Layout::create();
    npcLayout_->setContentSize(Director::getInstance()->getWinSize());
    npcLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    npcLayout_->setPosition(Vec2(0, 0));
    npcLayout_->setName("npc");

    auto textbox = ui::Button::create("image/textBox..png", "image/textBox..png");
    textbox->setPosition(Vec2(240, 50));
    textbox->setScale9Enabled(true);
    textbox->setContentSize(Size(400,100));
    textbox->setName("textbox");
    static int count = 0;
    int i = count % 5;
    i++;
    auto sentence = ui::Text::create(Sentence[i], "Arial", 10);
    sentence->setPosition(Vec2(180, 25));
    textbox->addChild(sentence);
    taskBarLayout_->addChild(textbox);
}

void UILayer::createLoadArchiveLayout()
{
    loadArchiveLayout_ = ui::Layout::create();
    loadArchiveLayout_->setContentSize(Director::getInstance()->getWinSize());
    loadArchiveLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    loadArchiveLayout_->setPosition(Vec2(0, 0));
    loadArchiveLayout_->setName("loadarchive");

}

void UILayer::initSentence() {
    Sentence.push_back("Hello, nice to see you!");
    Sentence.push_back("Have you heard that Zhu Hongming is the best teacher in Tongji University?");
    Sentence.push_back("How's your day? I feel great today");
    Sentence.push_back("It is said that Zuo lingxu is the god of coding.");
    Sentence.push_back("Thank you,I like it very much!");
}