#pragma once
#pragma once
#include "cocos2d.h"
#include "MapObject.h"
#include "DocumentManager.h"
#include "json/document.h"
#include "MapLayer.h"

class Mineral : public MapObject {
private:

    MapLayer* parent;


    bool isMined;  // �Ƿ��ѱ��ھ�
    std::string mineralType;  // ��ʯ����


public:
    // ���캯��
    explicit Mineral(const Vec<int>& pos ,MapLayer* parent, bool im,const std::string& type);

    // ��������
    ~Mineral() override;

    // ������ʯʵ�����Ӵ浵��ȡ��
    static MapObject* create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos);

    // ��ʼ����ʯ��������ض�Ӧ����Դ��
    void init() override;

    // ��ҽ������ھ��ʯ��
    void interact() override;

    // �����ʯ��������Դ��
    void clear() override;

    // ��ͣ
    void pause() override;

    // �ָ�
    void resume() override;

    void settle() override;

    // �Ƿ���������������ײ
    bool hasCollision() override;

    // �����ʯ�ھ��Ĵ浵����
    void update_ismined_Archive(const Vec<int>& position, bool isMinedValue);
};

