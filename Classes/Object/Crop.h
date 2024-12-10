#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "json/document.h"
#include "MapLayer.h"
#include <string>

class Land;

class Crop{
	private:
	cocos2d::Sprite* sprite_ = nullptr;
	MapLayer* parent_;
	std::string CropName;
	int LiveDay;
	int MaturationDay;

	public:
	explicit Crop(MapLayer* parent, const Vec<int>& pos);
	~Crop();
	static Crop* create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos);
	static Crop* createByPlayer(const cocos2d::Vec2& position, MapLayer* parent, Land* land); // ����������ӵ� Land ����

	void saveToArchive(const cocos2d::Vec2& position, Land* land); // ���� Land ����
	void init();
	void interact();
	void clear();
	void settle();

	// ΪLand�ഫ������
	std::string getCropName() const { return CropName; }
	int getLiveDay() const { return LiveDay; }
	int getMaturationDay() const { return MaturationDay; }

}; 

//���Ӵӵ�1�쿪ʼ����,ʩ�ʺ�ӵ�2�쿪ʼ����