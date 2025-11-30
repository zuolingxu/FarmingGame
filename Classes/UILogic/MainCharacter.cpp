#include "MainCharacter.h"
#include "DocumentManager.h"
#include "UILogic.h"
#include "json/document.h"

// Initialize static member
MainCharacter* MainCharacter::instance = nullptr;

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
    UILogic::getInstance()->refreshPowerUI(energy);

    return 1;
           
}

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
    UILogic::getInstance()->refreshMoneyUI(money);
    return 1;

}

bool MainCharacter::modifylevel(int delta) {
    // If delta is 0, return immediately (no change)
    if (delta == 0) return 1;  // 1 indicates success, quantity is unchanged
    else if (delta < 0) return 0;

    else if (level + delta > MAX_LEVEL) {
        level = MAX_LEVEL;
    }
    else {
        level += delta;

        level_gift();
        UILogic::getInstance()->refreshLevelUI(level);
    }

    return 1;

}

void MainCharacter::level_gift() {
    switch (level%4) {
  
        case 1:
            modifyItemQuantity(ItemType::POTATO, 5);
            modifyMoney(50);
            break;
		case 2:
            modifyItemQuantity(ItemType::FERTILIZER, 5);
            modifyItemQuantity(ItemType::POTATO_SEED,5);
            break;
        case 3:
            modifyItemQuantity(ItemType::PUMPKIN_SEED,10);
            modifyMoney(100);
            break;
        case 0:
            modifyItemQuantity(ItemType::CAULIFLOWER_SEED, 10);
            break;
    }
    if (level == 2)
        modifyItemQuantity(ItemType::PICKAXE, 1);
    else if(level==3)
        modifyItemQuantity(ItemType::FISHING_ROD, 1);


    return;
}


// init from archive
MainCharacter::MainCharacter():currentItem(NullItem::getInstance()),money(0) {
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
            money = (*doc)["key_info"]["money"].GetInt(); // ��ȡ��Ϸ��Ǯ
        }
        else {
            CCLOG("Archive document is missing or malformed!");
        }
        // level
        if (doc->HasMember("key_info") && (*doc)["key_info"].HasMember("level")) {
            level = (*doc)["key_info"]["level"].GetInt(); // ��ȡ��Ϸ��Ǯ
        }
        else {
            CCLOG("Archive document is missing or malformed!");
        }
    }

    //energy time money item
    // energy
    UILogic::getInstance()->refreshPowerUI(energy);
    // money
    UILogic::getInstance()->refreshMoneyUI(money);
    // show bag item in ui
     UILogic::getInstance()->updateBagItems(getRawInventory());
     // show level ui
     UILogic::getInstance()->refreshLevelUI(level);
     //time in timemanager

}


MainCharacter::~MainCharacter() = default;

void MainCharacter::loadInventoryFromArchive(const rapidjson::Value& json) {
    if (json.IsArray()) {
        inventory->clear();  // Clear existing inventory

        for (const auto& item : json.GetArray()) {
            if (item.HasMember("type") && item.HasMember("quantity")) {
                std::string typeStr = item["type"].GetString();
                int quantity = item["quantity"].GetInt();

                // Convert string to ItemType
                ItemType t = stringToItemType(typeStr);
                if (t != ItemType::NONE && quantity > 0)
                {
                    inventory.emplace_back(std::make_unique<RealItem>(t, quantity));
                }
            }
        }
        currentItem = NullItem::getInstance();  // Reset currentItem to NullItem after loading
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


std::vector<Item>* MainCharacter::getInventory() const {
    return inventory;// Return the list of items in the inventory
}

bool MainCharacter::hasItem(ItemType type) const {
    return std::any_of(inventory.begin(), inventory.end(),
        [type](const auto& item) { return item->getType() == type; });  // Return true if the item type is found
}

void MainCharacter::setCurrentItem(ItemType type) {
    for (const auto& item : inventory)
    {
        if (item->getType() == type)
        {
            currentItem = item.get();  // Set the current held item
            return;
        }
    }
    currentItem = NullItem::getInstance();  // If no matching item type is found, set to NullItem
}

const Item* MainCharacter::getCurrentItem() const {
    return currentItem;  // Return the current held item
}

ItemType MainCharacter::getCurrentItemType() const {
    return currentItem->getType();
}

bool MainCharacter::modifyItemQuantity(ItemType type, int delta) {
    // If delta is 0, return immediately (no change)
    if (delta == 0) return 1;  // 1 indicates success, quantity is unchanged

    
    // Traverse the inventory to find the specified item type
    for (auto it = inventory.begin(); it != inventory.end(); ++it)
    {
        
        if ((*it)->getType() == type)
        {
            if ((*it)->getQuantity() + delta < 0|| (*it)->getQuantity()  + delta > MAX_QUANTITY) {
                return 0;  // If the resulting quantity is less than zero or more than max quantity, return an error
            }
            
            // Modify the item quantity
            int newQty = (*it)->getQuantity() + delta;

            // If the item quantity becomes zero, remove the item from inventory
            if (newQty == 0)
            {
                if (currentItem == it->get())
                    currentItem = NullItem::getInstance();

                inventory.erase(it);
            }
            else
            {
                (*it)->setQuantity(newQty);
            }

            UILogic::getInstance()->updateBagItems(getRawInventory());
            return true;// Operation successful
        }
    }
    
    // If the item is not found, and delta is positive, add the item to the inventory
    if (delta > 0)
    {
        inventory.emplace_back(std::make_unique<RealItem>(type, delta));  // Add new item with quantity
        UILogic::getInstance()->updateBagItems();
        return true;  // Item added successfully
    }

    return 0;  // If the item is not found and delta is negative, return an error
}

void MainCharacter::cleanup() {
    delete instance;
    instance = nullptr;  // Clean up the singleton instance
}

void MainCharacter::eat_food_and_gain_energy(ItemType type) {
    for (const auto& item : inventory)
    {
        if (item->getType() == type && item->isConsumable())
        {
            item->consume();
            return;
        }
    }
}


void MainCharacter::change_archive_in_memory() {
    // ��ȡ DocumentManager ʵ��
    DocumentManager* docManager = DocumentManager::getInstance();
    rapidjson::Document* doc = docManager->getArchiveDocument();

    if (doc) {
        // ȷ�� "key_info" �� "belongings" ����
        if (!doc->HasMember("key_info")) {
            rapidjson::Value keyInfo(rapidjson::kObjectType);
            doc->AddMember("key_info", keyInfo, doc->GetAllocator());
        }

        // money to archive
        (*doc)["key_info"]["money"].SetInt(money);

        // level to archive
        (*doc)["key_info"]["level"].SetInt(level);

        // ȷ�� "belongings" �������?
        if (!doc->HasMember("belongings"))
        {
            rapidjson::Value arr(rapidjson::kArrayType);
            doc->AddMember("belongings", arr, doc->GetAllocator());
        }

        auto& arr = (*doc)["belongings"];
        arr.SetArray();

        for (const auto& item : inventory)
        {
            rapidjson::Value obj(rapidjson::kObjectType);
            obj.AddMember("type", rapidjson::Value(itemTypeToString(item->getType()).c_str(), doc->GetAllocator()), doc->GetAllocator());
            obj.AddMember("quantity", item->getQuantity(), doc->GetAllocator());
            arr.PushBack(obj, doc->GetAllocator());
        }

    }
    else {
        CCLOG("Failed to access the archive document.");
    }
}
