#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include <fstream>
#include <string>
#include "cocos2d.h"
#include "json/reader.h"
#include "json/document.h"

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