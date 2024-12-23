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
            case UILayerType::TIME:
                return ret->timeLayout_;
                break;
            case UILayerType::SHOP:
                return ret->shopLayout_;
                break;
            case UILayerType::MANUFACTURE:
                return ret->manufactureLayout_;
                break;
            case UILayerType::FISHING:
                return ret->fishLayout_;
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
        case UILayerType::TIME:
            createTimeLayout();
            uilogic->initTimeNode(timeLayout_);
            break;
        case UILayerType::SHOP:
            createShopLayout();
            uilogic->initShopNode(shopLayout_);
            break;
        case UILayerType::MANUFACTURE:
            createManufactureLayout();
            uilogic->initManufactureNode(manufactureLayout_);
            break;
        case UILayerType::FISHING:
            createFishLayout();
            uilogic->initFishNode(fishLayout_);
            break;
        default:
            break;
    }

    return true;
}

void UILayer::createTaskBarLayout()
{
    //create taskbar layout
    taskBarLayout_ = ui::Layout::create();
    taskBarLayout_->setContentSize(Director::getInstance()->getWinSize());
    taskBarLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    taskBarLayout_->setPosition(Vec2(0,0));
    taskBarLayout_->setName("taskbar");

    // create closebutton
    auto closeButton = ui::Button::create("image/exit.png", "image/exit.png");
    closeButton->setPosition(Vec2(420, 300));
    closeButton->setName("CloseButton");
    taskBarLayout_->addChild(closeButton);

}

void UILayer::createBagLayout()
{
    //create bag layout
    bagLayout_ = ui::Layout::create();
    bagLayout_->setContentSize(Director::getInstance()->getWinSize());
    bagLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    bagLayout_->setPosition(Vec2(0,0)); 
    bagLayout_->setName("bag");

    //create all the slots
    const int numSlots = 24; // the total number of slots
    const int columns = 12; // the columns of slots
    const int rows = 2; // the rows of slots
    const float slotSize = 20.0f; // the size of each slot

    for (int i = 0; i < numSlots; ++i)
    {
        int row = i / columns ;
        int col = i % columns;

        auto slot = ui::Button::create("image/frame-0.png", "image/frame-1.png");
        slot->setScale9Enabled(true);
        slot->setContentSize(Size(slotSize, slotSize));
        float x =  col * slotSize  + slotSize / 2 + 120;
        float y =  (1-row) * slotSize  + slotSize / 2  ;
        slot->setPosition(Vec2(x, y));
        slot->setName("Slot_" + std::to_string(i));

        bagLayout_->addChild(slot);
    }

}

void UILayer::createStartScreenLayout()
{
    //create startscreen layout
    startScreenLayout_ = ui::Layout::create();
    startScreenLayout_->setContentSize(Director::getInstance()->getWinSize());
    startScreenLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    startScreenLayout_->setPosition(Vec2(0, 0));
    startScreenLayout_->setName("startscreen");

    // create startbutton
    auto startButton = ui::Button::create("image/TitleButtons-0-4.png", "image/TitleButtons-0-0.png");
    startButton->setPosition(Vec2(100,64));
    startButton->setName("NewButton");
    startScreenLayout_->addChild(startButton);

    // create loadbutton
    auto loadButton = ui::Button::create("image/TitleButtons-0-5.png", "image/TitleButtons-0-1.png");
    loadButton->setPosition(Vec2(228, 64));
    loadButton->setName("LoadButton");
    startScreenLayout_->addChild(loadButton);

    // create exitbutton
    auto exitButton = ui::Button::create("image/TitleButtons-0-7.png", "image/TitleButtons-0-3.png");
    exitButton->setPosition(Vec2(356, 64));
    exitButton->setName("ExitButton");
    startScreenLayout_->addChild(exitButton);
}

