#pragma once
#include "DocumentManager.h"
#include "cocos2d.h"
constexpr int GridSize = 16;

// Vec2 is the 2D vector class provided by cocos2dx
// press ctrl and click Vec2 to see class code
inline cocos2d::Vec2 gridComputer(cocos2d::Vec2 vect){
    return cocos2d::Vec2(vect.x/GridSize, vect.y/GridSize);
}