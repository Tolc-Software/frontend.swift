#include "Objc/Proxy/moduleFile.hpp"
#include "Objc/Proxy/attribute.hpp"
#include "Objc/Proxy/class.hpp"
#include "Objc/Proxy/enum.hpp"
#include "Objc/Proxy/function.hpp"
#include "Objc/cache.hpp"
#include <fmt/format.h>
#include <string>

namespace Objc::Proxy {

ModuleFile::ModuleFile() : m_modules(), m_classes(), m_enums(), m_cache() {}

std::string ModuleFile::getObjcHeader() const {
	std::string out = "#pragma once\n#import <Foundation/Foundation.h>";

	for (auto const& e : m_enums) {
		out += fmt::format("{}\n", e.getObjcHeader());
	}

	for (auto const& m : m_modules) {
		out += m.getObjcHeader();
	}

	for (auto const& cls : m_classes) {
		out += fmt::format("{}\n", cls.getObjcHeader());
	}

	for (auto const& f : m_functions) {
		out += f.getObjcHeader();
	}

	for (auto const& a : m_attributes) {
		out += a.getObjcHeader();
	}

	return out;
}

std::string createExtraFunctionsSource(Objc::Cache const& cache) {
	std::string out;
	if (!cache.m_extraFunctions.empty()) {
		out += fmt::format(R"(
namespace {} {{
{}
{}

}})",
		                   cache.m_extraFunctionsNamespace,
		                   fmt::join(cache.m_extraClassConversions, "\n"),
		                   fmt::join(cache.m_extraFunctions, "\n"));
	}
	return out;
}

std::string ModuleFile::getObjcSource() const {
	std::string out = fmt::format(
	    R"(
#import "{libraryName}_objc.h"
#import <Foundation/Foundation.h>
#include <memory>
{classDeclarations}
{conversions}
)",
	    fmt::arg("classDeclarations",
	             fmt::join(m_cache.m_extraClassCategories, "\n")),
	    fmt::arg("conversions", createExtraFunctionsSource(m_cache)),
	    fmt::arg("libraryName", m_cache.m_moduleName));

	for (auto const& e : m_enums) {
		out += e.getObjcSource();
	}

	for (auto const& m : m_modules) {
		out += m.getObjcSource();
	}

	for (auto const& cls : m_classes) {
		out += cls.getObjcSource();
	}

	for (auto const& f : m_functions) {
		out += f.getObjcSource();
	}

	for (auto const& a : m_attributes) {
		out += a.getObjcSource();
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

void ModuleFile::addFunction(Objc::Proxy::Function const& f) {
	m_functions.push_back(f);
}

void ModuleFile::addAttribute(Objc::Proxy::Attribute const& v) {
	m_attributes.push_back(v);
}

}    // namespace Objc::Proxy
