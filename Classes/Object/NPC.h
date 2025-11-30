#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "MapObject.h"
#include "json/document.h"
#include "MapLayer.h"
#include "MainCharacter.h"
#include "Audio.h"
#include "UIlogic.h"
#include "SceneManager.h"
#include "MapObjectFactory.h"

static bool npc_registered = [](){
    MapObjectFactory::registerCreator("NPC", [](rapidjson::Value& v, MapLayer* p, const Vec<int>& pos)->MapObject* {
        return NPC::create(v, p, pos); 
    });
    return true;
}();


class NPC final : protected MapObject {
private:
    MapLayer* parent_;
    std::string name;
    int emotion;
    bool isPaused;
    // Record each NPC's favorite item
    const std::unordered_map<std::string, ItemType> Favorite = {
        {"Abigail", ItemType::CAULIFLOWER},
        {"Caroline", ItemType::PUMPKIN},
        {"Haley", ItemType::FISH}
    };
    // Record the scene where each NPC should be refreshed
    const std::unordered_map<std::string, std::string> Scene = {
        {"Abigail", "Town"},
        {"Caroline", "npc1house"},
        {"Haley", "npc2house"}
    };

    void change_archive_in_memory();  // Update the NPC's archive in memory
    // Set the default behavior for the NPC
    void defaultAction();

public:
    explicit NPC(MapLayer* parent, const Vec<int>& pos, std::string name, int emo, int length, bool pause);
    ~NPC() override;
    static MapObject* create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos);
    virtual void init();  // Initialize the NPC
    virtual void interact();  // Handle interactions with the player
    virtual void clear();  // Clear NPC's resources
    virtual void pause();  // Pause the NPC's activities
    virtual void resume();  // Resume the NPC's activities
    virtual void settle();  // Save the NPC's state
    virtual bool hasCollision();  // Check if the NPC has a collision
};
