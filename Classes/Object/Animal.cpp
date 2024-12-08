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
//    // ���� JSON ����
//    if (val.HasMember("Name") && val["Name"].IsString()) {
//        animal->name = val["Name"].GetString();
//    }
//
//    if (val.HasMember("Sold") && val["Sold"].IsBool()) {
//        animal->isSold = val["Sold"].GetBool();
//    }
//    else {
//        animal->isSold = false; // Ĭ��δ����
//    }
//
//    // ���� tilePosition "12 7" -> x=12, y=7
//
//
//    // ����ÿ�������СΪ 16x16 ����
//
//
//    // ��ʼ������
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
//        spriteFile = "animals/chicken.png"; // ����ʵ����Դ·������
//    }
//
//    if (name == "cow")
//    {
//        spriteFile = "animals/cow.png";
//    }
//
//
//    // ��������
//
//
//    // ���þ���λ��
//    info_.sprite->setPosition(position);
//
//    // ��������ӵ� MapLayer
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
