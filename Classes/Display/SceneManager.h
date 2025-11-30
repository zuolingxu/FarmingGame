#pragma once
#include "MapLayer.h"
#include "cocos2d.h"
#include "ui/UILoadingBar.h"
#include <string>
#include <unordered_map>

// Singleton, SceneManager manage every scene in the game
// SceneManager will be created when the app start
class SceneManager {
public:
    // Singleton, only one instance can be created
    SceneManager(SceneManager const&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(SceneManager const&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;

    // get the instance of SceneManager
    static SceneManager* getInstance();

    // settle every object on the map after day passed
    // This will call every settle function in every MapLayer
    void settle();

    // get the name of current map
    std::string getCurrentMapName() { return current_map_name_; }

    // pos (string)x y init position of playsprite ,default in the middle of the map
    // in the archive pos is the key
    // Go to Next Map. If pos is not specified, it won't add a player and all Listener will be removed(for start UI)
    // If the map cannot be found, it will throw an exception
    void NextMap(const std::string& map_name, const std::string& pos = "default", float interval = 0.0f) const;

private:
    static SceneManager* instance_; //  the only instance
    std::string current_map_name_; //  the name of the current map
    cocos2d::Director* director_; //  the director of the game
    std::unordered_map<std::string, MapLayer*> map_; //  the maps of the game

    // hide Constructor and Destructor
    SceneManager();
    ~SceneManager();

    // void createUIWithDocument(rapidjson::Document* doc);
    void createMapWithDocument(rapidjson::Document* doc);

    // If the Archive has been load/unload the maps will be automatic created/removed
    // create map with document when loading Archive
    void createMaps();

    // clear all game logic when changing Archive
    void clearMaps();

};
