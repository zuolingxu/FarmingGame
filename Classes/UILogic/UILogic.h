#pragma once
#include "cocos2d.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <map>

class Character {
public:
    // 物品类型枚举
    enum class ItemType {
        FERTILIZER,
        SEED,
        WATERING_CAN,
        NONE
    };

    // 物品结构
    struct Item {
        ItemType type;
        std::string name;
    };

private:
    std::vector<Item> inventory; // 人物持有的物品
    static Character* instance;   // 单例实例

    // 私有构造函数，防止外部实例化
    Character() {
        // 可以在这里进行一些初始化操作
    }

public:
    // 获取单例实例
    static Character* getInstance() {
        if (!instance) {
            instance = new Character(); // 动态内存分配
        }
        return instance;
    }

    void addItem(const Item& item) {
        inventory.push_back(item);
    }

    const std::vector<Item>& getInventory() const {
        return inventory;
    }

    bool hasItem(ItemType type) const {
        for (const auto& item : inventory) {
            if (item.type == type) {
                return true;
            }
        }
        return false;
    }

    // 清理单例实例（可选）
    static void cleanup() {
        delete instance; // 释放动态分配的内存
        instance = nullptr;
    }

    // 防止拷贝构造和赋值操作
    Character(const Character&) = delete;
    Character& operator=(const Character&) = delete;

    // 如果需要支持移动操作，可以添加移动构造函数和移动赋值运算符
    // Character(Character&&) = default;
    // Character& operator=(Character&&) = default;

    // 析构函数声明为私有，防止外部删除
    ~Character() {
        // 可以在这里进行一些清理操作
    }
};
