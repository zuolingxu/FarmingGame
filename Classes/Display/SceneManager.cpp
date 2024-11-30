#include "SceneManager.h"
#include "HelperClasses.h"

USING_NS_CC;

SceneManager* SceneManager::instance_ = new SceneManager;

SceneManager::SceneManager() : director_(Director::getInstance()) {}

SceneManager::~SceneManager() {}

void SceneManager::createMapWithDocument(rapidjson::Document* doc)
{
	std::string name = (*doc)["Name"].GetString();
	DocumentManager* manager = DocumentManager::getInstance();
	Vec<int> Size = toVec2((*doc)["Size"].GetString());
	std::string tmx_path = (*doc)["TMX"].GetString();
	if (manager->hasDocument(tmx_path))
	{
		tmx_path = manager->getPath(tmx_path);
	}

	rapidjson::Value* const_object = nullptr, *archive_object = nullptr;
	if (doc->HasMember("ObjectList"))
	{
		 const_object = &(*doc)["ObjectList"];
	}
	rapidjson::Document* archive_doc = manager->getArchiveDocument();
	if ((*archive_doc)["Map"].HasMember(name.c_str()))
	{
		archive_object = &(*archive_doc)["Map"][name.c_str()];
	}

	maps_.emplace(name, MapLayer::createWithDocument(
		Size, tmx_path,const_object, archive_object));
}

SceneManager* SceneManager::getInstance()
{
	return instance_;
}

void SceneManager::createMaps()
{
	DocumentManager* manager = DocumentManager::getInstance();
	const rapidjson::Document* doc = manager->getDocument(manager->getPath("global"));
	for (const rapidjson::Value& layer : (*doc)["Map"].GetArray())
	{
		createMapWithDocument(manager->getDocument(manager->getPath(layer.GetString())));
	}
}


void SceneManager::hideLayer(std::string layer_name)
{
	
}

void SceneManager::showLayer(std::string layer_name)
{
	
}

void SceneManager::NextScene(std::string scene_name)
{
	
}


