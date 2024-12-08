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
//    // ���� UI ���
//    createTaskBarLayout();
//    createBagLayout();
//    createStartScreenLayout();
//    //createToolBarLayout(); // ����������
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
//    taskBarLayout_->setBackGroundColor(Color3B(50, 50, 50)); // ���ɫ����
//    this->addChild(taskBarLayout_, 4);
//
//    // �������ť
//    auto taskButton = ui::Button::create("image/TitleButton-0-7.png", "image/TitleButton-0-3.png");
//    taskButton->setPosition(Vec2(420, 420));
//    taskButton->setName("task");
//    taskBarLayout_->addChild(taskButton);
//}
//
//void UILayer::createBagLayout()
//{
//    bagLayout_ = ui::Layout::create();
//    bagLayout_->setContentSize(Size(400, 100)); // ���ñ�������ĸ߶�Ϊ100����
//    bagLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
//    bagLayout_->setPosition(Vec2(0, -100)); // ��ʼʱ��������Ļ�·�
//    bagLayout_->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
//    bagLayout_->setBackGroundColor(Color3B(80, 80, 80)); // �л�ɫ����
//    this->addChild(bagLayout_, 4);
//
//    // ��ӱ�������
//    const int numSlots = 12; // ������12������
//    const int columns = 12; // ÿ��12��
//    const int rows = 2; // 2��
//    const float slotSize = 16.0f; // ÿ�����ӵĴ�С
//    const float padding = 1.0f; // ���
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
//    // ��ӹرձ�����ť
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
//    // ��ӿ�ʼ��ť
//    auto startButton = ui::Button::create("image/TitleButton-0-4.png", "image/TitleButton-0-0.png");
//    startButton->setPosition(Vec2(64, 32));
//    startButton->setName("new");
//    startScreenLayout_->addChild(startButton);
//
//    // ��Ӽ��ذ�ť
//    auto loadButton = ui::Button::create("image/TitleButton-0-5.png", "image/TitleButton-0-1.png");
//    loadButton->setPosition(Vec2(192, 32));
//    loadButton->setName("load");
//    startScreenLayout_->addChild(startButton);
//
//    // ����˳���ť
//    auto exitButton = ui::Button::create("image/TitleButton-0-7.png", "image/TitleButton-0-3.png");
//    exitButton->setPosition(Vec2(320, 32));
//    exitButton->setName("exit");
//    startScreenLayout_->addChild(startButton);
//}
//
//void UILayer::showBag()
//{
//    // ʹ�ö���������������Ұ���ӵײ����ϻ�����
//    auto moveIn = MoveBy::create(0.3f, Vec2(0, 100));
//    bagLayout_->runAction(moveIn);
//}
//
//void UILayer::hideBag()
//{
//    // ʹ�ö���������������Ұ�����»�����
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
//    // �������б������Ӳ�������Ʒ��ʾ
//    const int numSlots = 12;
//    for (int i = 0; i < numSlots; ++i)
//    {
//        auto slot = dynamic_cast<ui::Button*>(bagLayout_->getChildByName("Slot_" + std::to_string(i)));
//        if (slot)
//        {
//            // ���֮ǰ���ӽڵ㣨��Ʒͼ�꣩
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
//                    // �����Ʒ������ʾ
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
