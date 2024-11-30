#include "SceneManager.h"

USING_NS_CC;

SceneManager* SceneManager::instance_ = new SceneManager;

SceneManager::SceneManager() : layers_(16)
{
	director_ = Director::getInstance();
	
}

SceneManager::~SceneManager()
{
	for (const auto& i: layers_)
	{
		delete i.second;
	}
}

void SceneManager::initLayer()
{

}

void SceneManager::hideLayer(std::string layer_name)
{
	
}

void SceneManager::showLayer(std::string layer_name)
{
	
}

void SceneManager::parser_layer()
{

}

SceneManager* SceneManager::getInstance()
{
	return instance_;
}



