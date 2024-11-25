#pragma once
#include <unordered_map>
#include <fstream>
#include <string>
#include <format>
#include <filesystem>
#include "cocos2d.h"
#include "json/reader.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/istreamwrapper.h"
#include "json/ostreamwrapper.h"

class DocumentManager
{
private:
	static DocumentManager* instance_;
	int current_archive_;
	std::unordered_map<std::string, rapidjson::Document*> data_;
	// The Constructor will load the Config document(writable) and the global document(unwritable)
	DocumentManager();
	~DocumentManager();

	enum class DocumentType
	{
		normal,
		config,
		archive,
	};

	bool readFile(const std::string& path, DocumentType type = DocumentType::normal);
	void writeFile(const std::string& path, const std::string& name) const;

	// create a new usr config and load all exist archive's key_info
	// If cannot load NewUsrConfig, throw exception
	void createConfigDocument();

public:
	DocumentManager(const DocumentManager&) = delete;
	DocumentManager& operator=(const DocumentManager&) = delete;

	// Singleton mode, there's only one instance
	static DocumentManager* getInstance();

	// Get the name of the file from file path
	static std::string getFileName(const std::string& path);

	// Load json file from path, if not exist, throw exception
	void loadDocument(const std::string& path);

	// free the document from memory
	void freeDocument(const std::string& name);

	// get the json document, if not loaded, return nullptr
	// The path is related to Resources folder
	const rapidjson::Document* getDocument(const std::string& name);

	// create save and load a new Archive.
	// If the Archive has been opened, it will free the former document and return true.
	// If the Archive already exist, return false.
	// If cannot load NewusrArchive, throw exception
	bool createArchiveDocument(int num);

	// load Archive document, you can only open one Archive once
	// If the Archive cannot be opened or has been corrupted, it will return false
	// If the Archive has been opened, it will free the former document and return true
	bool loadArchiveDocument(const int num);

	// get Archive document, if not loaded it will return nullptr
	rapidjson::Document* getArchiveDocument() const ;

	// get ConfigDocument
	rapidjson::Document* getConfigDocument() const;

	// save Archive document when an archive has been loaded
	void saveArchiveDocument();

	// free and save Archive Document
	void freeArchiveDocument();

	// save Config document
	void saveConfigDocument() const;

	// delete Archive and save Config document
	bool deleteArchive(int num);
};