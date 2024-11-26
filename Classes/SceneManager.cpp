#include "SceneManager.h"

USING_NS_CC;

SceneManager* SceneManager::instance_ = new SceneManager;

SceneManager::SceneManager() : layers_(16)
{
	director_ = Director::getInstance();
}