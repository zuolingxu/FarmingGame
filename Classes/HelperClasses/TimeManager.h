#pragma once

#include <string>
#include <vector>
#include "cocos2d.h"
#include "json/document.h"
#include "MapLayer.h"
#include "PlayerSprite.h"

class TimeManager {
private:
    static TimeManager* instance_;//��������

    //��ʵʱ��10���ӣ���Ϸʱ��һ��24Сʱ
    int current_day_; // ��ǰ��Ϸ����
    float current_time_; // ��ǰʱ�䣨����Ϸʱ��СʱΪ��λ��

    bool is_paused_; // �Ƿ���ͣ
    std::vector<MapLayer*> map_layers_; // �洢���е�ͼ��

    TimeManager();//��json�ж�ȡʱ��
    ~TimeManager();

    void updateTime(float dt);// ������Ϸʱ��
    void settleAllObjects(); // ����������Ʒ�� settle ����

public:
    static TimeManager* getInstance(); // ��ȡ����ʵ��

    //��ui�Ľӿ�
    //��ȡ��ǰʱ��
    int getCurrentDay() const;//��Ϸʱ��ڼ���
    float getCurrentTime() const;//��Ϸʱ��ڼ�Сʱ
    //��ͣ��Ϸ��
    void pause(); // ��ͣ��Ϸ
    void resume(); // �ָ���Ϸ

    //��Ϸ�浽һ������´浵ʱ�����
    void startNewGame();

    //����˯��ʱ�����
    void sleep(); // ˯������

    //�����õ��Ľӿ�
    void endOfDay(); // һ���������������settle����
    void saveGameData(); // ��NewUrsArchive�д浵
};