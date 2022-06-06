#pragma once

#include "Objc/Proxy/attribute.hpp"
#include "Objc/Proxy/class.hpp"
#include "Objc/Proxy/enum.hpp"
#include "Objc/Proxy/function.hpp"
#include "Objc/Proxy/structure.hpp"
#include "Objc/cache.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace Objc::Proxy {

class ModuleFile {
public:
	explicit ModuleFile();

	/**
	* The code that uses the C++ input
	*/
	std::filesystem::path getObjcHeaderFile() const;
	std::string getObjcHeader();

	/**
	* The code that exposes the C++ code to Objective-C
	*/
	std::filesystem::path getObjcSourceFile() const;
	std::string getObjcSource();

	/**
	* Connects Objective-C++ to Swift
	*/
	std::filesystem::path getBridgingHeaderFile() const;
	std::string getBridgingHeader() const;

	void addEnum(Objc::Proxy::Enum const& e);
	void addClass(Objc::Proxy::Class const& c);
	void addModule(Objc::Proxy::Class const& m);
	void addFunction(Objc::Proxy::Function const& f);
	void addAttribute(Objc::Proxy::Attribute const& v);

	void setCache(Objc::Cache const& cache);

private:
	void sortAllStructures();

	std::vector<Class> m_modules;
	std::vector<Class> m_classes;
	std::vector<Function> m_functions;
	std::vector<Enum> m_enums;
	std::vector<Attribute> m_attributes;

	std::vector<Structure const*> m_allStructures;

	Objc::Cache m_cache;
};
}    // namespace Objc::Proxy
