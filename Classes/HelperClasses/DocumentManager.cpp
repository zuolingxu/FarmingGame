#include "DocumentManager.h"

using namespace rapidjson;
// All player data shall be saved in this folder
// On Windows the path is C:\Users\Username\AppData\Local\FarmingGame
static const std::string g_writable_path = cocos2d::FileUtils::getInstance()->getWritablePath();
DocumentManager* DocumentManager::instance_ = new DocumentManager;

DocumentManager::DocumentManager() : data_(32), current_archive_(0)
{
	std::string file_path = g_writable_path + "UsrConfig.json";
	std::ifstream input_file(file_path);

	if (!input_file.is_open())
	{
		createConfigDocument();
	}
	else
	{
		IStreamWrapper isw(input_file);
		Document doc;
		doc.ParseStream(isw);
		input_file.close();
		if (doc.HasParseError() || !doc.IsObject())
		{
			try
			{
				std::filesystem::remove(file_path);
			}
			catch (std::runtime_error e)
			{
				CCLOG("Error: %s",e.what());
				throw std::runtime_error("Config Document has been corrupted, "
							 "cannot delete the file, please delete the file and "
							 "run this game again to fix it.");
			}
			throw std::runtime_error("Config Document has been corrupted, "
				"run this game again to fix it.");
		}
		data_.emplace("UsrConfig", std::move(doc));
	}

	loadDocument("global.json", "global");
}

DocumentManager::~DocumentManager()
{
	saveArchiveDocument();
	saveConfigDocument();
	delete instance_;
}

DocumentManager* DocumentManager::getInstance()
{
	return instance_;
}

bool DocumentManager::loadDocument(const std::string& path, const std::string& name)
{
	if (data_.find(name) != data_.end())
	{
		return true;
	}
	// Get the path of global.json, json file can only input by ifstream
	std::string filePath = cocos2d::FileUtils::getInstance()->fullPathForFilename(path);
	std::ifstream input_file(filePath);
	if (!input_file.is_open())
	{
		CCLOG("File doesn't exist: %s", filePath.c_str());
		throw std::runtime_error("Game file is not complete, please download again.");
		return false;
	}
	IStreamWrapper isw(input_file);
	Document doc;
	doc.ParseStream(isw);
	input_file.close();

	if (doc.HasParseError())
	{
		CCLOG("Invalid file format: %s", filePath.c_str());
		return false;
	}
	if (name.empty())
	{
		if(doc.HasMember("Name") && doc["Name"].IsString())
		{
			data_.emplace(doc["Name"].GetString(), std::move(doc));
		}
		else
		{
			CCLOG("Document doesn't has a name");
			return false;
		}
	}
	else
	{
		data_.emplace(name, std::move(doc));
	}
	return true;
}

void DocumentManager::freeDocument(const std::string& name)
{
	const auto it = data_.find(name);
	if (it != data_.end())
	{
		data_.erase(it);
	}
}

const Document* DocumentManager::getDocument(const std::string& name)
{
	auto it = data_.find(name);
	if (it == data_.end())
	{
		return nullptr;
	}
	return &(it->second);
}

Document* DocumentManager::createConfigDocument()
{
	std::string file_path = cocos2d::FileUtils::getInstance()->fullPathForFilename("NewUsrConfig.json");
	std::ifstream new_file(file_path);
	IStreamWrapper isw(new_file);
	Document doc;
	doc.ParseStream(isw);
	new_file.close();
	data_.emplace("UsrConfig", std::move(doc));
	int num = 1;
	std::string archive_path = std::format("{}Save_{}.json", g_writable_path, num);
	while (std::filesystem::exists(archive_path))
	{
		// fix the Config Document
		loadArchiveDocument(num);
		saveArchiveDocument();
		freeArchiveDocument();
		num++;
		archive_path = std::format("{}Save_{}.json", g_writable_path, num);
	}
	return &data_.at("UsrConfig");
}

