#pragma once
#include "cocos2d.h"
#include <string>
#include <vector>
#include "HelperClasses.h"

USING_NS_CC;

class MainCharacter {

private:
    std::vector<Item> inventory;       // ������Ʒ
    Item* currentItem;                 // ��ǰ���е���Ʒ��ֻ����һ����
    static MainCharacter* instance;    // ����ʵ��

    // �洢 ItemType ���������Ƶ�ӳ��
    static std::unordered_map<ItemType, std::string> itemTypeToName;

    // ���캯��˽�У���ֹ�ⲿֱ�Ӵ���ʵ��
    MainCharacter();

public:
    // ��ȡ����ʵ��
    static MainCharacter* getInstance();

    // �����Ʒ������
    void addItem(const Item& item);

    // ��ȡ�����е�������Ʒ
    const std::vector<Item>& getInventory() const;

    // ��鱳�����Ƿ���ָ�����͵���Ʒ
    bool hasItem(ItemType type) const;

    // ���õ�ǰ���е���Ʒ
    void setCurrentItem(ItemType type);

    // ��ȡ��ǰ������Ʒ
    const Item* getCurrentItem() const;

    // �޸ı�����ָ����Ʒ������ +n ���� -n
    bool modifyItemQuantity(ItemType type, int delta);

    // ������ʵ��
    static void cleanup();

    // ��ֹ���ƺ͸�ֵ
    MainCharacter(const MainCharacter&) = delete;
    MainCharacter& operator=(const MainCharacter&) = delete;

    // ��������
    ~MainCharacter();
};
