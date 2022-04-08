#include "ObjcSwift/Proxy/moduleFile.hpp"
#include <fmt/format.h>
#include <string>

namespace ObjcSwift::Proxy {

ModuleFile::ModuleFile(Module const& rootModule, std::string const& libraryName)
    : m_rootModuleName(rootModule.getVariableName()),
      m_libraryName(libraryName), m_modules({rootModule}), m_typeInfo() {}

void ModuleFile::addModule(Module const& m) {
	m_modules.push_back(m);
}

std::filesystem::path ModuleFile::getFilepath() const {
	return m_libraryName + "_swift.cpp";
}

void ModuleFile::setTypeInfo(ObjcSwift::Proxy::TypeInfo const& info) {
	m_typeInfo = info;
	m_typeInfo.m_includes.insert("#include <pybind11/pybind11.h>");
}

std::string ModuleFile::getObjcSwift() const {
	std::string out = R"()";

	// for (auto const& m : m_modules) {
	// 	out += m.getObjcSwift();
	// }

	return out;
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

std::string ModuleFile::getSwift() const {
	std::string out = fmt::format(R"(
public class {libraryName} {{
)",
	                              fmt::arg("libraryName", m_libraryName));

	for (auto const& m : m_modules) {
		out += m.getSwift();
	}

	return out + '}';
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

std::filesystem::path ModuleFile::getSwiftFile() const {
	return m_libraryName + "_swift.swift";
}

std::filesystem::path ModuleFile::getBridgingHeaderFile() const {
	return m_libraryName + "-Bridging-Header.h";
}

}    // namespace ObjcSwift::Proxy
