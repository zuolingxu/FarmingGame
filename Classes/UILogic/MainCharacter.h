#pragma once
#include "cocos2d.h"
#include <string>
#include <vector>


USING_NS_CC;

class MainCharacter {
public:
    enum class ItemType {
        // 不同种类的种子
        CAULIFLOWER_SEED,
        PUMPKIN_SEED,
        POTATO_SEED,

        // 成熟后的作物
        CAULIFLOWER,
        PUMPKIN,
        POTATO,

        // 工具
        FERTILIZER,
        FISHING_ROD,
        HOE,
        PICKAXE,
        WATERING_CAN,

        // 默认无物品
        NONE
    };

    struct Item {
        ItemType type;
        int quantity;
        Item(ItemType t,int q=1) : type(t),quantity(1) {}
    };

private:
    std::vector<Item> inventory;       // 背包物品
    Item* currentItem;                 // 当前持有的物品（只能有一个）
    static MainCharacter* instance;    // 单例实例

    // 存储 ItemType 和中文名称的映射
    static std::unordered_map<ItemType, std::string> itemTypeToName;

    // 构造函数私有，禁止外部直接创建实例
    MainCharacter();

public:
    // 获取单例实例
    static MainCharacter* getInstance();

    // 添加物品到背包
    void addItem(const Item& item);

    // 获取背包中的所有物品
    const std::vector<Item>& getInventory() const;

    // 检查背包中是否有指定类型的物品
    bool hasItem(ItemType type) const;

    // 设置当前持有的物品
    void setCurrentItem(ItemType type);

    // 获取当前持有物品
    const Item* getCurrentItem() const;

    // 修改背包中指定物品的数量 +n 或者 -n
    bool modifyItemQuantity(ItemType type, int delta);

    // 清理单例实例
    static void cleanup();

    // 禁止复制和赋值
    MainCharacter(const MainCharacter&) = delete;
    MainCharacter& operator=(const MainCharacter&) = delete;

    // 析构函数
    ~MainCharacter();
};
