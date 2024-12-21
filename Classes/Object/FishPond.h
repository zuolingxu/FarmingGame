//#pragma once
//#include "cocos2d.h"
//#include "DocumentManager.h"
//#include "MapObject.h"
//#include "json/document.h"
//#include "MapLayer.h"
//#include"PlayerSprite.h"
//#include "MainCharacter.h"
//
//class FishPond final : protected MapObject {
//private:
//	MapLayer* parent_;
//	float Probility;
//    bool isFishing;
//    bool ifCaught();
//public:
//    explicit FishPond(MapLayer* parent, const Vec<int>& pos) {
//
//    };
//	~FishPond() override {};
//	static MapObject* create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos);
//	virtual void interact() override;
//	virtual bool hasCollision() override;
//    void stopFishing();
//    void startFishing();
//    bool isCurrentlyFishing();
//};
//
//
//
////改变状态，记录钓鱼时间
//void FishPond::stopFishing()
//{
//    isFishing = false;
//    if (ifCaught()) {
//        //鱼加一
//    }
//    else {
//
//    }
//}
//
//// 停止钓鱼
//void FishPond::stopFishing() {
//    isFishing = true;
//};
//
////开始钓鱼
//void FishPond::startFishing() {
//    isFishing = false;
//};
//
//bool FishPond::isCurrentlyFishing() {
//    return isFishing;
//}
//
////判断是否钓到鱼了
//bool FishPond::ifCaught() {
//    std::random_device rd;
//    std::default_random_engine engine(rd());
//    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
//
//    float chance = distribution(engine);
//    return chance < Probility;
//};
//
//
//bool FishPond::hasCollision() {
//    return false;
//}