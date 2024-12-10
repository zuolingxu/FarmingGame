#pragma once
#include "cocos2d.h"
#include "Crop.h"
#include "DocumentManager.h"
#include "MapObject.h"
#include "json/document.h"
#include "MapLayer.h"
#include "UILogic.h"

class Crop;

class Land final: protected MapObject {
    private:
	Crop* crop_ = nullptr;  // ָ���������
    MapLayer* parent_;      // ����ͼ��

    bool Water;
    bool Fertilizer;

    public:
	explicit Land(MapLayer* parent, const Vec<int>& pos);
    ~Land() override;
    static MapObject* create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos);//�Ӵ浵��������
    static MapObject* createByPlayer(const cocos2d::Vec2& position, MapLayer* parent); // ��Ҵ�������
    virtual void init() override;
    virtual void interact() override;
    virtual void clear() override;
    virtual void pause() override;
    virtual void resume() override;
    virtual void settle() override;
    virtual bool hasCollision() override;

    void saveToArchive(const cocos2d::Vec2& position); // ���浽�浵�ķ������ڴ棩

    // Getter ����
    bool isWatered() const { return Water; }
    bool isFertilized() const { return Fertilizer; }
    void setWater(bool water) { Water = water; }
    void setFertilizer(bool fertilizer) { Fertilizer = fertilizer; }
}; 