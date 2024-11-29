#include "UILayer.h"

USING_NS_CC;

UILayer* UILayer::createWithFile(const std::string& name)
{
	if (! this -> init())
	{
		return nullptr;
	}
}

