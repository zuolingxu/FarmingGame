#pragma once
#include <unordered_map>
#include <string>
#include "cocos2d.h"
#include "json/document.h"


class DocumentManager
{
private:
	static DocumentManager* instance_;
	int current_archive_;
	std::unordered_map<std::string, rapidjson::Document*> data_;
	std::unordered_map<std::string, std::string> name_map_;
	// The Constructor will load the Config document(writable) and the global document(unwritable)
	DocumentManager();

	//  Init the file name - file path map
	void initNameMap();

	~DocumentManager();

	static rapidjson::Document* readFile(const std::string& path);
	void writeFile(const std::string& path, const std::string& name) const;

	// create a new usr config and load all exist archive's key_info
	// If cannot load NewUsrConfig, throw exception
	void createConfigDocument();

public:
	DocumentManager(const DocumentManager&) = delete;
	DocumentManager& operator=(const DocumentManager&) = delete;

	// Singleton mode, there's only one instance
	static DocumentManager* getInstance();

	// Get the file path with name, you can use it like :
	// DocumentManager* manager = DocumentManager::getInstance();
	// manager->getDocument(manager->getPath("global"));
	std::string getPath(const std::string& name);

	// for normal document: get -> free

	// get the json document, if not loaded, return nullptr
	// The path is related to Resources folder
	// If not loaded, load file with path, if not exist, throw exception
	const rapidjson::Document* getDocument(const std::string& path);

	// free the document from memory
	void freeDocument(const std::string& path);



	// for archive document: create/load -> get -> free/save
	// special option: delete

	// create save and load a new Archive.
	// If the Archive has been opened, it will free the former document and return true.
	// If the Archive already exist, return false.
	// If cannot load NewUsrArchive, throw exception
	bool createArchiveDocument(const int num);

	// load Archive document, you can only open one Archive once
	// If the Archive cannot be opened or has been corrupted, it will return false
	// If the Archive has been opened, it will free the former document and return true
	bool loadArchiveDocument(const int num);

	// get Archive document, if not loaded it will return nullptr
	rapidjson::Document* getArchiveDocument() const;

	// free and save Archive Document
	void freeArchiveDocument();

	// save Archive document when an archive has been loaded
	void saveArchiveDocument();

	// delete Archive and save Config document
	bool deleteArchive(int num);



	// for config document: get -> save

	// get ConfigDocument
	rapidjson::Document* getConfigDocument() const;

	// save Config document
	void saveConfigDocument() const;
};