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
            //case UILayerType::TOOL_BAR:
                //return ret->toolBarLayout_;
                //break;
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
    if (!Layer::init())
    {
        return false;
    }

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
        //case UILayerType::TOOL_BAR:
            //createToolBarLayout();
            //break;
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
    //taskBarLayout_->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    //taskBarLayout_->setBackGroundColor(Color3B(50, 50, 50)); // 深灰色背景
    //this->addChild(taskBarLayout_, 4);

    // 添加任务按钮
    auto taskButton = ui::Button::create("image/TitleButtons-0-7.png", "image/TitleButtons-0-3.png");
    taskButton->setPosition(Vec2(420, 300));
    taskBarLayout_->addChild(taskButton);
}

void UILayer::createBagLayout()
{
    bagLayout_ = ui::Layout::create();
    bagLayout_->setContentSize(Director::getInstance()->getWinSize()); // 设置背包界面的高度为100像素
    bagLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    bagLayout_->setPosition(Vec2(0,0)); 
    //bagLayout_->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    //bagLayout_->setBackGroundColor(Color3B(80, 80, 80)); // 中灰色背景
    //this->addChild(bagLayout_, 4);

    // 添加背包格子
    const int numSlots = 24; // 假设有12个格子
    const int columns = 12; // 每行12个
    const int rows = 2; // 2行
    const float slotSize = 32.0f; // 每个格子的大小
    const float padding = 0.0f; // 间距

    for (int i = 0; i < numSlots; ++i)
    {
        int row = i / columns;
        int col = i % columns;

        auto slot = ui::Button::create("image/frame-0.png", "image/frame-1.png");
        slot->setScale9Enabled(true);
        slot->setContentSize(Size(slotSize, slotSize));
        float x = padding + col * (slotSize + padding) + slotSize / 2;
        float y = padding + row * (slotSize + padding) + slotSize / 2;
        slot->setPosition(Vec2(x, y));

        bagLayout_->addChild(slot);
    }

    // 添加关闭背包按钮
    auto closeBagButton = ui::Button::create("image/exit.png", "image/exit.png");
    closeBagButton->setPosition(Vec2(420,420));
    bagLayout_->addChild(closeBagButton);
}

void UILayer::createStartScreenLayout()
{
    startScreenLayout_ = ui::Layout::create();
    startScreenLayout_->setContentSize(Director::getInstance()->getWinSize());
    startScreenLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    startScreenLayout_->setPosition(Vec2(0, 0));
    this->addChild(startScreenLayout_, 3);

    // 添加开始按钮
    auto startButton = ui::Button::create("image/TitleButtons-0-4.png", "image/TitleButtons-0-0.png");
    startButton->setPosition(Vec2(100,64));
    startScreenLayout_->addChild(startButton);

    // 添加加载按钮
    auto loadButton = ui::Button::create("image/TitleButtons-0-5.png", "image/TitleButtons-0-1.png");
    loadButton->setPosition(Vec2(228, 64));
    startScreenLayout_->addChild(loadButton);

    // 添加退出按钮
    auto exitButton = ui::Button::create("image/TitleButtons-0-7.png", "image/TitleButtons-0-3.png");
    exitButton->setPosition(Vec2(356, 64));
    startScreenLayout_->addChild(exitButton);
}

void UILayer::createToolBarLayout()
{
    toolBarLayout_ = ui::Layout::create();
    toolBarLayout_->setContentSize(Director::getInstance()->getWinSize()); // 设置背包界面的高度为100像素
    toolBarLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    toolBarLayout_->setPosition(Vec2(0, 0));
    //bagLayout_->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    //bagLayout_->setBackGroundColor(Color3B(80, 80, 80)); // 中灰色背景
    //this->addChild(bagLayout_, 4);

    // 添加背包格子
    const int numSlots = 12; // 假设有12个格子
    const float slotSize = 32.0f; // 每个格子的大小
    const float padding = 0.0f; // 间距

    for (int i = 0; i < numSlots; ++i)
    {

        auto slot = ui::Button::create("image/frame-0.png", "image/frame-1.png");
        slot->setScale9Enabled(true);
        slot->setContentSize(Size(slotSize, slotSize));
        float x = padding + i * (slotSize + padding) + slotSize / 2;
        slot->setPosition(Vec2(x, 0));

        bagLayout_->addChild(slot);
    }
}

void UILayer::showStartScreen()
{
    startScreenLayout_->setVisible(true);
}

void UILayer::hideStartScreen()
{
    startScreenLayout_->setVisible(false);
}

void UILayer::refreshBagItems(const std::vector<Item>& items)
{
    // 遍历所有背包格子并更新物品显示
    const int numSlots = 12;
    for (int i = 0; i < numSlots; ++i)
    {
        auto slot = dynamic_cast<ui::Button*>(bagLayout_->getChildByName("Slot_" + std::to_string(i)));
        if (slot)
        {
            // 清除之前的子节点（物品图标）
            slot->removeAllChildren();

            if (i < items.size())
            {
                const Item& item = items[i];
                if (!item.iconPath.empty())
                {
                    auto itemSprite = Sprite::create(item.iconPath);
                    if (itemSprite)
                    {
                        itemSprite->setPosition(Vec2(slot->getContentSize().width / 2, slot->getContentSize().height / 2));
                        itemSprite->setName("ItemIcon");
                        slot->addChild(itemSprite);
                    }

                    // 添加物品数量显示
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
}