Document* DocumentManager::createArchiveDocument(const int num)
{
	if (current_archive_ != 0)
	{
		CCLOG("Free current Archive first!");
		throw std::logic_error("Free current Archive first!");
	}

	std::string archive_name = std::format("Save_{}", num);
	if (data_["UsrConfig"]["Archive"].HasMember(archive_name.c_str()))
	{
		CCLOG("Archive already exists!");
		return nullptr;
	}

	std::string file_path = cocos2d::FileUtils::getInstance()->fullPathForFilename("NewUsrArchive.json");
	std::ifstream new_file(file_path);
	IStreamWrapper isw(new_file);
	Document doc;
	doc.ParseStream(isw);
	new_file.close();

	data_.emplace("UsrArchive", std::move(doc));
	current_archive_ = num;
	return &data_.at("UsrArchive");
}

bool DocumentManager::loadArchiveDocument(const int num)
{
	if (current_archive_ != 0)
	{
		CCLOG("Free current Archive first!");
		throw std::logic_error("Free current Archive first!");
	}

	auto filepath = std::format("{}Save_{}.json", g_writable_path, num);
	std::ifstream input_file(filepath);

	if (!input_file.is_open())
	{
		return false;
	}
	else
	{
		IStreamWrapper isw(input_file);
		Document doc;
		doc.ParseStream(isw);
		input_file.close();
		if (doc.HasParseError() || !doc.IsObject())
		{
			return false;
		}
		data_.emplace("UsrArchive", std::move(doc));
		current_archive_ = num;
		return true;
	}
}

void DocumentManager::freeArchiveDocument()
{
	if (current_archive_ != 0)
	{
		saveArchiveDocument();
		data_.erase("UsrArchive");
		current_archive_ = 0;
	}
}

Document* DocumentManager::getArchiveDocument()
{
	if (current_archive_ == 0)
	{
		return nullptr;
	}
	return &data_.at("UsrArchive");
}

Document* DocumentManager::getConfigDocument()
{
	return &data_.at("UsrConfig");
}

void DocumentManager::saveArchiveDocument()
{
	if (current_archive_ > 0)
	{
		Document& config = data_.at("UsrConfig");
		Document& current_archive = data_.at("UsrArchive");

		std::string filepath = std::format("{}Save_{}.json", g_writable_path, current_archive_);
		std::ofstream ofs(filepath);
		OStreamWrapper osw(ofs);
		Writer<OStreamWrapper> writer(osw);
		current_archive.Accept(writer);
		ofs.close();

		Value& config_archive = config["Archive"];
		std::string file_name = std::format("Save_{}", current_archive_);
		config_archive.RemoveMember(file_name.c_str());
		auto& allocator = config.GetAllocator();
		Value key_info;
		key_info.CopyFrom(current_archive["key_info"], allocator);
		config_archive.AddMember(Value(file_name.c_str(), allocator), key_info, allocator);
	}
}

void DocumentManager::saveConfigDocument()
{
	Document& config = data_.at("UsrConfig");
	std::ofstream ofs(g_writable_path + "UsrConfig.json");
	OStreamWrapper osw(ofs);
	Writer<OStreamWrapper> writer(osw);
	config.Accept(writer);
	ofs.close();
}

void DocumentManager::deleteArchive(const int num)
{
	std::string archive_name = std::format("Save_{}", num);
	std::string file_path = std::format("{}{}.json", g_writable_path, archive_name);
	if (data_["UsrConfig"]["Archive"].HasMember(archive_name .c_str()))
	{
		if (num == current_archive_)
		{
			freeArchiveDocument();
		}
		data_["UsrConfig"]["Archive"].RemoveMember(archive_name.c_str());
		if (std::filesystem::exists(file_path))
		{
			try
			{
				std::filesystem::remove(file_path);
			}
			catch (std::runtime_error)
			{
				throw std::runtime_error("cannot delete the file, please delete the file manually "
					"and run this game again to fix it.");
			}
		}
		saveConfigDocument();
	}
}