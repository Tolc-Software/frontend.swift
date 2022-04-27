#pragma once

#include "Objc/Proxy/class.hpp"
#include "Objc/Proxy/enum.hpp"
#include "Objc/Proxy/module.hpp"
#include "Objc/cache.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace Objc::Proxy {

class ModuleFile {
public:
	explicit ModuleFile();

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

	void addEnum(Objc::Proxy::Enum const& e);
	void addClass(Objc::Proxy::Class const& c);
	void addModule(Objc::Proxy::Class const& m);

	void setCache(Objc::Cache const& cache);

private:
	std::vector<Class> m_modules;
	std::vector<Class> m_classes;
	std::vector<Enum> m_enums;

	Objc::Cache m_cache;
};
}    // namespace Objc::Proxy
