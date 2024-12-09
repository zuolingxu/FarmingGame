#include "TimeManager.h"
#include "cocos2d.h"
#include "SceneManager.h"
#include "PlayerSprite.h"

TimeManager* TimeManager::instance_ = nullptr;

TimeManager::TimeManager() : current_day_(1), current_time_(6.0f), is_paused_(false) {
    // 从文档中读取游戏天数
    DocumentManager* docManager = DocumentManager::getInstance();
    rapidjson::Document* doc = docManager->getArchiveDocument();

    if (doc->HasMember("key_info") && (*doc)["key_info"].HasMember("day")) {
        current_day_ = (*doc)["key_info"]["day"].GetInt(); // 读取游戏天数
    }
    // 初始化调度器
    cocos2d::Director::getInstance()->getScheduler()->schedule(
        [this](float dt) {
            if (!is_paused_) {
                updateTime(dt);
            }
        },
        this, 1.0f, false, "time_update_key");
}

TimeManager::~TimeManager() {
    // 清理资源
    cocos2d::Director::getInstance()->getScheduler()->unschedule("time_update_key", this);
}

TimeManager* TimeManager::getInstance() {
    if (instance_ == nullptr) {
        instance_ = new TimeManager();
    }
    return instance_;
}

int TimeManager::getCurrentDay() const{
    return current_day_;
}

float TimeManager::getCurrentTime() const {
    return current_time_;
}

void TimeManager::startNewGame() {
    // 启动游戏时的初始化逻辑
    current_day_ = 1;
    current_time_ = 6.0f; // 从早上六点开始
    is_paused_ = false;
}

void TimeManager::endOfDay() {
    // 一天结束时的逻辑
    settleAllObjects(); // 调用所有物品的 settle 函数
    current_day_++; // 增加游戏天数
    current_time_ = 0.0f;

    // 读取当前存档
    DocumentManager* docManager = DocumentManager::getInstance();
    rapidjson::Document* doc = docManager->getArchiveDocument();

    if (doc->HasMember("key_info") && (*doc)["key_info"].HasMember("day")) {
        (*doc)["key_info"]["day"].SetInt(current_day_); // 更新天数
    }
    else {
        // 如果没有找到天数信息，可以选择添加
        rapidjson::Value keyInfo(rapidjson::kObjectType);
        keyInfo.AddMember("day", current_day_, doc->GetAllocator());
        (*doc)["key_info"] = keyInfo; // 添加 key_info
    }
}

void TimeManager::pause() {
    is_paused_ = true; // 暂停游戏
}

void TimeManager::resume() {
    is_paused_ = false; // 恢复游戏
}

void TimeManager::saveGameData() {
    // 获取 DocumentManager 实例
    DocumentManager* docManager = DocumentManager::getInstance();
    docManager->saveArchiveDocument();
}

void TimeManager::settleAllObjects() {
    // 调用每个地图层中所有物品的 settle 函数
    SceneManager::getInstance()->settle();
}

//更改
void TimeManager::sleep() {
    if(current_time_>=0.0f&& current_time_<6.0f)
        current_time_ = 6.0f;//凌晨睡觉，当天六点起床
    else {
        endOfDay();
        current_time_ = 6.0f;//夜晚睡觉，第二天六点起床
    }
    //更改
    // 恢复主角体力值
    //PlayerSprite::getInstance()->setEnergy(100);

    // 保存当前交互地图数据、主角状态和时间
    saveGameData();

    //更改
    //是否需要切换场景，播放动画
}

//更改
void TimeManager::updateTime(float dt) {
    // 每秒钟增加游戏时间
    current_time_ += dt * 24.0f / (float)(60 * MinutesOfOneDay);

    // 判断是否达到2点且主角没有睡觉
    /*if (current_time_ >= 2.0f && current_time_ < 3.0f) {
        // 假设有一个方法来检查主角是否已经睡觉
        if (!PlayerSprite::getInstance()->isSleeping()) {
            sleep(); // 强制主角睡觉
        }
    }*/

    if (current_time_ >= 24.0f) {
        endOfDay(); // 如果时间超过24小时，结束一天
    }

    // 调试使用
    // logCurrentTime();
}