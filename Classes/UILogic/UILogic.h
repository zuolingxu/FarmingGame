#pragma once
#include "cocos2d.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <map>

class Character {
public:
    // ��Ʒ����ö��
    enum class ItemType {
        FERTILIZER,
        SEED,
        WATERING_CAN,
        NONE
    };

    // ��Ʒ�ṹ
    struct Item {
        ItemType type;
        std::string name;
    };

private:
    std::vector<Item> inventory; // ������е���Ʒ
    static Character* instance;   // ����ʵ��

    // ˽�й��캯������ֹ�ⲿʵ����
    Character() {
        // �������������һЩ��ʼ������
    }

public:
    // ��ȡ����ʵ��
    static Character* getInstance() {
        if (!instance) {
            instance = new Character(); // ��̬�ڴ����
        }
        return instance;
    }

    void addItem(const Item& item) {
        inventory.push_back(item);
    }

    const std::vector<Item>& getInventory() const {
        return inventory;
    }

    bool hasItem(ItemType type) const {
        for (const auto& item : inventory) {
            if (item.type == type) {
                return true;
            }
        }
        return false;
    }

    // ������ʵ������ѡ��
    static void cleanup() {
        delete instance; // �ͷŶ�̬������ڴ�
        instance = nullptr;
    }

    // ��ֹ��������͸�ֵ����
    Character(const Character&) = delete;
    Character& operator=(const Character&) = delete;

    // �����Ҫ֧���ƶ���������������ƶ����캯�����ƶ���ֵ�����
    // Character(Character&&) = default;
    // Character& operator=(Character&&) = default;

    // ������������Ϊ˽�У���ֹ�ⲿɾ��
    ~Character() {
        // �������������һЩ�������
    }
};
