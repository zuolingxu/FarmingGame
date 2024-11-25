#ifndef JSONMANAGER_H
#define JSONMANAGER_H

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
	std::unordered_map<std::string, rapidjson::Document> data_;
	// The Constructor will load the Config document(writable) and the global document(unwritable)
	DocumentManager();
	~DocumentManager();

	rapidjson::Document* createConfigDocument();


public: 
	DocumentManager(const DocumentManager&) = delete;
	DocumentManager& operator=(const DocumentManager&) = delete;


	// Singleton mode, there's only one instance
	static DocumentManager* getInstance();

	// Load json file from path, if not exist, return false
	bool loadDocument(const std::string& path, const std::string& name = "");

	// free the document from memory
	void freeDocument(const std::string& name);

	// get the json document, if not loaded, return nullptr
	const rapidjson::Document* getDocument(const std::string& name);

	// create Archive document and load it
	// You should free loaded archive first
	// if the Archive already exist, return nullptr
	rapidjson::Document* createArchiveDocument(int num);

	// load Archive document, you can only open one Archive once
	// If the Archive can be opened or corrupted, it will return false
	// You should free loaded archive first
	bool loadArchiveDocument(const int num);

	// get Archive document, if not loaded it will return nullptr
	rapidjson::Document* getArchiveDocument();

	// get ConfigDocument
	rapidjson::Document* getConfigDocument();

	// save Archive document
	void saveArchiveDocument();

	// freeArchiveDocument
	void freeArchiveDocument();

	// save Config document
	void saveConfigDocument();

	// delete Archive and save Config document
	void deleteArchive(int num);
};
#endif