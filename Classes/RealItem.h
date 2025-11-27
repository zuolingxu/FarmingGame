#pragma once

#include "Item.h"

class RealItem final : public Item
{
private:
    ItemType    type_;
    int         quantity_;
    std::string iconPath_;

public:
    RealItem(ItemType t, int q = 1);

    // Item interface
    ItemType    getType() const override     { return type_; }
    std::string getIconPath() const override { return iconPath_; }

    int  getQuantity() const override { return quantity_; }
    void setQuantity(int q) override  { quantity_ = q; }

    bool isNone() const override { return type_ == ItemType::NONE; }

    bool isTool() const override;
    bool canMine() const override  { return type_ == ItemType::PICKAXE; }
    bool canHoe() const override   { return type_ == ItemType::HOE; }
    bool canWater() const override { return type_ == ItemType::WATERING_CAN; }
    bool canFish() const override  { return type_ == ItemType::FISHING_ROD; }
    bool isConsumable() const override;

    void consume() const override;
};