#pragma once
#include "cocos2d.h"

class PlayerSprite : cocos2d::Sprite{
    private:
    static std::map<std::string, PlayerSprite* > player_list_;
    //PlayerSprite();
    //~PlayerSprite();

    public:
    //PlayerSprite* getInstance();
}; 