void UILayer::createNpcLayout()
{
    //create npc layout
    npcLayout_ = ui::Layout::create();
    npcLayout_->setContentSize(Director::getInstance()->getWinSize());
    npcLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    npcLayout_->setPosition(Vec2(0, 0));
    npcLayout_->setName("npc");

    // create textbox
    auto textbox = ui::Button::create("image/textBox..png", "image/textBox..png");
    textbox->setPosition(Vec2(240, 60));
    textbox->setScale9Enabled(true);
    textbox->setContentSize(Size(400,100));
    textbox->setName("textbox");
    npcLayout_->addChild(textbox);

    // create closebutton
    auto closeButton = ui::Button::create("image/exit.png", "image/exit.png");
    closeButton->setPosition(Vec2(432, 108));
    closeButton->setName("CloseButton");
    npcLayout_->addChild(closeButton);

    // create the potrait of different npc
    auto potrait = ui::Button::create("image/abi.png", "image/abi.png");
    potrait->setPosition(Vec2(90, 142));
    potrait->setName("portrait");
    npcLayout_->addChild(potrait);
}

void UILayer::createLoadArchiveLayout()
{
    //create loadarchive layout
    loadArchiveLayout_ = ui::Layout::create();
    loadArchiveLayout_->setContentSize(Director::getInstance()->getWinSize());
    loadArchiveLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    loadArchiveLayout_->setPosition(Vec2(0, 0));
    loadArchiveLayout_->setName("loadarchive");
}

void UILayer::createTimeLayout()
{
    //create time layout
    timeLayout_ = ui::Layout::create();
    timeLayout_->setContentSize(Director::getInstance()->getWinSize());
    timeLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    timeLayout_->setPosition(Vec2(0, 0));
    timeLayout_->setName("time");

    // create displayer
    auto displayer = ui::Button::create("image/time.png", "image/time.png");
    displayer->setPosition(Vec2(438, 285));
    displayer->setScale9Enabled(true);
    displayer->setContentSize(Size(95, 75));
    displayer->setName("displayer");

    // create day which is added to displayer
    auto day = ui::Text::create("Day: 1", "fonts/arial.ttf", 10);
    day->setColor(Color3B(93, 59, 23));
    day->setPosition(Vec2(48,35));
    day->setName("day");
    displayer->addChild(day);

    // create hour which is added to displayer
    auto hour = ui::Text::create("6:00", "fonts/arial.ttf", 10);
    hour->setColor(Color3B(93, 59, 23));
    hour->setPosition(Vec2(48, 25));
    hour->setName("hour");
    displayer->addChild(hour);

    // create power which is added to displayer
    auto power = ui::Text::create("Power:100", "fonts/arial.ttf", 10);
    power->setColor(Color3B(93, 59, 23));
    power->setPosition(Vec2(53, 65));
    power->setName("power");
    displayer->addChild(power);

    // create money which is added to displayer
    auto money = ui::Text::create("Money:1000", "fonts/arial.ttf", 10);
    money->setColor(Color3B(93, 59, 23));
    money->setPosition(Vec2(45, 7));
    money->setName("money");
    displayer->addChild(money);

    // create level which is added to displayer
    auto level= ui::Text::create("LEVEL:1", "fonts/arial.ttf", 8);
    level->setColor(Color3B(93, 59, 23));
    level->setPosition(Vec2(45, 52));
    level->setName("level");
    displayer->addChild(level);

    timeLayout_->addChild(displayer);
}

