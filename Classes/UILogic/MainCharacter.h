#pragma once
#include "cocos2d.h"
#include <string>
#include <vector>
#include "HelperClasses.h"  

USING_NS_CC;

class MainCharacter {

private:
    std::vector<Item> inventory;       // Backpack items
    Item* currentItem;                 // The current item held by the character (only one item can be held)
    static MainCharacter* instance;    // Singleton instance

    // Private constructor to prevent direct creation of an instance
    MainCharacter();

public:
    // Get the singleton instance
    static MainCharacter* getInstance();

    // Add an item to the backpack
    void addItem(const Item& item);

    // Get all items in the backpack
    const std::vector<Item>& getInventory() const;

    // Check if the backpack contains an item of a specific type
    bool hasItem(ItemType type) const;

    // Set the current held item
    void setCurrentItem(ItemType type);

    // Get the current held item
    const Item* getCurrentItem() const;

    // Modify the quantity of a specified item in the backpack (+n or -n)
    bool modifyItemQuantity(ItemType type, int delta);

    // Clean up the singleton instance
    static void cleanup();

    // Disable copy constructor and assignment operator
    MainCharacter(const MainCharacter&) = delete;
    MainCharacter& operator=(const MainCharacter&) = delete;

    // Destructor
    ~MainCharacter();
};
