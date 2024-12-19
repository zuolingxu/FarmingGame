#include "TimeManager.h"
#include "cocos2d.h"
#include "SceneManager.h"
#include "PlayerSprite.h"
#include "MainCharacter.h"

TimeManager* TimeManager::instance_ = nullptr;

//todo xianshishijian
TimeManager::TimeManager() : current_day_(1), current_time_(6.0f), is_paused_(false) {
    // ���ĵ��ж�ȡ��Ϸ����
    DocumentManager* docManager = DocumentManager::getInstance();
    rapidjson::Document* doc = docManager->getArchiveDocument();

    if (doc->HasMember("key_info") && (*doc)["key_info"].HasMember("day")) {
        current_day_ = (*doc)["key_info"]["day"].GetInt(); // ��ȡ��Ϸ����
    }
    if (doc->HasMember("key_info") && (*doc)["key_info"].HasMember("season")) {
        season = (*doc)["key_info"]["season"].GetString(); // ��ȡ��Ϸ����
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
    if (1 <= (current_day_ % 20) && (current_day_ % 20) <= 10)
        season = "spring";
    else
        season = "autumn";

    // ��ȡ��ǰ�浵
    DocumentManager* docManager = DocumentManager::getInstance();
    rapidjson::Document* doc = docManager->getArchiveDocument();

    if (doc->HasMember("key_info") && (*doc)["key_info"].HasMember("day")) {
        (*doc)["key_info"]["day"].SetInt(current_day_); // ��������
    }
    else {
        // ���û���ҵ�������Ϣ������ѡ�����
        rapidjson::Value keyInfo(rapidjson::kObjectType);
        keyInfo.AddMember("day", current_day_, doc->GetAllocator());
        (*doc)["key_info"] = keyInfo; // ��� key_info
    }
    if (doc->HasMember("key_info") && (*doc)["key_info"].HasMember("season")) {
        // ����Ѿ����� "season" ����������
        (*doc)["key_info"]["season"].SetString(season.c_str(), doc->GetAllocator()); // ���� const char* �ͷ�����
    }
    else {
        // ���û�� "season" ������� "season"
        (*doc)["key_info"].AddMember("season", rapidjson::Value(season.c_str(), doc->GetAllocator()), doc->GetAllocator());
    }
}

void TimeManager::pause() {
    is_paused_ = true; // ��ͣ��Ϸ
}

void TimeManager::resume() {
    is_paused_ = false; // �ָ���Ϸ
}

//archieve memory->disk
void TimeManager::saveGameData() {
    // ��ȡ DocumentManager ʵ��
    DocumentManager* docManager = DocumentManager::getInstance();
    docManager->saveArchiveDocument();
}

void TimeManager::settleAllObjects() {
    // ����ÿ����ͼ����������Ʒ�� settle ����
    SceneManager::getInstance()->settle();
}

// todo interact with bed callback this function
// todo donghua
void TimeManager::sleep() {
    if(current_time_>=0.0f&& current_time_<6.0f)
        current_time_ = 6.0f;//�賿˯��������������
    else {
        endOfDay();
        current_time_ = 6.0f;//ҹ��˯�����ڶ���������
    }

    // �ָ���������ֵ
    MainCharacter::getInstance()->modifyEnergy(MainCharacter::getInstance()->MAX_ENERGY);

    // save maincharacter data in acrhive in memory
    MainCharacter::getInstance()->change_archive_in_memory();

    //archieve memory->disk
    saveGameData();

    // ��˯�����ڵ����������֮ǰ
    // �𴲶������������֮��
}

//����
 //Todo shuaxinshijian
void TimeManager::updateTime(float dt) {
    int pre_current_time_ = (int)current_time_;

    // ÿ����������Ϸʱ��
    current_time_ += dt * 24.0f / (float)(60 * MinutesOfOneDay);

    if (pre_current_time_ != (int)current_time_) {
        //Todo shuaxinshijian
    }

    // �ж��Ƿ�ﵽ2��
    if (current_time_ >= 2.0f && current_time_ < 3.0f) {
        // ����������Ƿ��Ѿ�˯��
        // todo
        // ����˲�ƶ���
        // ������˯����
        SceneManager::getInstance()->NextMap("player_house", "15 2");
        sleep(); // ǿ������˯��
        // �����𴲶���
    }
    // never change sequence with previous judge
    if (current_time_ >= 24.0f) {
        endOfDay(); // ���ʱ�䳬��24Сʱ������һ��
    }

    // ����ʹ��
    // logCurrentTime();
}

