#pragma once
#include "cocos2d.h"
#include <string>
#include <vector>
#include "HelperClasses.h"  

USING_NS_CC;

class MainCharacter {
public:
    const int MAX_QUANTITY = 10000;  //item max quantity
    const int MAX_ENERGY = 500;
    const int MAX_MONEY = 114514;
    const int MAX_LEVEL = 20;

    const int Tilling_the_soil_consumes_energy = -5;
    const int Getting_mineral_consumes_energy = -5;
    const int Watering_crops_consumes_energy = -3;
    const int Fertilizing_consumes_energy = -3;

    const int Eating_cauliflower_gain_energy = 100;
    const int Eating_potato_gain_energy = 150;
    const int Eating_pumpkin_gain_energy = 200;
    const int Eating_fish_gain_energy = 250;
    const int Eating_soup_gain_energy = 300;

private:
    std::vector<Item>* inventory;       // Backpack items
    Item* currentItem;                 // The current item held by the character (only one item can be held)
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
    const std::vector<Item>const* getInventory() const;

    // Check if the backpack contains an item of a specific type
    bool hasItem(ItemType type) const;

    // Set the current held item
    void setCurrentItem(ItemType type);

    // Get the current held item
    const Item* getCurrentItem() const;

    // Get the type of the current held item
    ItemType getCurrentItemType() const;

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
};