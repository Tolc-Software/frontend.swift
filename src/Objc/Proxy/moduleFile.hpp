#pragma once

#include "Objc/Proxy/module.hpp"
#include "Objc/cache.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace Objc::Proxy {

class ModuleFile {
public:
	explicit ModuleFile(Module const& rootModule,
	                    std::string const& libraryName);

	/**
	* Add a module to the file.
	* NOTE: The order these are added is the order they will be put in the file
	*/
	void addModule(Module const& m);

	std::filesystem::path getFilepath() const;

	/**
	* The code that uses the C++ input
	*/
	std::filesystem::path getObjcHeaderFile() const;
	std::string getObjcHeader() const;

	/**
	* The code that exposes the C++ code to Objective-C
	*/
	std::filesystem::path getObjcSourceFile() const;
	std::string getObjcSource() const;

	/**
	* Connects Objective-C++ to Swift
	*/
	std::filesystem::path getBridgingHeaderFile() const;
	std::string getBridgingHeader() const;

	void setCache(Objc::Cache const& cache);

private:
	std::string m_rootModuleName;
	std::string m_libraryName;

	// Will be put one after each other
	// The modules are responsible for which of them are submodules
	std::vector<Module> m_modules;

	Objc::Cache m_cache;
};
}    // namespace Objc::Proxy
