#include "UIManager.h"

USING_NS_CC;
std::map<std::string, UIManager*> UIManager::ui_list;

UIManager* UIManager::createWithFile(const std::string& path)
{
	auto it = ui_list.find(path);
	if (it != ui_list.end())
	{
		return it->second;
	}
	DocumentManager* manager = DocumentManager::getInstance();
	auto doc = manager->getDocument(path);
		
	Layer* layer = Layer::create();
	// initLayer();
	UIManager* new_ui = new UIManager;
	new_ui->ui_layer = layer;
	ui_list.emplace(path, new_ui);
	return new_ui;
}

Layer* UIManager::getLayer() const
{
	return ui_layer;
}

