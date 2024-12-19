#include "TimeManager.h"
#include "cocos2d.h"
#include "SceneManager.h"
#include "PlayerSprite.h"
#include "MainCharacter.h"

TimeManager* TimeManager::instance_ = nullptr;

//todo xianshishijian
TimeManager::TimeManager() : current_day_(1), current_time_(6.0f), is_paused_(false) {
    // 从文档中读取游戏天数
    DocumentManager* docManager = DocumentManager::getInstance();
    rapidjson::Document* doc = docManager->getArchiveDocument();

    if (doc->HasMember("key_info") && (*doc)["key_info"].HasMember("day")) {
        current_day_ = (*doc)["key_info"]["day"].GetInt(); // 读取游戏天数
    }
    if (doc->HasMember("key_info") && (*doc)["key_info"].HasMember("season")) {
        season = (*doc)["key_info"]["season"].GetString(); // 读取游戏季节
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
    if (1 <= (current_day_ % 20) && (current_day_ % 20) <= 10)
        season = "spring";
    else
        season = "autumn";

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
    if (doc->HasMember("key_info") && (*doc)["key_info"].HasMember("season")) {
        // 如果已经存在 "season" 键，更新它
        (*doc)["key_info"]["season"].SetString(season.c_str(), doc->GetAllocator()); // 传递 const char* 和分配器
    }
    else {
        // 如果没有 "season" 键，添加 "season"
        (*doc)["key_info"].AddMember("season", rapidjson::Value(season.c_str(), doc->GetAllocator()), doc->GetAllocator());
    }
}

void TimeManager::pause() {
    is_paused_ = true; // 暂停游戏
}

void TimeManager::resume() {
    is_paused_ = false; // 恢复游戏
}

//archieve memory->disk
void TimeManager::saveGameData() {
    // 获取 DocumentManager 实例
    DocumentManager* docManager = DocumentManager::getInstance();
    docManager->saveArchiveDocument();
}

void TimeManager::settleAllObjects() {
    // 调用每个地图层中所有物品的 settle 函数
    SceneManager::getInstance()->settle();
}

// todo interact with bed callback this function
// todo donghua
void TimeManager::sleep() {
    if(current_time_>=0.0f&& current_time_<6.0f)
        current_time_ = 6.0f;//凌晨睡觉，当天六点起床
    else {
        endOfDay();
        current_time_ = 6.0f;//夜晚睡觉，第二天六点起床
    }

    // 恢复主角体力值
    MainCharacter::getInstance()->modifyEnergy(MainCharacter::getInstance()->MAX_ENERGY);

    // save maincharacter data in acrhive in memory
    MainCharacter::getInstance()->change_archive_in_memory();

    //archieve memory->disk
    saveGameData();

    // 入睡动画在调用这个函数之前
    // 起床动画在这个函数之后
}

//更改
 //Todo shuaxinshijian
void TimeManager::updateTime(float dt) {
    int pre_current_time_ = (int)current_time_;

    // 每秒钟增加游戏时间
    current_time_ += dt * 24.0f / (float)(60 * MinutesOfOneDay);

    if (pre_current_time_ != (int)current_time_) {
        //Todo shuaxinshijian
    }

    // 判断是否达到2点
    if (current_time_ >= 2.0f && current_time_ < 3.0f) {
        // 不检查主角是否已经睡觉
        // todo
        // 加入瞬移动画
        // 加入入睡动画
        SceneManager::getInstance()->NextMap("player_house", "15 2");
        sleep(); // 强制主角睡觉
        // 加入起床动画
    }
    // never change sequence with previous judge
    if (current_time_ >= 24.0f) {
        endOfDay(); // 如果时间超过24小时，结束一天
    }

    // 调试使用
    // logCurrentTime();
}

