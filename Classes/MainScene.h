#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include <fstream>
#include "cocos2d.h"
#include "nlohmann/json.hpp" // add this file to use json

// All player data shall be saved in this folder
extern const std::string WRITABLE_PATH;

class Main : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Main);
};

#endif // __Main_SCENE_H__
