#pragma once
#include "cocos2d.h"
#include <string>
#include <vector>
#include "HelperClasses.h"  
#include "Item.h"
#include "RealItem.h"
#include "NullItem.h"

USING_NS_CC;

class MainCharacter {
public:
    const int MAX_QUANTITY = 99;  //item max quantity
    const int MAX_ENERGY = 300;
    const int MAX_MONEY = 114514;
    const int MAX_LEVEL = 20;

    const int Tilling_the_soil_consumes_energy = -8;
    const int Getting_mineral_consumes_energy = -8;
    const int Watering_crops_consumes_energy = -4;
    const int Fertilizing_consumes_energy = -2;
    const int Fishing_consumes_energy = -15;

    const int Eating_cauliflower_gain_energy = 50;
    const int Eating_potato_gain_energy = 80;
    const int Eating_pumpkin_gain_energy = 100;
    const int Eating_fish_gain_energy = 30;
    const int Eating_soup_gain_energy = 150;

private:
    std::vector<std::unique_ptr<RealItem>> inventory;       // Backpack items
    Item* currentItem = NullItem::getInstance();                 // The current item held by the character (only one item can be held)
    static MainCharacter* instance;    // Singleton instance

    int energy = MAX_ENERGY;           // every time inter game is 6:00 energy is 100, dont need to load from archive or save in archive

    int money;

    int level;

    // Private constructor to prevent direct creation of an instance
    MainCharacter();

public:
    // Get the singleton instance
    static MainCharacter* getInstance();

    // modify energy
    // energy can meet need return 0
    bool modifyEnergy(int delta);

    // Get energy
    int getEnergy() { return energy; }

    bool modifyMoney(int delta);

    int getCurrentMoney() { return money; }

    // Get all items in the backpack
    const std::vector<std::unique_ptr<RealItem>>& getInventory() const { return inventory; }

    // Check if the backpack contains an item of a specific type
    bool hasItem(ItemType type) const;

    // Set the current held item
    void setCurrentItem(ItemType type);

    // Get the current held item
    const Item* getCurrentItem() const { return currentItem; }

    // Get the type of the current held item
    ItemType getCurrentItemType() const { return currentItem->getType(); }

    // Modify the quantity of a specified item in the backpack (+n or -n)
    bool modifyItemQuantity(ItemType type, int delta);

    //
    bool modifylevel(int delta);

    // Clean up the singleton instance
    static void cleanup();

    // Disable copy constructor and assignment operator
    MainCharacter(const MainCharacter&) = delete;
    MainCharacter& operator=(const MainCharacter&) = delete;

    // Destructor
    ~MainCharacter();

    // Load inventory from JSON
    void loadInventoryFromArchive(const rapidjson::Value& json);

    void eat_food_and_gain_energy(ItemType type);

    void change_archive_in_memory();

    void level_gift(); //give gift when levels up

    std::vector<RealItem*> getRawInventory() const {
    std::vector<RealItem*> raw;
    raw.reserve(inventory.size());
    for (const auto& ptr : inventory)
        raw.push_back(ptr.get());
    return raw;
}
};