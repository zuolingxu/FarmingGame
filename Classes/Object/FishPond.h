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
    float successRate;   // 钓鱼成功率
    bool isFishing;// 是否在钓鱼
    bool ifCaught();

public:
    FishPond(std::string fishType, float successRate = 0.5f)
        : isFishing(false), successRate(successRate){}

    // 停止钓鱼
    void stopFishing();
    // 获取钓鱼状态
    bool isCurrentlyFishing();
    //开始钓鱼
    void startFishing();
};

