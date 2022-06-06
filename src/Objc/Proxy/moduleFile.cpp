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

void ModuleFile::sortAllStructures() {
	// Is the only place m_allStructures should change
	if (!m_allStructures.empty()) {
		return;
	}
	m_allStructures.reserve(m_enums.size() + m_modules.size() +
	                        m_classes.size() + m_functions.size() +
	                        m_attributes.size());
	for (auto const& e : m_enums) {
		m_allStructures.push_back(&e);
	}

	for (auto const& m : m_modules) {
		m_allStructures.push_back(&m);
	}

	for (auto const& cls : m_classes) {
		m_allStructures.push_back(&cls);
	}

	for (auto const& f : m_functions) {
		m_allStructures.push_back(&f);
	}

	for (auto const& a : m_attributes) {
		m_allStructures.push_back(&a);
	}

	std::sort(m_allStructures.begin(),
	          m_allStructures.end(),
	          [](auto const* s0, auto const* s1) -> bool {
		          return s0->m_id < s1->m_id;
	          });
}

std::string ModuleFile::getObjcHeader() {
	sortAllStructures();
	std::string out = "#pragma once\n#import <Foundation/Foundation.h>";

	for (auto const& s : m_allStructures) {
		out += s->getObjcHeader();
	}

	return out;
}

std::string createExtraFunctionsSource(Objc::Cache const& cache) {
	std::string out;
	if (!cache.m_extraFunctions.empty() ||
	    !cache.m_extraClassConversions.empty()) {
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

std::string ModuleFile::getObjcSource() {
	sortAllStructures();
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

	for (auto const& structure : m_allStructures) {
		out += structure->getObjcSource();
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
