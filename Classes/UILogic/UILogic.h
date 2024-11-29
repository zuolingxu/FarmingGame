#pragma once
#include "cocos2d.h"
#include <string>
#include <unordered_map>

class SceneManager
{
    private:
	static SceneManager* instance_;
	cocos2d::Director* director_;
	std::unordered_map<std::string, cocos2d::Layer*> layers_;

	SceneManager();
	~SceneManager();

	// Command should write here and be private
	// ToScene(std::string name);
	// showLayer(std::string name);
	// hideLayer(std::string name);


    public:
	void ececuteCommand();

};

