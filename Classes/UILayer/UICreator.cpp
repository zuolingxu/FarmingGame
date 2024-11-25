#include "UICreator.h"

USING_NS_CC;

cocos2d::Layer* UICreator::createWithFile(const rapidjson::Value* doc)
{
	Layer* layer = Layer::create();

	return layer;
}
