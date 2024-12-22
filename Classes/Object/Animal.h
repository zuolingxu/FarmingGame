#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "MapObject.h"
#include "json/document.h"
#include "MapLayer.h"
#include"PlayerSprite.h"
#include "MainCharacter.h"
#include<set>

class Animal final: protected MapObject {
private:
	MapLayer* parent_;
	bool isPause;
	//Types of animals
	std::string type;
	//Has it been sold 
	bool isSold;
	//How many gold coins can be obtained if you sell it
	int value;
	//Default refresh location
	Vec<int> defaultPos;
	int satiety;
	float breedProbility;
	//Modify animal in archive
	bool change_archive_in_memory();
	//delete animal in archive
	bool delete_archive_in_memory();
	//add aniaml in achieve
	bool new_archive_in_memory(int x, int y);
	//Default patroling route
	void patrolPath();
	//Available refresh points
	const int lifePoints[6] = {4,5,6,7,11,14};
	static std::set<int>tag;
public:
	explicit Animal(MapLayer* parent, const Vec<int>& pos, std::string Type, int AnimalValue,int satiety,int length,float breedPro=0.2);
	~Animal() override {};
	//Factory function
	static MapObject* create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos);
	//Initialize animation
	virtual void init() override;
	//Interacting with players includes being fed and being sold.
	virtual void interact() override;
	virtual void clear() override;
	virtual void pause() override;
	virtual void resume() override;
	virtual void settle() override;
	virtual bool hasCollision() override;
	void defaultAction();
	void sold();
	//Called at the end of each day, used to simulate animal reproduction.
	void breed();
};


