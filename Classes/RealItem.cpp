#include "RealItem.h"
#include "MainCharacter.h"

static std::string itemTypeToString(ItemType t);

RealItem::RealItem(ItemType t, int q) : type_(t), quantity_(q)
{
    iconPath_ = DocumentManager::getInstance()->getPath(itemTypeToString(t));
}

bool RealItem::isTool() const
{
    switch (type_)
    {
    case ItemType::PICKAXE:
    case ItemType::HOE:
    case ItemType::WATERING_CAN:
    case ItemType::FISHING_ROD:
        return true;
    default:
        return false;
    }
}

bool RealItem::isConsumable() const
{
    switch (type_)
    {
    case ItemType::CAULIFLOWER:
    case ItemType::PUMPKIN:
    case ItemType::POTATO:
    case ItemType::SOUP:
    case ItemType::FISH:
        return true;
    default:
        return false;
    }
}

void RealItem::consume() const
{
    if (MainCharacter::getInstance()->modifyItemQuantity(type_, -1))
    {
        int gain = 0;
        switch (type_)
        {
        case ItemType::CAULIFLOWER: gain = MainCharacter::Eating_cauliflower_gain_energy; break;
        case ItemType::PUMPKIN:     gain = MainCharacter::Eating_pumpkin_gain_energy;     break;
        case ItemType::POTATO:      gain = MainCharacter::Eating_potato_gain_energy;      break;
        case ItemType::SOUP:        gain = MainCharacter::Eating_soup_gain_energy;        break;
        case ItemType::FISH:        gain = MainCharacter::Eating_fish_gain_energy;        break;
        default: break;
        }
        MainCharacter::getInstance()->modifyEnergy(gain);
    }
}


static std::string itemTypeToString(ItemType t)
{
    switch (t)
    {
    case ItemType::CAULIFLOWER_SEED: return "CAULIFLOWER_SEED";
    case ItemType::PUMPKIN_SEED:     return "PUMPKIN_SEED";
    case ItemType::POTATO_SEED:      return "POTATO_SEED";
    case ItemType::CAULIFLOWER:      return "CAULIFLOWER";
    case ItemType::PUMPKIN:          return "PUMPKIN";
    case ItemType::POTATO:           return "POTATO";
    case ItemType::SOUP:             return "SOUP";
    case ItemType::FISH:             return "FISH";
    case ItemType::FERTILIZER:       return "FERTILIZER";
    case ItemType::FISHING_ROD:      return "FISHING_ROD";
    case ItemType::HOE:              return "HOE";
    case ItemType::PICKAXE:          return "PICKAXE";
    case ItemType::WATERING_CAN:     return "WATERING_CAN";
    case ItemType::ROCK:             return "ROCK";
    case ItemType::IRON:             return "IRON";
    case ItemType::NONE:             return "NONE";
    default:                         return "UNKNOWN";
    }
}

ItemType stringToItemType(const std::string& s)
{
    std::string str = s;
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    if (str == "CAULIFLOWER_SEED") return ItemType::CAULIFLOWER_SEED;
    if (str == "PUMPKIN_SEED")     return ItemType::PUMPKIN_SEED;
    if (str == "POTATO_SEED")      return ItemType::POTATO_SEED;
    if (str == "CAULIFLOWER")      return ItemType::CAULIFLOWER;
    if (str == "PUMPKIN")          return ItemType::PUMPKIN;
    if (str == "POTATO")           return ItemType::POTATO;
    if (str == "SOUP")             return ItemType::SOUP;
    if (str == "FISH")             return ItemType::FISH;
    if (str == "FERTILIZER")       return ItemType::FERTILIZER;
    if (str == "FISHING_ROD")      return ItemType::FISHING_ROD;
    if (str == "HOE")              return ItemType::HOE;
    if (str == "PICKAXE")          return ItemType::PICKAXE;
    if (str == "WATERING_CAN")     return ItemType::WATERING_CAN;
    if (str == "ROCK" || str == "STONE") return ItemType::ROCK;
    if (str == "IRON")             return ItemType::IRON;
    if (str == "NONE")             return ItemType::NONE;
    return ItemType::NONE;
}