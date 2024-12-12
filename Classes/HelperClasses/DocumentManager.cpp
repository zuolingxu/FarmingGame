#include "DocumentManager.h"
#include "json/writer.h"
#include "json/istreamwrapper.h"
#include "json/ostreamwrapper.h"
#include "cocos2d.h"
#include <format>
#include <filesystem>
#include <fstream>

#ifdef _MSC_VER
#undef GetObject
#endif

using namespace rapidjson;
// All player data shall be saved in this folder
// On Windows the path is C:\Users\Username\AppData\Local\FarmingGame
static const std::string g_writable_path = cocos2d::FileUtils::getInstance()->getWritablePath();
DocumentManager* DocumentManager::instance_ = new DocumentManager;


rapidjson::Document* DocumentManager::readFile(const std::string& path)
{
	std::ifstream input_file(path);
	std::string name;

	if (!input_file.is_open())
	{
		CCLOG("Invalid file name");
		return nullptr;
	}
	IStreamWrapper isw(input_file);
	Document* doc = new Document;
	doc -> ParseStream(isw);
	if (doc->HasParseError() || !doc->IsObject())
	{
		CCLOG("Invalid file format");
		delete doc;
		return nullptr;
	}
	input_file.close();
	return doc;
}

void DocumentManager::writeFile(const std::string& path, const std::string& name) const
{
	const Document* doc = data_.at(name);
	std::ofstream ofs(path);
	OStreamWrapper osw(ofs);
	Writer<OStreamWrapper> writer(osw);
	doc -> Accept(writer);
	ofs.close();
}

DocumentManager::DocumentManager() : current_archive_(0), data_(32)
{
	Document* doc = readFile(g_writable_path + "UsrConfig.json");
	if (doc == nullptr)
	{
		createConfigDocument();
	}
	else
	{
		data_.emplace("UsrConfig", doc);
	}
	initNameMap();
}

void DocumentManager::initNameMap()
{
	rapidjson::Value* name_map = &(*getDocument("global.json"))["Files"];
	for (auto& it: name_map->GetObject())
	{
		name_map_.emplace(it.name.GetString(), it.value.GetString());
	}
}

DocumentManager::~DocumentManager()
{
	saveArchiveDocument();
	saveConfigDocument();
	for (const auto& pari : data_)
	{
		delete pari.second;
	}
	delete instance_;
}

DocumentManager* DocumentManager::getInstance()
{
	return instance_;
}

std::string DocumentManager::getPath(const std::string& name)
{
	if (name_map_.contains(name))
	{
		return name_map_.at(name);
	}
	else
	{
		return "";
	}
}

bool DocumentManager::hasDocument(const std::string& name) const
{
	return name_map_.contains(name);
}


void DocumentManager::freeDocument(const std::string& path)
{
	const auto it = data_.find(path);
	if (it != data_.end())
	{
		delete it -> second;
		data_.erase(it);
	}
}

rapidjson::Document* DocumentManager::getDocument(const std::string& path)
{
	if (!data_.contains(path))
	{
		const std::string file_path = cocos2d::FileUtils::getInstance()->fullPathForFilename(path);
		Document* doc = readFile(file_path);
		if (doc == nullptr)
		{
			throw std::runtime_error("Failed to read " + path +
				"\nThe Game File has been corrupted or incomplete. Download it again.");
		}
		data_.emplace(path, doc);
	}
	return data_.at(path);
}

void DocumentManager::createConfigDocument()
{
	std::string file_path = cocos2d::FileUtils::getInstance()->fullPathForFilename("NewUsrConfig.json");
	Document* doc = readFile(file_path);
	if (doc == nullptr)
	{
		throw std::runtime_error("Failed to read NewUsrConfig.json");
	}
	data_.emplace("UsrConfig", doc);
	
	for (int i = 1; i <= 100; i++)
	{
		std::string archive_path = std::format("{}Save_{}.json", g_writable_path, i);
		if (std::filesystem::exists(archive_path))
		{
			// fix the Config Document
			loadArchiveDocument(i);
			saveArchiveDocument();
			freeArchiveDocument();
		}
		archive_path = std::format("{}Save_{}.json", g_writable_path, i);
	}
	saveConfigDocument();
}

bool DocumentManager::createArchiveDocument(const int num)
{
	std::string archive_name = std::format("Save_{}", num);
	if ((*data_["UsrConfig"])["Archive"].HasMember(archive_name.c_str()))
	{
		// CCLOG("Archive already exists!");
		return false;
	}

	std::string file_path = cocos2d::FileUtils::getInstance()->fullPathForFilename("NewUsrArchive.json");
	Document* doc = readFile(file_path);
	if (doc == nullptr)
	{
		throw std::runtime_error("Failed to read NewUsrConfig.json");
	}

	if (current_archive_ != 0)
	{
		freeArchiveDocument();
	}

	data_.emplace("UsrArchive", doc);
	current_archive_ = num;
	saveArchiveDocument();
	return true;
}

bool DocumentManager::loadArchiveDocument(const int num)
{
	auto file_path = std::format("{}Save_{}.json", g_writable_path, num);
	Document* doc = readFile(file_path);
	if (doc == nullptr)
	{
		return false;
	}
	data_.emplace("UsrArchive", doc);

	if (current_archive_ != 0)
	{
		freeArchiveDocument();
	}
	current_archive_ = num;
	return true;
}

Document* DocumentManager::getArchiveDocument() const
{
	if (current_archive_ == 0)
	{
		return nullptr;
	}
	return data_.at("UsrArchive");
}

void DocumentManager::freeArchiveDocument()
{
	if (current_archive_ != 0)
	{
		saveArchiveDocument();
		freeDocument("UsrArchive");
		current_archive_ = 0;
	}
}

Document* DocumentManager::getConfigDocument() const
{
	return data_.at("UsrConfig");
}

void DocumentManager::saveArchiveDocument()
{
	if (current_archive_ > 0)
	{
		Document* config = data_.at("UsrConfig");
		Document* current_archive = data_.at("UsrArchive");

		std::string filepath = std::format("{}Save_{}.json", g_writable_path, current_archive_);
		writeFile(filepath, "UsrArchive");

		Value& config_archive = (*config)["Archive"];
		std::string file_name = std::format("Save_{}", current_archive_);
		config_archive.RemoveMember(file_name.c_str());
		auto& allocator = config -> GetAllocator();
		Value key_info;
		key_info.CopyFrom((*current_archive)["key_info"], allocator);
		config_archive.AddMember(Value(file_name.c_str(), allocator), key_info, allocator);
		saveConfigDocument();
	}
}

void DocumentManager::saveConfigDocument() const
{
	writeFile(g_writable_path + "UsrConfig.json", "UsrConfig");
}

bool DocumentManager::deleteArchive(const int num)
{
	std::string archive_name = std::format("Save_{}", num);
	std::string file_path = std::format("{}{}.json", g_writable_path, archive_name);
	Document& config = *data_.at("UsrConfig");
	if (config["Archive"].HasMember(archive_name .c_str()))
	{
		if (num == current_archive_)
		{
			freeArchiveDocument();
		}
		config["Archive"].RemoveMember(archive_name.c_str());
		if (std::filesystem::exists(file_path))
		{
			try
			{
				std::filesystem::remove(file_path);
			}
			catch (const std::exception& e)
			{
				return false;
			}
		}
		saveConfigDocument();
	}
	return true;
}