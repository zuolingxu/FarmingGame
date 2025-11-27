#pragma once

#include "Item.h"

class NullItem final : public Item
{
private:
    NullItem() = default;

public:
    static NullItem* getInstance()
    {
        static NullItem instance;
        return &instance;
    }

    ItemType    getType() const override     { return ItemType::NONE; }
    std::string getIconPath() const override { return ""; }

    bool isNone() const override { return true; }

    void use() const override
    {
        // Optional: play empty-hand click sound
        // AudioEngine::play2d("sound/empty_click.wav");
        CCLOG("Nothing in hand");
    }
};