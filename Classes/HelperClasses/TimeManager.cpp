#include "TimeManager.h"
#include "cocos2d.h"
#include "SceneManager.h"
#include "PlayerSprite.h"

TimeManager* TimeManager::instance_ = nullptr;

TimeManager::TimeManager() : current_day_(1), current_time_(6.0f), is_paused_(false) {
    // 从文档中读取游戏天数
    rapidjson::Document* doc = DocumentManager::getInstance()->getDocument("NewUsrArchive.json");

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
}

void TimeManager::pause() {
    is_paused_ = true; // 暂停游戏
}

void TimeManager::resume() {
    is_paused_ = false; // 恢复游戏
}

//有问题
void TimeManager::saveGameData() {
    // 保存游戏数据到 JSON
    rapidjson::Document* doc = DocumentManager::getInstance()->getDocument("NewUsrArchive.json");

    // 确保文档是一个对象
    if (!doc->IsObject()) {
        doc->SetObject();
    }

    auto& allocator = doc->GetAllocator();

    // 更新游戏天数
    rapidjson::Value key_info(rapidjson::kObjectType);
    key_info.AddMember("day", current_day_, allocator);
    key_info.AddMember("money", 1000, allocator); // 假设初始金钱为1000

    // 更新 belongings
    rapidjson::Value belongings(rapidjson::kObjectType);
    belongings.AddMember("axe", rapidjson::Value(rapidjson::kArrayType).Add(1).Add("wood"), allocator);
    belongings.AddMember("sunflower_seed", rapidjson::Value(rapidjson::kArrayType).Add(10), allocator);

    // 更新 relationship
    rapidjson::Value relationship(rapidjson::kObjectType);
    relationship.AddMember("Tom", 0, allocator);
    relationship.AddMember("Alice", 0, allocator);

    // 更新 Map
    rapidjson::Value map(rapidjson::kObjectType);
    rapidjson::Value example(rapidjson::kObjectType);
    rapidjson::Value land(rapidjson::kObjectType);
    rapidjson::Value info(rapidjson::kObjectType);
    info.AddMember("CropName", "Potato", allocator);
    info.AddMember("LiveDay", "1", allocator);
    info.AddMember("MaturationDay", "5", allocator);
    info.AddMember("Water", true, allocator);
    info.AddMember("Fertilizer", 1, allocator);

    land.AddMember("Type", "Land", allocator);
    land.AddMember("Info", info, allocator);
    example.AddMember("12 13", land, allocator);
    map.AddMember("Example", example, allocator);

    // 将所有信息添加到文档
    doc->AddMember("key_info", key_info, allocator);
    doc->AddMember("belongings", belongings, allocator);
    doc->AddMember("relationship", relationship, allocator);
    doc->AddMember("Map", map, allocator);

    // 写入文件
    DocumentManager::getInstance()->writeFile("NewUsrArchive.json", "game_data");
}

void TimeManager::settleAllObjects() {
    // 调用每个地图层中所有物品的 settle 函数
    SceneManager::getInstance()->settle();
}

//有问题
void TimeManager::sleep() {
    if(current_time_>0.0f&& current_time_<6.0f)
        current_time_ = 6.0f;//凌晨睡觉，当天六点起床
    else {
        current_day_++;
        current_time_ = 6.0f;//夜晚睡觉，第二天六点起床
        endOfDay();
    }
    // 恢复主角体力值
    PlayerSprite::getInstance()->setEnergy(100);

    // 保存当前交互地图数据、主角状态和时间
    saveGameData();

    //是否需要切换场景，播放动画
}

void TimeManager::updateTime(float dt) {
    // 每秒钟增加游戏时间
    current_time_ += dt * 24.0f / 600.0f; // 现实10分钟等于游戏1天

    // 判断是否达到2点且主角没有睡觉
    if (current_time_ >= 2.0f && current_time_ < 3.0f) {
        // 假设有一个方法来检查主角是否已经睡觉
        if (!PlayerSprite::getInstance()->isSleeping()) {
            sleep(); // 强制主角睡觉
        }
    }

    if (current_time_ >= 24.0f) {
        endOfDay(); // 如果时间超过24小时，结束一天
    }
}