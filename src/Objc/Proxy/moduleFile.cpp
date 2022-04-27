#include "Objc/Proxy/moduleFile.hpp"
#include "Objc/cache.hpp"
#include <fmt/format.h>
#include <string>

namespace Objc::Proxy {

ModuleFile::ModuleFile() : m_modules(), m_classes(), m_enums(), m_cache() {}

std::filesystem::path ModuleFile::getFilepath() const {
	return m_cache.m_moduleName + "_swift.cpp";
}

std::string ModuleFile::getObjcHeader() const {
	std::string out = "#import <Foundation/Foundation.h>";

	for (auto const& e : m_enums) {
		out += fmt::format("{}\n", e.getObjcHeader());
	}

	for (auto const& cls : m_classes) {
		out += fmt::format("{}\n", cls.getObjcHeader());
	}

	for (auto const& m : m_modules) {
		out += m.getObjcHeader();
	}

	return out;
}

std::string createExtraFunctions(Objc::Cache const& cache) {
	std::string out;
	if (!cache.m_extraFunctions.empty()) {
		out += fmt::format(R"(
namespace {} {{
{}

}})",
		                   cache.m_extraFunctionsNamespace,
		                   fmt::join(cache.m_extraFunctions, "\n"));
	}
	return out;
}

std::string ModuleFile::getObjcSource() const {
	std::string out =
	    fmt::format(R"(
#include "{libraryName}_objc.h"
#import <Foundation/Foundation.h>
#include <memory>
)",
	                fmt::arg("libraryName", m_cache.m_moduleName));

	out += createExtraFunctions(m_cache);

	for (auto const& e : m_enums) {
		out += fmt::format("{}\n", e.getObjcSource());
	}

	for (auto const& cls : m_classes) {
		out += fmt::format("{}\n", cls.getObjcSource());
	}

	for (auto const& m : m_modules) {
		out += m.getObjcSource();
	}

	return out;
}

std::string ModuleFile::getBridgingHeader() const {
	return fmt::format("#include <{libraryName}_objc.h>",
	                   fmt::arg("libraryName", m_cache.m_moduleName));
}

std::filesystem::path ModuleFile::getObjcHeaderFile() const {
	return m_cache.m_moduleName + "_objc.h";
}

std::filesystem::path ModuleFile::getObjcSourceFile() const {
	return m_cache.m_moduleName + "_objc.mm";
}

std::filesystem::path ModuleFile::getBridgingHeaderFile() const {
	return m_cache.m_moduleName + "-Bridging-Header.h";
}

void ModuleFile::setCache(Objc::Cache const& cache) {
	m_cache = cache;
}

void ModuleFile::addEnum(Objc::Proxy::Enum const& e) {
	m_enums.push_back(e);
}

void ModuleFile::addClass(Objc::Proxy::Class const& c) {
	m_classes.push_back(c);
}

void ModuleFile::addModule(Objc::Proxy::Class const& m) {
	m_modules.push_back(m);
}

}    // namespace Objc::Proxy
