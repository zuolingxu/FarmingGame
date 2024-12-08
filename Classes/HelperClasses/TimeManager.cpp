#include "TimeManager.h"
#include "cocos2d.h"
#include "SceneManager.h"
#include "PlayerSprite.h"

TimeManager* TimeManager::instance_ = nullptr;

TimeManager::TimeManager() : current_day_(1), current_time_(6.0f), is_paused_(false) {
    // ���ĵ��ж�ȡ��Ϸ����
    rapidjson::Document* doc = DocumentManager::getInstance()->getDocument("NewUsrArchive.json");

    if (doc->HasMember("key_info") && (*doc)["key_info"].HasMember("day")) {
        current_day_ = (*doc)["key_info"]["day"].GetInt(); // ��ȡ��Ϸ����
    }

    // ��ʼ��������
    cocos2d::Director::getInstance()->getScheduler()->schedule(
        [this](float dt) {
            if (!is_paused_) {
                updateTime(dt);
            }
        },
        this, 1.0f, false, "time_update_key");
}

TimeManager::~TimeManager() {
    // ������Դ
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
    // ������Ϸʱ�ĳ�ʼ���߼�
    current_day_ = 1;
    current_time_ = 6.0f; // ���������㿪ʼ
    is_paused_ = false;
}


void TimeManager::endOfDay() {
    // һ�����ʱ���߼�
    settleAllObjects(); // ����������Ʒ�� settle ����
    current_day_++; // ������Ϸ����
    current_time_ = 0.0f;
}

void TimeManager::pause() {
    is_paused_ = true; // ��ͣ��Ϸ
}

void TimeManager::resume() {
    is_paused_ = false; // �ָ���Ϸ
}

//������
void TimeManager::saveGameData() {
    // ������Ϸ���ݵ� JSON
    rapidjson::Document* doc = DocumentManager::getInstance()->getDocument("NewUsrArchive.json");

    // ȷ���ĵ���һ������
    if (!doc->IsObject()) {
        doc->SetObject();
    }

    auto& allocator = doc->GetAllocator();

    // ������Ϸ����
    rapidjson::Value key_info(rapidjson::kObjectType);
    key_info.AddMember("day", current_day_, allocator);
    key_info.AddMember("money", 1000, allocator); // �����ʼ��ǮΪ1000

    // ���� belongings
    rapidjson::Value belongings(rapidjson::kObjectType);
    belongings.AddMember("axe", rapidjson::Value(rapidjson::kArrayType).Add(1).Add("wood"), allocator);
    belongings.AddMember("sunflower_seed", rapidjson::Value(rapidjson::kArrayType).Add(10), allocator);

    // ���� relationship
    rapidjson::Value relationship(rapidjson::kObjectType);
    relationship.AddMember("Tom", 0, allocator);
    relationship.AddMember("Alice", 0, allocator);

    // ���� Map
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

    // ��������Ϣ��ӵ��ĵ�
    doc->AddMember("key_info", key_info, allocator);
    doc->AddMember("belongings", belongings, allocator);
    doc->AddMember("relationship", relationship, allocator);
    doc->AddMember("Map", map, allocator);

    // д���ļ�
    DocumentManager::getInstance()->writeFile("NewUsrArchive.json", "game_data");
}

void TimeManager::settleAllObjects() {
    // ����ÿ����ͼ����������Ʒ�� settle ����
    SceneManager::getInstance()->settle();
}

//������
void TimeManager::sleep() {
    if(current_time_>0.0f&& current_time_<6.0f)
        current_time_ = 6.0f;//�賿˯��������������
    else {
        current_day_++;
        current_time_ = 6.0f;//ҹ��˯�����ڶ���������
        endOfDay();
    }
    // �ָ���������ֵ
    PlayerSprite::getInstance()->setEnergy(100);

    // ���浱ǰ������ͼ���ݡ�����״̬��ʱ��
    saveGameData();

    //�Ƿ���Ҫ�л����������Ŷ���
}

void TimeManager::updateTime(float dt) {
    // ÿ����������Ϸʱ��
    current_time_ += dt * 24.0f / 600.0f; // ��ʵ10���ӵ�����Ϸ1��

    // �ж��Ƿ�ﵽ2��������û��˯��
    if (current_time_ >= 2.0f && current_time_ < 3.0f) {
        // ������һ����������������Ƿ��Ѿ�˯��
        if (!PlayerSprite::getInstance()->isSleeping()) {
            sleep(); // ǿ������˯��
        }
    }

    if (current_time_ >= 24.0f) {
        endOfDay(); // ���ʱ�䳬��24Сʱ������һ��
    }
}