void UILayer::createShopLayout()
{
    //create shop layout
    shopLayout_ = ui::Layout::create();
    shopLayout_->setContentSize(Director::getInstance()->getWinSize());
    shopLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    shopLayout_->setPosition(Vec2(0, 0));
    shopLayout_->setName("shop");

    // create purchase zone
    auto label1 = ui::Button::create("image/textBox..png", "image/textBox..png");
    label1->setScale9Enabled(true);
    label1->setContentSize(Size(180, 30));
    label1->setPosition(Vec2(240, 300));

    // create label for purchase zone
    auto Label1 = ui::Text::create("Purchase zone", "fonts/arial.ttf", 12);
    Label1->setPosition(Vec2(90, 15));
    Label1->setColor(Color3B(0, 0, 0));
    label1->addChild(Label1);
    shopLayout_->addChild(label1);

    // create sell zone
    auto label2 = ui::Button::create("image/textBox..png", "image/textBox..png");
    label2->setScale9Enabled(true);
    label2->setContentSize(Size(180, 30));
    label2->setPosition(Vec2(240, 140));

    // create label for sell zone
    auto Label2 = ui::Text::create("Sell zone", "fonts/arial.ttf", 12);
    Label2->setPosition(Vec2(90, 15));
    Label2->setColor(Color3B(0, 0, 0));
    label2->addChild(Label2);
    shopLayout_->addChild(label2);

    // create button to buy cauliflower
    auto cauliflower = ui::Button::create("image/shop.png", "image/shop.png");
    cauliflower->setScale9Enabled(true);
    cauliflower->setContentSize(Size(360, 36));
    cauliflower->setPosition(Vec2(240, 252));
    cauliflower->setName("cauliflower");

    // create cauliflower picture which is added to cauliflower button
    auto cauliflower_pic = ui::Button::create("image/bagobjects-8.png", "image/bagobjects-8.png");
    cauliflower_pic->setScale(2.0);
    cauliflower_pic->setPosition(Vec2(25, 18));
    cauliflower->addChild(cauliflower_pic);

    // create label for cauliflower which is added to cauliflower button
    auto cauliflowerLabel = ui::Text::create("Cauliflower seed     50", "fonts/Marker Felt.ttf", 14);
    cauliflowerLabel->setPosition(Vec2(180, 18));
    cauliflower->addChild(cauliflowerLabel);
    shopLayout_->addChild(cauliflower);

    // create button to buy potato
    auto potato = ui::Button::create("image/shop.png", "image/shop.png");
    potato->setScale9Enabled(true);
    potato->setContentSize(Size(360, 36));
    potato->setPosition(Vec2(240, 216));
    potato->setName("potato");

    // create potato picture which is added to potato button
    auto potato_pic = ui::Button::create("image/bagobjects-9.png", "image/bagobjects-9.png");
    potato_pic->setScale(2.0);
    potato_pic->setPosition(Vec2(25, 18));
    potato->addChild(potato_pic);

    // create label for potato which is added to potato button
    auto potatoLabel = ui::Text::create("Potato seed         30", "fonts/Marker Felt.ttf", 14);
    potatoLabel->setPosition(Vec2(180, 18));
    potato->addChild(potatoLabel);
    shopLayout_->addChild(potato);

    // create button to buy pumpkin
    auto pumpkin = ui::Button::create("image/shop.png", "image/shop.png");
    pumpkin->setScale9Enabled(true);
    pumpkin->setContentSize(Size(360, 36));
    pumpkin->setPosition(Vec2(240, 180));
    pumpkin->setName("pumpkin");

    // create pumpkin picture which is added to pumpkin button
    auto pumpkin_pic = ui::Button::create("image/bagobjects-10.png", "image/bagobjects-10.png");
    pumpkin_pic->setScale(2.0);
    pumpkin_pic->setPosition(Vec2(25, 18));
    pumpkin->addChild(pumpkin_pic);

    // create label for pumpkin which is added to pumpkin button
    auto pumpkinLabel = ui::Text::create("Pumpkin seed       80", "fonts/Marker Felt.ttf", 14);
    pumpkinLabel->setPosition(Vec2(180, 18));
    pumpkin->addChild(pumpkinLabel);
    shopLayout_->addChild(pumpkin);

    // create button to sell cauliflower
    auto sell_cauliflower = ui::Button::create("image/shop.png", "image/shop.png");
    sell_cauliflower->setScale9Enabled(true);
    sell_cauliflower->setContentSize(Size(360, 36));
    sell_cauliflower->setPosition(Vec2(240, 100));
    sell_cauliflower->setName("sell_cauliflower");

    // create cauliflower picture which is added to sell cauliflower button
    auto sell_cauliflower_pic = ui::Button::create("image/bagobjects-1.png", "image/bagobjects-1.png");
    sell_cauliflower_pic->setScale(2.0);
    sell_cauliflower_pic->setPosition(Vec2(25, 18));
    sell_cauliflower->addChild(sell_cauliflower_pic);

    // create label for cauliflower which is added to sell cauliflower button
    auto sell_cauliflowerLabel = ui::Text::create("Cauliflower           100", "fonts/Marker Felt.ttf", 14);
    sell_cauliflowerLabel->setPosition(Vec2(180, 18));
    sell_cauliflower->addChild(sell_cauliflowerLabel);
    shopLayout_->addChild(sell_cauliflower);

    // create button to sell potato
    auto sell_potato = ui::Button::create("image/shop.png", "image/shop.png");
    sell_potato->setScale9Enabled(true);
    sell_potato->setContentSize(Size(360, 36));
    sell_potato->setPosition(Vec2(240, 64));
    sell_potato->setName("sell_potato");

    // create potato picture which is added to sell potato button
    auto sell_potato_pic = ui::Button::create("image/bagobjects-2.png", "image/bagobjects-2.png");
    sell_potato_pic->setScale(2.0);
    sell_potato_pic->setPosition(Vec2(25, 18));
    sell_potato->addChild(sell_potato_pic);

    // create label for potato which is added to sell potato button
    auto sell_potatoLabel = ui::Text::create("Potato                60", "fonts/Marker Felt.ttf", 14);
    sell_potatoLabel->setPosition(Vec2(180, 18));
    sell_potato->addChild(sell_potatoLabel);
    shopLayout_->addChild(sell_potato);

    // create button to sell pumpkin
    auto sell_pumpkin = ui::Button::create("image/shop.png", "image/shop.png");
    sell_pumpkin->setScale9Enabled(true);
    sell_pumpkin->setContentSize(Size(360, 36));
    sell_pumpkin->setPosition(Vec2(240, 28));
    sell_pumpkin->setName("sell_pumpkin");

    // create pumpkin picture which is added to sell pumpkin button
    auto sell_pumpkin_pic = ui::Button::create("image/bagobjects-3.png", "image/bagobjects-3.png");
    sell_pumpkin_pic->setScale(2.0);
    sell_pumpkin_pic->setPosition(Vec2(25, 18));
    sell_pumpkin->addChild(sell_pumpkin_pic);

    // create label for pumpkin which is added to sell pumpkin button
    auto sell_pumpkinLabel = ui::Text::create("Pumpkin              160", "fonts/Marker Felt.ttf", 14);
    sell_pumpkinLabel->setPosition(Vec2(180, 18));
    sell_pumpkin->addChild(sell_pumpkinLabel);
    shopLayout_->addChild(sell_pumpkin);
}


