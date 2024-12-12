#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

// 定义 UI 类型枚举
enum class UILayerType {
    START_SCREEN,
    BAG,
    TASK_BAR
};

class UILayer : public cocos2d::Ref
{
public:
    struct Item {
        std::string name;      // 物品名称
        int quantity;          // 物品数量
        std::string iconPath;  // 物品图标路径

        Item(const std::string& itemName = "", int num = 0, const std::string& icon = "")
            : name(itemName), quantity(num), iconPath(icon) {}
    };
    // 创建 UILayer 实例
    static cocos2d::Node* createUILayer(UILayerType type);

    // 初始化 UILayer
    bool initWithType(UILayerType type);

    // 获取 UI 元素，如背包栏、任务栏等
    cocos2d::ui::Layout* getBagLayout() const { return bagLayout_; }
    cocos2d::ui::Layout* getTaskBarLayout() const { return taskBarLayout_; }
    cocos2d::ui::Layout* getStartScreenLayout() const { return startScreenLayout_; }
        
    // TODO: Manufacture
    // TODO: BagLayer
    // TODO: ListLayer

private:
    // UI 组件
    cocos2d::ui::Layout* taskBarLayout_;
    cocos2d::ui::Layout* bagLayout_;
    cocos2d::ui::Layout* startScreenLayout_;

    // 创建各个 UI 组件的方法
    void createTaskBarLayout();
    void createBagLayout();
    void createStartScreenLayout();
};
