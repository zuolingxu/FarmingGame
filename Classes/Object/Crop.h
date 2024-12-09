#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "json/document.h"
#include "MapLayer.h"
#include <string>
#include "Land.h"
class Crop{
	private:
	cocos2d::Sprite* sprite_ = nullptr;
	MapLayer* parent_;
	std::string CropName;
	int LiveDay;
	int MaturationDay;

	public:
	explicit Crop(MapLayer* parent);
	~Crop();
	static Crop* create(rapidjson::Value& val, MapLayer* parent);
	static Crop* createByPlayer(const cocos2d::Vec2& position, MapLayer* parent, Land* land); // 传入调用种子的 Land 对象

	void saveToArchive(const cocos2d::Vec2& position, Land* land); // 传入 Land 对象
	void init();
	void interact();
	void clear();
	void settle();

	// 为Land类传入数据
	std::string getCropName() const { return CropName; }
	int getLiveDay() const { return LiveDay; }
	int getMaturationDay() const { return MaturationDay; }

}; 

//种子从第1天开始生长,施肥后从第2天开始生长