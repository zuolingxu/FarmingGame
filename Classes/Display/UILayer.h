#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

// ���� UI ����ö��
enum class UILayerType {
    START_SCREEN,
    BAG,
    TASK_BAR
};

class UILayer : public cocos2d::Ref
{
public:
    struct Item {
        std::string name;      // ��Ʒ����
        int quantity;          // ��Ʒ����
        std::string iconPath;  // ��Ʒͼ��·��

        Item(const std::string& itemName = "", int num = 0, const std::string& icon = "")
            : name(itemName), quantity(num), iconPath(icon) {}
    };
    // ���� UILayer ʵ��
    static cocos2d::Node* createUILayer(UILayerType type);

    // ��ʼ�� UILayer
    bool initWithType(UILayerType type);

    // ��ȡ UI Ԫ�أ��米��������������
    cocos2d::ui::Layout* getBagLayout() const { return bagLayout_; }
    cocos2d::ui::Layout* getTaskBarLayout() const { return taskBarLayout_; }
    cocos2d::ui::Layout* getStartScreenLayout() const { return startScreenLayout_; }
        
    // TODO: Manufacture
    // TODO: BagLayer
    // TODO: ListLayer

private:
    // UI ���
    cocos2d::ui::Layout* taskBarLayout_;
    cocos2d::ui::Layout* bagLayout_;
    cocos2d::ui::Layout* startScreenLayout_;

    // �������� UI ����ķ���
    void createTaskBarLayout();
    void createBagLayout();
    void createStartScreenLayout();
};
