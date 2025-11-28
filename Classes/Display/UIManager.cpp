#include "UIManager.h"
#include "UILayer.h"
#include "UILogic.h"

USING_NS_CC;

static constexpr int FRONT_UI_ZORDER = 10;
static constexpr int BACK_UI_ZORDER = -10;

UIManager* UIManager::instance_ = nullptr;

UIManager::UIManager()
    : permanent_node_(nullptr)
{
}

UIManager::~UIManager()
{
    if (permanent_node_) {
        permanent_node_->release();
    }
}

UIManager* UIManager::getInstance()
{
    if (instance_ == nullptr) {
        instance_ = new UIManager();
    }
    return instance_;
}

void UIManager::initUIs()
{
    if (permanent_node_ == nullptr) {
        permanent_node_ = Node::create();
        permanent_node_->retain();
        permanent_node_->setName("UI");

        Node* start_screen = UILayer::createUILayer(UILayerType::START_SCREEN);
        Node* bag = UILayer::createUILayer(UILayerType::BAG);
        Node* task_bar = UILayer::createUILayer(UILayerType::TASK_BAR);
        Node* npc = UILayer::createUILayer(UILayerType::NPC);
        Node* load_archive = UILayer::createUILayer(UILayerType::LOAD_ARCHIVE);
        Node* time = UILayer::createUILayer(UILayerType::TIME);
        Node* shop = UILayer::createUILayer(UILayerType::SHOP);
        Node* manufacture = UILayer::createUILayer(UILayerType::MANUFACTURE);
        Node* fish = UILayer::createUILayer(UILayerType::FISHING);

        permanent_node_->addChild(start_screen, BACK_UI_ZORDER);
        permanent_node_->addChild(bag, BACK_UI_ZORDER);
        permanent_node_->addChild(task_bar, BACK_UI_ZORDER);
        permanent_node_->addChild(npc, BACK_UI_ZORDER);
        permanent_node_->addChild(load_archive, BACK_UI_ZORDER);
        permanent_node_->addChild(time, BACK_UI_ZORDER);
        permanent_node_->addChild(shop, BACK_UI_ZORDER);
        permanent_node_->addChild(manufacture, BACK_UI_ZORDER);
        permanent_node_->addChild(fish, BACK_UI_ZORDER);

        UILogic* uilogic = UILogic::getInstance();
        uilogic->initStartScreenNode(start_screen);
        uilogic->initBagNode(bag); // 已修正，原为initStartScreenNode
        uilogic->initTaskBarNode(task_bar);
        uilogic->initNpcNode(npc);
        uilogic->initLoadArchiveNode(load_archive);
        uilogic->initShopNode(shop);
        uilogic->initManufactureNode(manufacture);
        uilogic->initFishNode(fish);
    }
}

cocos2d::Node* UIManager::getUINode()
{
    return permanent_node_;
}

void UIManager::hideUILayer(const std::string& UI_name)
{
    if (UI_name == "ALL") {
        for (auto layer : permanent_node_->getChildren()) {
            layer->setVisible(false);
            layer->setLocalZOrder(BACK_UI_ZORDER);
            layer->pause();
        }
        current_UI_name_.clear();
    } else if (UI_name == "CURRENT") {
        if (!current_UI_name_.empty()) {
            Node* layer = permanent_node_->getChildByName(current_UI_name_);
            if (layer != nullptr) {
                layer->setVisible(false);
                layer->setLocalZOrder(BACK_UI_ZORDER);
                layer->pause();
            }
            current_UI_name_.clear();
        }
    } else {
        Node* layer = permanent_node_->getChildByName(UI_name);
        if (layer != nullptr) {
            layer->setVisible(false);
            layer->setLocalZOrder(BACK_UI_ZORDER);
            layer->pause();
            current_UI_name_.clear();
        }
    }
}

void UIManager::showUILayer(const std::string& UI_name, bool base)
{
    Node* layer = permanent_node_->getChildByName(UI_name);
    if (layer != nullptr) {
        if (!base) {
            hideUILayer("CURRENT");
            current_UI_name_ = UI_name;
        }
        layer->setVisible(true);
        layer->setLocalZOrder(FRONT_UI_ZORDER + (base ? -1 : 0)); // 已修正逻辑
        layer->resume();
    }
}
