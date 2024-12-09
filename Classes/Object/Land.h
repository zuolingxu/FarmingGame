#pragma once
#include "cocos2d.h"
#include "Crop.h"
#include "DocumentManager.h"
#include "Object.h"
#include "json/document.h"
#include "MapLayer.h"
#include "UILogic.h"

class Land final: protected ::Object {
    private:
	Crop* crop_ = nullptr;  // 指向作物对象
    MapLayer* parent_;      // 父地图层

    bool Water;
    bool Fertilizer;

    public:
	explicit Land(MapLayer* parent);
    ~Land() override;
    static Object* create(rapidjson::Value& val, MapLayer* parent);//从存档创建耕地
    static Object* createByPlayer(const cocos2d::Vec2& position, MapLayer* parent); // 玩家创建耕地
    virtual void init() override;
    virtual void interact() override;
    virtual void clear() override;
    virtual void pause() override;
    virtual void resume() override;
    virtual void settle() override;
    virtual bool hasCollision() override;

    void saveToArchive(const cocos2d::Vec2& position); // 保存到存档的方法（内存）

    // Getter 方法
    bool isWatered() const { return Water; }
    bool isFertilized() const { return Fertilizer; }
    void setWater(bool water) { Water = water; }
    void setFertilizer(bool fertilizer) { Fertilizer = fertilizer; }
}; 