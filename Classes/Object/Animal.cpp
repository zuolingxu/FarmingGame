//#include "Animal.h"
//
//Animal::Animal(MapLayer* parent)
//	: parent_(parent), name(""), isSold(false), position(0, 0){
//	
//}
//
//Animal::~Animal()
//{
//	if (info_.sprite) {
//		info_.sprite->removeFromParent();
//		info_.sprite = nullptr;
//	}
//}
//
//
//Object* Animal::create(rapidjson::Value& val, MapLayer* parent)
//{
//    Animal* animal = new Animal(parent);
//
//    // 解析 JSON 数据
//    if (val.HasMember("Name") && val["Name"].IsString()) {
//        animal->name = val["Name"].GetString();
//    }
//
//    if (val.HasMember("Sold") && val["Sold"].IsBool()) {
//        animal->isSold = val["Sold"].GetBool();
//    }
//    else {
//        animal->isSold = false; // 默认未售卖
//    }
//
//    // 解析 tilePosition "12 7" -> x=12, y=7
//
//
//    // 假设每个网格大小为 16x16 像素
//
//
//    // 初始化对象
//    animal->init();
//
//	return animal;
//}
//
//void Animal::init()
//{
//	info_.sprite = nullptr;
//
//    std::string spriteFile;
//
//    if (name == "chicken")
//    {
//        spriteFile = "animals/chicken.png"; // 根据实际资源路径调整
//    }
//
//    if (name == "cow")
//    {
//        spriteFile = "animals/cow.png";
//    }
//
//
//    // 创建精灵
//
//
//    // 设置精灵位置
//    info_.sprite->setPosition(position);
//
//    // 将精灵添加到 MapLayer
//
//}
//
//void Animal::interact()
//{
//	
//}
//
//void Animal::clear()
//{
//	info_.sprite = nullptr;
//}
//
//void Animal::pause()
//{
//	
//}
//
//void Animal::resume()
//{
//	
//}
//
//
//void Animal::settle()
//{
//	
//}
//
//bool Animal::hasCollision()
//{
//	return true;
//}
//
