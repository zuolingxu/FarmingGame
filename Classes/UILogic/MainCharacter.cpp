#include "MainCharacter.h"

// 初始化静态成员
MainCharacter* MainCharacter::instance = nullptr;


MainCharacter::MainCharacter() : currentItem(nullptr) {
    // 可以在此处初始化任何需要的成员
}

MainCharacter* MainCharacter::getInstance() {
    if (!instance) {
        instance = new MainCharacter();
    }
    return instance;
}

void MainCharacter::addItem(const Item& item) {
    inventory.push_back(item);
}

const std::vector<MainCharacter::Item>& MainCharacter::getInventory() const {
    return inventory;
}

bool MainCharacter::hasItem(ItemType type) const {
    for (const auto& item : inventory) {
        if (item.type == type) {
            return true;
        }
    }
    return false;
}

void MainCharacter::setCurrentItem(ItemType type) {
    for (auto& item : inventory) {
        if (item.type == type) {
            currentItem = &item; // 设置当前持有的物品
            return;
        }
    }
    currentItem = nullptr; // 如果没有找到对应类型的物品，设置为 nullptr
}

const MainCharacter::Item* MainCharacter::getCurrentItem() const {
    return currentItem;
}

bool MainCharacter::modifyItemQuantity(ItemType type, int delta) {
    // 如果delta为0，直接返回
    if (delta == 0) return 1;  // 1 表示操作成功，数量没有变化

    // 遍历背包中的物品，找到指定类型的物品
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->type == type) {
            // 修改物品数量
            if (it->quantity + delta < 0) {
                return 0;  // 如果修改后的数量小于零，返回错误
            }

            it->quantity += delta;

            // 如果物品数量变为零或更少，删除该物品
            if (it->quantity == 0) {
                inventory.erase(it);
            }
            return 1;  // 操作成功
        }
    }

    // 如果没有找到该物品类型，且 delta 大于零，添加该物品到背包
    if (delta > 0) {
        inventory.push_back(Item(type, delta));
        return 1;  // 添加物品成功
    }

    return 0;  // 如果没有找到物品，且 delta 为负，返回错误
}


void MainCharacter::cleanup() {
    delete instance;
    instance = nullptr;
}

MainCharacter::~MainCharacter() 
{

}
