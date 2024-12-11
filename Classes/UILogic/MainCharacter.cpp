#include "MainCharacter.h"

// ��ʼ����̬��Ա
MainCharacter* MainCharacter::instance = nullptr;


MainCharacter::MainCharacter() : currentItem(nullptr) {
    // �����ڴ˴���ʼ���κ���Ҫ�ĳ�Ա
}

MainCharacter* MainCharacter::getInstance() {
    if (!instance) {
        instance = new MainCharacter();
    }
    return instance;
}

void MainCharacter::addItem(const Item& item) {
    inventory.push_back(item);
}

const std::vector<MainCharacter::Item>& MainCharacter::getInventory() const {
    return inventory;
}

bool MainCharacter::hasItem(ItemType type) const {
    for (const auto& item : inventory) {
        if (item.type == type) {
            return true;
        }
    }
    return false;
}

void MainCharacter::setCurrentItem(ItemType type) {
    for (auto& item : inventory) {
        if (item.type == type) {
            currentItem = &item; // ���õ�ǰ���е���Ʒ
            return;
        }
    }
    currentItem = nullptr; // ���û���ҵ���Ӧ���͵���Ʒ������Ϊ nullptr
}

const MainCharacter::Item* MainCharacter::getCurrentItem() const {
    return currentItem;
}

bool MainCharacter::modifyItemQuantity(ItemType type, int delta) {
    // ���deltaΪ0��ֱ�ӷ���
    if (delta == 0) return 1;  // 1 ��ʾ�����ɹ�������û�б仯

    // ���������е���Ʒ���ҵ�ָ�����͵���Ʒ
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->type == type) {
            // �޸���Ʒ����
            if (it->quantity + delta < 0) {
                return 0;  // ����޸ĺ������С���㣬���ش���
            }

            it->quantity += delta;

            // �����Ʒ������Ϊ�����٣�ɾ������Ʒ
            if (it->quantity == 0) {
                inventory.erase(it);
            }
            return 1;  // �����ɹ�
        }
    }

    // ���û���ҵ�����Ʒ���ͣ��� delta �����㣬��Ӹ���Ʒ������
    if (delta > 0) {
        inventory.push_back(Item(type, delta));
        return 1;  // �����Ʒ�ɹ�
    }

    return 0;  // ���û���ҵ���Ʒ���� delta Ϊ�������ش���
}


void MainCharacter::cleanup() {
    delete instance;
    instance = nullptr;
}

MainCharacter::~MainCharacter() 
{

}
