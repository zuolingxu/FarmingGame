#include "UILayer.h"

USING_NS_CC;

Node* UILayer::createUILayer(UILayerType type)
{
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
            break;
        case UILayerType::BAG:
            createBagLayout();
            break;
        case UILayerType::TASK_BAR:
            createTaskBarLayout();
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
    //taskBarLayout_->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    //taskBarLayout_->setBackGroundColor(Color3B(50, 50, 50)); // 深灰色背景
    //this->addChild(taskBarLayout_, 4);

    // 添加关闭按钮
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
    //bagLayout_->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    //bagLayout_->setBackGroundColor(Color3B(80, 80, 80)); // 中灰色背景
    //this->addChild(bagLayout_, 4);

    // 添加背包格子
    const int numSlots = 24; // 假设有12个格子
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
    //this->addChild(startScreenLayout_, 3);

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