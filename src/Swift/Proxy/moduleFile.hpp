#pragma once

#include "Swift/Proxy/module.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace Swift::Proxy {

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
	* The code that exposes the C++ code to Swift
	*/
	std::filesystem::path getSwiftFile() const;
	std::string getSwift() const;

	/**
	* Connects Objective-C++ to Swift
	*/
	std::filesystem::path getBridgingHeaderFile() const;
	std::string getBridgingHeader() const;

private:
	std::string m_rootModuleName;
	std::string m_libraryName;

	// Will be put one after each other
	// The modules are responsible for which of them are submodules
	std::vector<Module> m_modules;
};
}    // namespace Swift::Proxy
