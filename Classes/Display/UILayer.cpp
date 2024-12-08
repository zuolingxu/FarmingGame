//#include "UILayer.h"
//
//USING_NS_CC;
//
//UILayer* UILayer::createUILayer()
//{
//    UILayer* ret = new (std::nothrow) UILayer();
//    if (ret && ret->init())
//    {
//        ret->autorelease();
//        return ret;
//    }
//    else
//    {
//        delete ret;
//        return nullptr;
//    }
//}
//
//bool UILayer::init()
//{
//    if (!Layer::init())
//    {
//        return false;
//    }
//
//    // 创建 UI 组件
//    createTaskBarLayout();
//    createBagLayout();
//    createStartScreenLayout();
//    //createToolBarLayout(); // 创建工具栏
//
//
//    return true;
//}
//
//void UILayer::createTaskBarLayout()
//{
//    taskBarLayout_ = ui::Layout::create();
//    taskBarLayout_->setContentSize(Size(320, 160));
//    taskBarLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
//    taskBarLayout_->setPosition(Vec2((Director::getInstance()->getVisibleSize().width - taskBarLayout_->getContentSize().width) / 2, (Director::getInstance()->getVisibleSize().height - taskBarLayout_->getContentSize().height) / 2));
//    taskBarLayout_->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
//    taskBarLayout_->setBackGroundColor(Color3B(50, 50, 50)); // 深灰色背景
//    this->addChild(taskBarLayout_, 4);
//
//    // 添加任务按钮
//    auto taskButton = ui::Button::create("image/TitleButton-0-7.png", "image/TitleButton-0-3.png");
//    taskButton->setPosition(Vec2(420, 420));
//    taskButton->setName("task");
//    taskBarLayout_->addChild(taskButton);
//}
//
//void UILayer::createBagLayout()
//{
//    bagLayout_ = ui::Layout::create();
//    bagLayout_->setContentSize(Size(400, 100)); // 设置背包界面的高度为100像素
//    bagLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
//    bagLayout_->setPosition(Vec2(0, -100)); // 初始时隐藏在屏幕下方
//    bagLayout_->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
//    bagLayout_->setBackGroundColor(Color3B(80, 80, 80)); // 中灰色背景
//    this->addChild(bagLayout_, 4);
//
//    // 添加背包格子
//    const int numSlots = 12; // 假设有12个格子
//    const int columns = 12; // 每行12个
//    const int rows = 2; // 2行
//    const float slotSize = 16.0f; // 每个格子的大小
//    const float padding = 1.0f; // 间距
//
//    for (int i = 0; i < numSlots; ++i)
//    {
//        int row = i / columns;
//        int col = i % columns;
//
//        auto slot = ui::Button::create("image/TitleButton-0-4.png", "image/TitleButton-0-4.png");
//        slot->setScale9Enabled(true);
//        slot->setContentSize(Size(slotSize, slotSize));
//        float x = padding + col * (slotSize + padding) + slotSize / 2;
//        float y = padding + row * (slotSize + padding) + slotSize / 2;
//        slot->setPosition(Vec2(x, y));
//        slot->setName("Slot_" + std::to_string(i));
//
//        bagLayout_->addChild(slot);
//    }
//
//    // 添加关闭背包按钮
//    auto closeBagButton = ui::Button::create("image/TitleButton-0-4.png", "image/TitleButton-0-0.png");
//    closeBagButton->setPosition(Vec2(bagLayout_->getContentSize().width - 30, bagLayout_->getContentSize().height - 30));
//    closeBagButton->setName("CloseBagButton");
//    bagLayout_->addChild(closeBagButton);
//}
//void UILayer::createStartScreenLayout()
//{
//    startScreenLayout_ = ui::Layout::create();
//    startScreenLayout_->setContentSize(Director::getInstance()->getVisibleSize());
//    startScreenLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
//    startScreenLayout_->setPosition(Vec2(0, 0));
//    this->addChild(startScreenLayout_, 3);
//
//    // 添加开始按钮
//    auto startButton = ui::Button::create("image/TitleButton-0-4.png", "image/TitleButton-0-0.png");
//    startButton->setPosition(Vec2(64, 32));
//    startButton->setName("new");
//    startScreenLayout_->addChild(startButton);
//
//    // 添加加载按钮
//    auto loadButton = ui::Button::create("image/TitleButton-0-5.png", "image/TitleButton-0-1.png");
//    loadButton->setPosition(Vec2(192, 32));
//    loadButton->setName("load");
//    startScreenLayout_->addChild(startButton);
//
//    // 添加退出按钮
//    auto exitButton = ui::Button::create("image/TitleButton-0-7.png", "image/TitleButton-0-3.png");
//    exitButton->setPosition(Vec2(320, 32));
//    exitButton->setName("exit");
//    startScreenLayout_->addChild(startButton);
//}
//
//void UILayer::showBag()
//{
//    // 使用动作将背包滑入视野（从底部向上滑动）
//    auto moveIn = MoveBy::create(0.3f, Vec2(0, 100));
//    bagLayout_->runAction(moveIn);
//}
//
//void UILayer::hideBag()
//{
//    // 使用动作将背包滑出视野（向下滑动）
//    auto moveOut = MoveBy::create(0.3f, Vec2(0, -100));
//    bagLayout_->runAction(moveOut);
//}
//
//void UILayer::showStartScreen()
//{
//    startScreenLayout_->setVisible(true);
//}
//
//void UILayer::hideStartScreen()
//{
//    startScreenLayout_->setVisible(false);
//}
//
//void UILayer::refreshBagItems(const std::vector<Item>& items)
//{
//    // 遍历所有背包格子并更新物品显示
//    const int numSlots = 12;
//    for (int i = 0; i < numSlots; ++i)
//    {
//        auto slot = dynamic_cast<ui::Button*>(bagLayout_->getChildByName("Slot_" + std::to_string(i)));
//        if (slot)
//        {
//            // 清除之前的子节点（物品图标）
//            slot->removeAllChildren();
//
//            if (i < items.size())
//            {
//                const Item& item = items[i];
//                if (!item.iconPath.empty())
//                {
//                    auto itemSprite = Sprite::create(item.iconPath);
//                    if (itemSprite)
//                    {
//                        itemSprite->setPosition(Vec2(slot->getContentSize().width / 2, slot->getContentSize().height / 2));
//                        itemSprite->setName("ItemIcon");
//                        slot->addChild(itemSprite);
//                    }
//
//                    // 添加物品数量显示
//                    if (item.quantity > 1)
//                    {
//                        auto quantityLabel = ui::Text::create(std::to_string(item.quantity), "Arial", 20);
//                        quantityLabel->setTextColor(Color4B::WHITE);
//                        quantityLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
//                        quantityLabel->setPosition(Vec2(slot->getContentSize().width - 5, 5));
//                        slot->addChild(quantityLabel);
//                    }
//                }
//            }
//        }
//    }
//}
