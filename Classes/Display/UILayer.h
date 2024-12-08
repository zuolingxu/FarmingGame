<<<<<<< Updated upstream
//#pragma once
//#include "cocos2d.h"
//#include "ui/CocosGUI.h"
//
//class UILayer : public cocos2d::Layer
//{
//public:
//    struct Item {
//        std::string name;      // ��Ʒ����
//        int quantity;          // ��Ʒ����
//        std::string iconPath;  // ��Ʒͼ��·��
//
//        Item(const std::string& itemName = "", int num = 0, const std::string& icon = "")
//            : name(itemName), quantity(num), iconPath(icon) {}
//    };
//    // ���� UILayer ʵ��
//    static UILayer* createUILayer();
//
//    // ��ʼ�� UILayer
//    virtual bool init() override;
//
//    // ��ȡ UI Ԫ�أ��米��������������
//    cocos2d::ui::Layout* getBagLayout() const { return bagLayout_; }
//    cocos2d::ui::Layout* getTaskBarLayout() const { return taskBarLayout_; }
//    cocos2d::ui::Layout* getStartScreenLayout() const { return startScreenLayout_; }
//    cocos2d::ui::Layout* getToolBarLayout() const { return toolBarLayout_; }
//
//    // ˢ�±�������
//    void refreshBagItems(const std::vector<Item>& items);
//
//    // ��ʾ������ UI Ԫ��
//    void showBag();
//    void hideBag();
//    void showStartScreen();
//    void hideStartScreen();
//
//private:
//    // UI ���
//    cocos2d::ui::Layout* taskBarLayout_;
//    cocos2d::ui::Layout* bagLayout_;
//    cocos2d::ui::Layout* startScreenLayout_;
//    cocos2d::ui::Layout* toolBarLayout_; // ����������
//
//    // �������� UI ����ķ���
//    void createTaskBarLayout();
//    void createBagLayout();
//    void createStartScreenLayout();
//    void createToolBarLayout();
//};
=======
#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

// ���� UI ����ö��
enum class UILayerType {
    START_SCREEN,
    BAG,
    TASK_BAR,
    TOOL_BAR
};

class UILayer : public cocos2d::Layer
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
    static Node* createUILayer(UILayerType type);

    // ��ʼ�� UILayer
    bool initWithType(UILayerType type);

    // ��ȡ UI Ԫ�أ��米��������������
    cocos2d::ui::Layout* getBagLayout() const { return bagLayout_; }
    cocos2d::ui::Layout* getTaskBarLayout() const { return taskBarLayout_; }
    cocos2d::ui::Layout* getStartScreenLayout() const { return startScreenLayout_; }
    cocos2d::ui::Layout* getToolBarLayout() const { return toolBarLayout_; }

    // ˢ�±�������
    void refreshBagItems(const std::vector<Item>& items);

    // ��ʾ������ UI Ԫ��
    void showBag();
    void hideBag();
    void showStartScreen();
    void hideStartScreen();
private:
    // UI ���
    cocos2d::ui::Layout* taskBarLayout_;
    cocos2d::ui::Layout* bagLayout_;
    cocos2d::ui::Layout* startScreenLayout_;
    cocos2d::ui::Layout* toolBarLayout_; // ����������

    // �������� UI ����ķ���
    void createTaskBarLayout();
    void createBagLayout();
    void createStartScreenLayout();
    void createToolBarLayout();
};
>>>>>>> Stashed changes
