#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include <unordered_map>
#include <fstream>
#include <string>
#include <cstdio>
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
	std::string current_archive_;
	std::unordered_map<std::string, rapidjson::Document> data_;
	// The Constructor will load the Config document(writable) and the global document(unwritable)
	DocumentManager();
	~DocumentManager();

public: 
	DocumentManager(const DocumentManager&) = delete;
	DocumentManager& operator=(const DocumentManager&) = delete;


	// Singleton mode, there's only one instance
	static DocumentManager* getInstance();

	// Load json file from path, if not exist, return false
	bool loadDocument(std::string& path);

	// free the json file from memory
	void freeDocument(std::string& path);

	// get the json document, if not loaded it will load it first
	const rapidjson::Document* getDocument(std::string& path);

	// load Archive document, you can only open one Archive
	bool loadArchiveDocument(std::string& name);

	// get Archive document, if not loaded it will return nullptr
	rapidjson::Document* getArchiveDocument();

	// get ConfigDocument
	rapidjson::Document* getConfigDocument();

	// save Archive and Config document
	void saveDocument();

	// delete Archive
	void deleteArchive(std::string& name);
};
#endif