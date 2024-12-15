#include "MainCharacter.h"
#include "DocumentManager.h"
#include "UILogic.h"
#include "json/document.h"

// Initialize static member
MainCharacter* MainCharacter::instance = nullptr;

//todo xianshitili
bool MainCharacter::modifyEnergy(int delta) {
    // If delta is 0, return immediately (no change)
    if (delta == 0) return 1;  // 1 indicates success, quantity is unchanged
    else if (energy + delta < 0) return 0;

    else if(energy+delta>MAX_ENERGY){
        energy = MAX_ENERGY;
    }
    else {
        energy = energy + delta;
    }
    //todo xianshitili
    return 1;
           
}

//todo xianshijinqian
bool MainCharacter::modifyMoney(int delta) {
    // If delta is 0, return immediately (no change)
    if (delta == 0) return 1;  // 1 indicates success, quantity is unchanged
    else if (money + delta < 0) return 0;

    else if (money + delta > MAX_MONEY) {
        money = MAX_ENERGY;
    }
    else {
        money = money + delta;
    }
    //todo xianshijinqian
    return 1;

}

//todo shuaxinbeibao
//todo xianshitili
// init from archive
MainCharacter::MainCharacter():currentItem(nullptr),money(0) {
    // Add a default NONE item to the inventory

    // Point currentItem to the NONE item in the inventory
    DocumentManager* dm = DocumentManager::getInstance();
    // no instance, no archive
    if(dm){
        // bag
        rapidjson::Document* doc = dm->getArchiveDocument();
        if (doc && doc->HasMember("belongings") && (*doc)["belongings"].IsArray()) {
            loadInventoryFromArchive((*doc)["belongings"]);
        }
        else {
            CCLOG("Archive document is missing or malformed!");
        }
        // money
        if (doc->HasMember("key_info") && (*doc)["key_info"].HasMember("money")) {
            money = (*doc)["key_info"]["money"].GetInt(); // 读取游戏金钱
        }
        else {
            CCLOG("Archive document is missing or malformed!");
        }
    }

    //todo shuaxinbeibao
    // show bag item in ui
   // UILogic::getInstance()->updateBagItems(inventory);

}


MainCharacter::~MainCharacter() {
    // No need to delete currentItem, as it points to an object in the inventory
    currentItem = nullptr;

    // Clear the inventory
    inventory.clear();
}

void MainCharacter::loadInventoryFromArchive(const rapidjson::Value& json) {
    if (json.IsArray()) {
        inventory.clear();  // Clear existing inventory

        for (const auto& item : json.GetArray()) {
            if (item.HasMember("type") && item.HasMember("quantity")) {
                std::string typeStr = item["type"].GetString();
                int quantity = item["quantity"].GetInt();

                // Convert string to ItemType
                ItemType type = Item::stringToItemType(typeStr);
                if (type != ItemType::NONE) { // Ensure valid type
                    inventory.emplace_back(type, quantity);
                }
                else {
                    CCLOG("Unknown item type: %s", typeStr.c_str());
                }
            }
        }

        // Set current item to the first item in the inventory
        currentItem = !inventory.empty() ? &inventory.front() : nullptr;
    }
    else {
        CCLOG("Invalid JSON format for inventory!");
    }
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

ItemType MainCharacter::getCurrentItemType() const {
    return currentItem ? currentItem->type : ItemType::NONE;
}

//todo shuaxinwupinlan
bool MainCharacter::modifyItemQuantity(ItemType type, int delta) {
    // If delta is 0, return immediately (no change)
    if (delta == 0) return 1;  // 1 indicates success, quantity is unchanged

    // Traverse the inventory to find the specified item type
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->type == type) {
            // Modify the item quantity
            if (it->quantity + delta < 0|| it->quantity + delta > MAX_QUANTITY) {
                return 0;  // If the resulting quantity is less than zero or more than max quantity, return an error
            }

            it->quantity += delta;

            // If the item quantity becomes zero or less, remove the item from inventory
            if (it->quantity == 0) {
                if (currentItem == &(*it)) {
                    currentItem = nullptr;  // Clear the current item pointer
                }

                inventory.erase(it);
            }

            //todo shuaxinwupinlan
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

void MainCharacter::eat_food_and_gain_energy(ItemType type) {
    if (modifyItemQuantity(type, -1)) {
        int gain_energy = 0;
        switch (type) {
            case ItemType::CAULIFLOWER: 
                gain_energy=Eating_cauliflower_gain_energy;
                break;
            case ItemType::PUMPKIN:
                gain_energy = Eating_pumpkin_gain_energy;
                break;
            case ItemType::POTATO: 
                gain_energy = Eating_potato_gain_energy;
                break;
            case ItemType::SOUP:
                gain_energy = Eating_soup_gain_energy;
                break;
            case ItemType::FISH: 
                gain_energy = Eating_fish_gain_energy;
                break;
            
        }
        modifyEnergy(gain_energy);
    }
}


void MainCharacter::change_archive_in_memory() {
    // 获取 DocumentManager 实例
    DocumentManager* docManager = DocumentManager::getInstance();
    rapidjson::Document* doc = docManager->getArchiveDocument();

    if (doc) {
        // 确保 "key_info" 和 "belongings" 存在
        if (!doc->HasMember("key_info")) {
            rapidjson::Value keyInfo(rapidjson::kObjectType);
            doc->AddMember("key_info", keyInfo, doc->GetAllocator());
        }

        // 更新金钱
        (*doc)["key_info"]["money"].SetInt(money);

        // 确保 "belongings" 数组存在
        if (!doc->HasMember("belongings")) {
            rapidjson::Value belongings(rapidjson::kArrayType);
            doc->AddMember("belongings", belongings, doc->GetAllocator());
        }

        // 获取存档中的物品数组
        rapidjson::Value& belongingsArray = (*doc)["belongings"];

        // 遍历背包中的物品，并更新存档中的相应物品数量
        for (auto& item : inventory) {
            bool itemFound = false;

            // 遍历存档中的物品，检查是否已经有该物品
            for (auto& archivedItem : belongingsArray.GetArray()) {
                std::string storedItemType = archivedItem["type"].GetString();
                std::string currentItemType = Item::itemTypeToString(item.type);

                if (storedItemType == currentItemType) {
                    // 如果找到相同类型的物品，更新数量
                    archivedItem["quantity"].SetInt(item.quantity);
                    itemFound = true;
                    break;
                }
            }

            // 如果没有找到该物品，说明是新物品，添加到存档中
            if (!itemFound) {
                rapidjson::Value newItem(rapidjson::kObjectType);
                newItem.AddMember("type", rapidjson::Value(Item::itemTypeToString(item.type).c_str(), doc->GetAllocator()), doc->GetAllocator());
                newItem.AddMember("quantity", item.quantity, doc->GetAllocator());
                belongingsArray.PushBack(newItem, doc->GetAllocator());
            }
        }

    }
    else {
        CCLOG("Failed to access the archive document.");
    }
}
