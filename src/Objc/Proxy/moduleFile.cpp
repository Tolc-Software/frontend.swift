#include "Objc/Proxy/moduleFile.hpp"
#include <fmt/format.h>
#include <string>

namespace Objc::Proxy {

ModuleFile::ModuleFile(Module const& rootModule, std::string const& libraryName)
    : m_rootModuleName(rootModule.getVariableName()),
      m_libraryName(libraryName), m_modules({rootModule}) {}

void ModuleFile::addModule(Module const& m) {
	m_modules.push_back(m);
}

std::filesystem::path ModuleFile::getFilepath() const {
	return m_libraryName + "_swift.cpp";
}

std::string ModuleFile::getObjcHeader() const {
	std::string out =
	    R"(
#import <Foundation/Foundation.h>

)";

	for (auto const& m : m_modules) {
		out += m.getObjcHeader();
	}

	return out;
}

std::string ModuleFile::getObjcSource() const {
	std::string out = fmt::format(R"(
#include "{libraryName}_objc.h"
#import <Foundation/Foundation.h>
#include <memory>

)",
	                              fmt::arg("libraryName", m_libraryName));

	for (auto const& m : m_modules) {
		out += m.getObjcSource();
	}

	return out;
}

std::string ModuleFile::getBridgingHeader() const {
	return fmt::format("#include <{libraryName}_objc.h>",
	                   fmt::arg("libraryName", m_libraryName));
}

std::filesystem::path ModuleFile::getObjcHeaderFile() const {
	return m_libraryName + "_objc.h";
}

std::filesystem::path ModuleFile::getObjcSourceFile() const {
	return m_libraryName + "_objc.mm";
}

std::filesystem::path ModuleFile::getBridgingHeaderFile() const {
	return m_libraryName + "-Bridging-Header.h";
}

}    // namespace Objc::Proxy