void UILayer::createManufactureLayout()
{
    // create manufacture layout
    manufactureLayout_ = ui::Layout::create();
    manufactureLayout_->setContentSize(Director::getInstance()->getWinSize());
    manufactureLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    manufactureLayout_->setPosition(Vec2(0, 0));
    manufactureLayout_->setName("manufacture");

    // create manufacture box
    auto box = ui::Button::create("image/textBox..png", "image/textBox..png");
    box->setScale9Enabled(true);
    box->setContentSize(Size(360, 240));
    box->setPosition(Vec2(240, 160));
    box->setName("box");

    // create button to manufacture fertilizer
    auto fertilizer = ui::Button::create("image/bagobjects-5.png", "image/bagobjects-5.png");
    fertilizer->setScale(2.5);
    fertilizer->setPosition(Vec2(50, 200));
    fertilizer->setName("fertilizer");
    box->addChild(fertilizer);

    // create button to manufacture soup
    auto soup = ui::Button::create("image/bagobjects-4.png", "image/bagobjects-4.png");
    soup->setScale(2.5);
    soup->setPosition(Vec2(100, 200));
    soup->setName("soup");
    box->addChild(soup);

    manufactureLayout_->addChild(box);
}

void UILayer::createFishLayout()
{
    // create fish layout
    fishLayout_ = ui::Layout::create();
    fishLayout_->setContentSize(Director::getInstance()->getWinSize());
    fishLayout_->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    fishLayout_->setPosition(Vec2(0,0));
    fishLayout_->setName("fish");

    // create fish box that contains different fish and close button
    auto fishbox = ui::Button::create("image/fish.png", "image/fish.png");
    fishbox->setPosition(Vec2(260, 240));
    fishbox->setName("fishbox");

    // create close button
    auto closeButton = ui::Button::create("image/exit.png", "image/exit.png");
    closeButton->setScale(0.7);
    closeButton->setPosition(Vec2(296, 265));
    closeButton->setName("CloseButton");

    fishLayout_->addChild(fishbox);
    fishLayout_->addChild(closeButton);
}

