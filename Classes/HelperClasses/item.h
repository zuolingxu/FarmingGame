#pragma once

#include <string>
#include "HelperClasses.h"
#include "DocumentManager.h"

class Item
{
public:
    virtual ~Item() = default;

    virtual ItemType    getType() const = 0;
    virtual std::string getIconPath() const = 0;

    // Only real items have quantity
    virtual int  getQuantity() const { return 0; }
    virtual void setQuantity(int /*q*/) {}

    // Helper queries
    virtual bool isNone() const { return false; }
    virtual bool isTool() const { return false; }
    virtual bool canMine() const { return false; }
    virtual bool canHoe() const { return false; }
    virtual bool canWater() const { return false; }
    virtual bool canFish() const { return false; }
    virtual bool isConsumable() const { return false; }

    // Actions (safe no-op for NullItem)
    virtual void use() const {}
    virtual void consume() const {}
};