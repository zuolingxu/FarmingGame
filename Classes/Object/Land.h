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

    MapLayer* parent;

	Crop* crop; 
    bool Fertilizer;


    public:
     explicit Land(const Vec<int>& pos, MapLayer* parent, bool Fertilizer);
    ~Land() override;

    static MapObject* create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos); //create by achive
    static MapObject* createByPlayer(const Vec<int>& position, MapLayer* parent);//in farm,hoe,nothing
    
    virtual void init() override;
    virtual void interact() override;
    virtual void clear() override;
    
    virtual void pause() override;
    virtual void resume() override;
    
    virtual void settle() override;

    // archive
    void create_archive_in_memory(const Vec<int>& position);
    void change_archive_in_memory(const Vec<int>& position);

    // Getter 
    bool isFertilized() const { return Fertilizer; }
    void setFertilizer(bool fertilizer) { Fertilizer = fertilizer; }
    virtual bool hasCollision() override;

}; 