#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include<vector>
#include<string>

// ���� UI ����ö��
enum class UILayerType {
    START_SCREEN,
    BAG,
    TASK_BAR,
    NPC,
    LOAD_ARCHIVE,
    TIME,
    SHOP,
    MANUFACTURE
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
    cocos2d::ui::Layout* getnpcLayout() const { return npcLayout_; }
    cocos2d::ui::Layout* getloadArchiveLayout() const { return loadArchiveLayout_; }
    cocos2d::ui::Layout* getTimeLayout() const { return timeLayout_; }
    cocos2d::ui::Layout* getShopLayout() const { return shopLayout_; }
    cocos2d::ui::Layout* getManufactureLayout() const { return manufactureLayout_; }

private:
    // UI ���
    cocos2d::ui::Layout* taskBarLayout_;
    cocos2d::ui::Layout* bagLayout_;
    cocos2d::ui::Layout* startScreenLayout_;
    cocos2d::ui::Layout* npcLayout_;
    cocos2d::ui::Layout* loadArchiveLayout_;
    cocos2d::ui::Layout* timeLayout_;
    cocos2d::ui::Layout* shopLayout_;
    cocos2d::ui::Layout* manufactureLayout_;
    std::vector<std::string> Sentence;

    // �������� UI ����ķ���
    void createTaskBarLayout();
    void createBagLayout();
    void createStartScreenLayout();
    void createNpcLayout();
    void createLoadArchiveLayout();
    void createTimeLayout();
    void createShopLayout();
    void createManufactureLayout();

    void initSentence();

};
