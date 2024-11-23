#include "DocumentManager.h"

using namespace rapidjson;
// All player data shall be saved in this folder
// On Windows the path is C:\Users\Username\AppData\Local\FarmingGame
static const std::string g_writable_path = cocos2d::FileUtils::getInstance()->getWritablePath();
DocumentManager* DocumentManager::instance_ = new DocumentManager;

DocumentManager::DocumentManager() : data_(32)
{
	std::string filePath = g_writable_path + "UsrConfig.json";
	std::ifstream input_file(filePath);

	if (!input_file.is_open())
	{
		// If the Config file doesn't exist, create a new one with NewUsrConfig under the Resource folder
		filePath = cocos2d::FileUtils::getInstance()->fullPathForFilename("NewUsrConfig.json");
		std::ifstream new_file(filePath);
		input_file.swap(new_file);
		new_file.close();
	}

	IStreamWrapper isw(input_file);
	Document doc;
	doc.ParseStream(isw);
	data_.emplace("UsrConfig.json", std::move(doc));
	loadDocument(std::string("global.json"));
}

DocumentManager::~DocumentManager()
{
	saveDocument();
	delete instance_;
}

DocumentManager* DocumentManager::getInstance()
{
	return instance_;
}

bool DocumentManager::loadDocument(const std::string& path)
{
	// Get the path of global.json, json file can only input by ifstream
	std::string filePath = cocos2d::FileUtils::getInstance()->fullPathForFilename(path);
	std::ifstream input_file(filePath);
	if (!input_file.is_open())
	{
		return false;
	}
	IStreamWrapper isw(input_file);
	Document doc;
	doc.ParseStream(isw);
	data_.emplace(path, std::move(doc));
	return true;
}

void DocumentManager::freeDocument(const std::string& path)
{
	data_.erase(path);
}

const Document* DocumentManager::getDocument(const std::string& path)
{
    if (data_.find(path) == data_.end())
    {
	    const bool result = loadDocument(path);
		if (!result)
		{
			return nullptr;
		}
    }
	return &data_.at(path);
}

bool DocumentManager::loadArchiveDocument(const std::string& name)
{
	if (!current_archive_.empty())
	{
		data_.erase("UsrArchive.json");
	}

	current_archive_ = name;
	std::string filePath = g_writable_path + name;
	std::ifstream input_file(filePath);

	if (!input_file.is_open())
	{
		// If the Archive file doesn't exist, create a new one with NewUsrArchive under the Resource folder
		filePath = cocos2d::FileUtils::getInstance()->fullPathForFilename("NewUsrArchive.json");
		std::ifstream new_file(filePath);
		input_file.swap(new_file);
		new_file.close();
	}

	IStreamWrapper isw(input_file);
	Document doc;
	doc.ParseStream(isw);
	data_.emplace("UsrArchive.json", std::move(doc));
	return true;
}

rapidjson::Document* DocumentManager::getArchiveDocument()
{
	if (data_.find(current_archive_) == data_.end())
	{
		return nullptr;
	}

	return &data_.at(current_archive_);
}

rapidjson::Document* DocumentManager::getConfigDocument()
{
	return &data_.at("UsrConfig.json");
}

void DocumentManager::saveDocument()
{
	Document& config = data_.at("UsrConfig.json");
	
	if (!current_archive_.empty())
	{
		Document& current_archive = data_.at(current_archive_);
		std::ofstream ofs(g_writable_path + current_archive_);
		OStreamWrapper osw(ofs);
		Writer<OStreamWrapper> writer(osw);
		current_archive.Accept(writer);

		Value& config_archive = config["Archive"];
		config_archive.RemoveMember(current_archive_.c_str());
		auto& allocator = config.GetAllocator();
		Value key_info;
		key_info.CopyFrom(current_archive["key_info"], allocator);
		config_archive.AddMember(Value(current_archive_.c_str(), allocator), key_info, allocator);
	}
	std::ofstream ofs(g_writable_path + "UsrConfig.json");
	OStreamWrapper osw(ofs);
	Writer<OStreamWrapper> writer(osw);
	config.Accept(writer);
}

void DocumentManager::deleteArchive(const std::string& name)
{
	std::string filePath = g_writable_path + name;
	std::remove(filePath.c_str());
	data_.erase(name);
}