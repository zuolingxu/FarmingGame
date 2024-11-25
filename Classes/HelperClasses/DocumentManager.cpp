#include "DocumentManager.h"

using namespace rapidjson;
// All player data shall be saved in this folder
// On Windows the path is C:\Users\Username\AppData\Local\FarmingGame
static const std::string g_writable_path = cocos2d::FileUtils::getInstance()->getWritablePath();
DocumentManager* DocumentManager::instance_ = new DocumentManager;


bool DocumentManager::readFile(const std::string& path, DocumentType type)
{
	std::ifstream input_file(path);
	std::string name;
	if (type == DocumentType::normal)
	{
		name = getFileName(path);
	}
	else if (type == DocumentType::config)
	{
		name = "UsrConfig";
	}
	else if (type == DocumentType::archive)
	{
		name = "UsrArchive";
	}

	if (!input_file.is_open())
	{
		CCLOG("Invalid file name");
		return false;
	}
	IStreamWrapper isw(input_file);
	Document* doc = new Document;
	doc -> ParseStream(isw);
	if (doc->HasParseError())
	{
		CCLOG("Invalid file format");
		delete doc;
		return false;
	}
	input_file.close();
	data_.emplace(name, doc);
	return true;
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
	if (!readFile(g_writable_path + "UsrConfig.json"))
	{
		createConfigDocument();
	}
	loadDocument("global.json");
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

std::string DocumentManager::getFileName(const std::string& path)
{
	size_t len = path.length();
	int start;
	for (start = len - 1; start >= 0; start--)
	{
		if (path[start] == '/' || path[start] == '\\')
		{
			break;
		}
	}
	int end;
	for (end = start; end < len; end++)
	{
		if (path[end] == '.')
		{
			break;
		}
	}
	start++;
	if (start != 0) {
		return path.substr(start, end - start);
	}
	return {};
}

void DocumentManager::loadDocument(const std::string& path)
{
	const std::string filePath = cocos2d::FileUtils::getInstance()->fullPathForFilename(path);
	if (!readFile(filePath))
	{
		throw std::runtime_error("²â²â²âFailed to read " + path + 
			"\nThe Game File has been corrupted or incomplete. Download it again.");
	}
}

void DocumentManager::freeDocument(const std::string& name)
{
	const auto it = data_.find(name);
	if (it != data_.end())
	{
		delete it -> second;
		data_.erase(it);
	}
}

const Document* DocumentManager::getDocument(const std::string& name)
{
	const auto it = data_.find(name);
	if (it != data_.end())
	{
		return it->second;
	}
	return nullptr;
}

void DocumentManager::createConfigDocument()
{
	std::string file_path = cocos2d::FileUtils::getInstance()->fullPathForFilename("NewUsrConfig.json");
	if (!readFile(file_path, DocumentType::config))
	{
		throw std::runtime_error("Failed to read NewUsrConfig.json");
	}
	
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
		CCLOG("Archive already exists!");
		return false;
	}

	std::string file_path = cocos2d::FileUtils::getInstance()->fullPathForFilename("NewUsrArchive.json");
	if (!readFile(file_path, DocumentType::archive))
	{
		throw std::runtime_error("Failed to read NewUsrConfig.json");
	}

	if (current_archive_ != 0)
	{
		freeArchiveDocument();
	}
	current_archive_ = num;
	saveArchiveDocument();
	return true;
}

bool DocumentManager::loadArchiveDocument(const int num)
{
	auto filepath = std::format("{}Save_{}.json", g_writable_path, num);
	if (!readFile(filepath, DocumentType::archive))
	{
		return false;
	}

	if (current_archive_ != 0)
	{
		freeArchiveDocument();
	}
	current_archive_ = num;
	return true;
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

Document* DocumentManager::getArchiveDocument() const
{
	if (current_archive_ == 0)
	{
		return nullptr;
	}
	return data_.at("UsrArchive");
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