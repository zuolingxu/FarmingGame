#include "MainCharacter.h"

// Initialize static member
MainCharacter* MainCharacter::instance = nullptr;

MainCharacter::MainCharacter() : currentItem(nullptr) {
    // Initialize any required members here
}

MainCharacter* MainCharacter::getInstance() {
    if (!instance) {
        instance = new MainCharacter();
    }
    return instance;
}


const std::vector<Item>& MainCharacter::getInventory() const {
    return inventory;  // Return the list of items in the inventory
}

bool MainCharacter::hasItem(ItemType type) const {
    for (const auto& item : inventory) {
        if (item.type == type) {
            return true;  // Return true if the item type is found
        }
    }
    return false;  // Return false if the item type is not found
}

void MainCharacter::setCurrentItem(ItemType type) {
    for (auto& item : inventory) {
        if (item.type == type) {
            currentItem = &item;  // Set the current held item
            return;
        }
    }
    currentItem = nullptr;  // If no matching item type is found, set to nullptr
}

const Item* MainCharacter::getCurrentItem() const {
    return currentItem;  // Return the current held item
}

bool MainCharacter::modifyItemQuantity(ItemType type, int delta) {
    // If delta is 0, return immediately (no change)
    if (delta == 0) return 1;  // 1 indicates success, quantity is unchanged

    // Traverse the inventory to find the specified item type
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->type == type) {
            // Modify the item quantity
            if (it->quantity + delta < 0) {
                return 0;  // If the resulting quantity is less than zero, return an error
            }

            it->quantity += delta;

            // If the item quantity becomes zero or less, remove the item from inventory
            if (it->quantity == 0) {
                inventory.erase(it);
            }
            return 1;  // Operation successful
        }
    }

    // If the item is not found, and delta is positive, add the item to the inventory
    if (delta > 0) {
        inventory.push_back(Item(type, delta));  // Add new item with quantity
        return 1;  // Item added successfully
    }

    return 0;  // If the item is not found and delta is negative, return an error
}

void MainCharacter::cleanup() {
    delete instance;
    instance = nullptr;  // Clean up the singleton instance
}

MainCharacter::~MainCharacter() {
    // Destructor
}