#pragma once

#include <string>
#include <vector>
#include "cocos2d.h"
#include "json/document.h"
#include "MapLayer.h"
#include "PlayerSprite.h"

class TimeManager {
private:
    const int SPRING_DAYS = 10;
    const int AUTUMN_DAYS = 10;
    const float MinutesOfOneDay = 10; // ��ʵMinuteOfOneDay���ӵ�����Ϸ1��

    static TimeManager* instance_;//��������

    int current_day_; // ��ǰ��Ϸ����
    float current_time_; // ��ǰʱ�䣨����Ϸʱ��СʱΪ��λ��
    std::string season;

    bool is_paused_; // �Ƿ���ͣ
    std::vector<MapLayer*> map_layers_; // �洢���е�ͼ��

    TimeManager();//��json�ж�ȡʱ��
    ~TimeManager();

    void updateTime(float dt);// ������Ϸʱ��
    void settleAllObjects(); // ����������Ʒ�� settle ����

public:
    static TimeManager* getInstance(); // ��ȡ����ʵ�� ��void SceneManager::createMaps()�����ã���ʼ��ʱ


    float getCurrentTime() const { return current_time_; }
    int getCurrentDay() const { return current_day_; }
    std::string getSeason()const { return season; }
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

    //����ʹ��
    /*void logCurrentTime() {
        CCLOG("Current Day: %d", current_day_);
        CCLOG("Current Time: %.1f", current_time_);
    }*/
};