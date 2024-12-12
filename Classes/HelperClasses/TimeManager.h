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
    const float MinutesOfOneDay = 10; // 现实MinuteOfOneDay分钟等于游戏1天

    static TimeManager* instance_;//单例函数

    int current_day_; // 当前游戏天数
    float current_time_; // 当前时间（以游戏时间小时为单位）
    std::string season;

    bool is_paused_; // 是否暂停
    std::vector<MapLayer*> map_layers_; // 存储所有地图层

    TimeManager();//从json中读取时间
    ~TimeManager();

    void updateTime(float dt);// 更新游戏时间
    void settleAllObjects(); // 调用所有物品的 settle 函数

public:
    static TimeManager* getInstance(); // 获取单例实例 在void SceneManager::createMaps()被调用，开始计时


    float getCurrentTime() const { return current_time_; }
    int getCurrentDay() const { return current_day_; }
    std::string getSeason()const { return season; }
    //暂停游戏等
    void pause(); // 暂停游戏
    void resume(); // 恢复游戏

    //游戏玩到一半加载新存档时请调用
    void startNewGame();

    //主角睡觉时请调用
    void sleep(); // 睡觉功能

    //可能用到的接口
    void endOfDay(); // 一天结束，调用所有settle（）
    void saveGameData(); // 在NewUrsArchive中存档

    //调试使用
    /*void logCurrentTime() {
        CCLOG("Current Day: %d", current_day_);
        CCLOG("Current Time: %.1f", current_time_);
    }*/
};