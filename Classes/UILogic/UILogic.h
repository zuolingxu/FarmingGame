#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
#include <vector>
#include "HelperClasses.h"

// �򵥵���Ʒ�ṹ�壬��UILayer���屣��һ��


// �򵥵�����ṹ��
struct Task {
    std::string description;
    bool completed;
    Task(const std::string& desc = "", bool done = false)
        : description(desc), completed(done) {}
};


class UILogic : public cocos2d::Ref
{
public:
    // ��ȡ����ʵ��
    static UILogic* getInstance();

    // ���ÿ�ʼ����ڵ㣨START_SCREEN���ͣ���Node��UILayer����
    void initStartScreenNode(cocos2d::Node* startScreenNode);

    // ���ñ����ڵ㣨BAG���ͣ���Node��UILayer����
    void initBagNode(cocos2d::Node* bagNode);

    // �����������ڵ㣨TASK_BAR���ͣ���Node��UILayer����
    void initTaskBarNode(cocos2d::Node* taskBarNode);

    // ˢ�±���UI
    void refreshBagUI();

    // ˢ������UI
    void updateTaskUI();

    // ������Ʒ������
    void addItemToBag(const Item& item);

    // ʹ����Ʒ/�Ƴ���Ʒ
    // �ڵ������ʱ����
    void useItemFromBag(int slotIndex);

    //// ����������
    void completeTask(int taskIndex);

    // ������Ҫ�� DocumentManager ����/���汳������������
    void loadDataFromSave();
    void saveDataToSave();

private:
    UILogic();
    ~UILogic();

    // ��ʼ���¼��󶨺���
    void bindStartScreenEvents();
    void bindBagEvents();
    void bindTaskBarEvents();

     //��ť����¼��ص�����
    void onNewButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void onLoadButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void onExitButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onCloseBagButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void onBagSlotClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onTaskItemClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    static UILogic* instance_;

    cocos2d::Node* startScreenNode_;
    cocos2d::Node* bagNode_;
    cocos2d::Node* taskBarNode_;

    std::vector<Item> bagItems_;
    std::vector<Task> tasks_;

};



