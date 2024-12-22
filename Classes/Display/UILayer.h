#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include<vector>
#include<string>

// define the enum of different UI layers
enum class UILayerType {
    START_SCREEN,
    BAG,
    TASK_BAR,
    NPC,
    LOAD_ARCHIVE,
    TIME,
    SHOP,
    MANUFACTURE,
    FISHING
};

class UILayer : public cocos2d::Ref
{
public:
    struct Item {
        std::string name;      // the name of the item
        int quantity;          // the quantity of the item
        std::string iconPath;  // the path of the icon picture of the item

        Item(const std::string& itemName = "", int num = 0, const std::string& icon = "")
            : name(itemName), quantity(num), iconPath(icon) {}
    };
    // create the instance of UILayer 
    static cocos2d::Node* createUILayer(UILayerType type);

    // initialize the UIlayer with the type
    bool initWithType(UILayerType type);

    // get UI elements
    cocos2d::ui::Layout* getBagLayout() const { return bagLayout_; }
    cocos2d::ui::Layout* getTaskBarLayout() const { return taskBarLayout_; }
    cocos2d::ui::Layout* getStartScreenLayout() const { return startScreenLayout_; }
    cocos2d::ui::Layout* getnpcLayout() const { return npcLayout_; }
    cocos2d::ui::Layout* getloadArchiveLayout() const { return loadArchiveLayout_; }
    cocos2d::ui::Layout* getTimeLayout() const { return timeLayout_; }
    cocos2d::ui::Layout* getShopLayout() const { return shopLayout_; }
    cocos2d::ui::Layout* getManufactureLayout() const { return manufactureLayout_; }
    cocos2d::ui::Layout* getFishLayout() const { return fishLayout_; }

private:
    // UI elements
    cocos2d::ui::Layout* taskBarLayout_;
    cocos2d::ui::Layout* bagLayout_;
    cocos2d::ui::Layout* startScreenLayout_;
    cocos2d::ui::Layout* npcLayout_;
    cocos2d::ui::Layout* loadArchiveLayout_;
    cocos2d::ui::Layout* timeLayout_;
    cocos2d::ui::Layout* shopLayout_;
    cocos2d::ui::Layout* manufactureLayout_;
    cocos2d::ui::Layout* fishLayout_;


    // the methods of initializing the UI elements
    void createTaskBarLayout();
    void createBagLayout();
    void createStartScreenLayout();
    void createNpcLayout();
    void createLoadArchiveLayout();
    void createTimeLayout();
    void createShopLayout();
    void createManufactureLayout();
    void createFishLayout();

};
