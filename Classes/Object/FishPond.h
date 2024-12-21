#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "MapObject.h"
#include "json/document.h"
#include "MapLayer.h"
#include"PlayerSprite.h"
#include "MainCharacter.h"

class FishPond {
protected:
    float successRate;   // ����ɹ���
    bool isFishing;// �Ƿ��ڵ���
    bool ifCaught();

public:
    FishPond(std::string fishType, float successRate = 0.5f)
        : isFishing(false), successRate(successRate){}

    // ֹͣ����
    void stopFishing();
    // ��ȡ����״̬
    bool isCurrentlyFishing();
    //��ʼ����
    void startFishing();
};

