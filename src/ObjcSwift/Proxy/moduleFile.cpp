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

std::string getExtraFunctions(ObjcSwift::Proxy::TypeInfo const& typeInfo) {
	if (typeInfo.m_trampolineClasses.empty()) {
		return "";
	}
	return fmt::format(
	    R"_tolc_delimiter(
namespace {ns} {{
{joinedClasses}
}}
)_tolc_delimiter",
	    fmt::arg("ns", typeInfo.m_extraFunctionsNamespace),
	    fmt::arg("joinedClasses",
	             fmt::join(typeInfo.m_trampolineClasses, "\n")));
}

std::string ModuleFile::getObjcSwift() const {
	std::string out = fmt::format(
	    R"(
{includes}

namespace py = pybind11;
{extraFunctions}
PYBIND11_MODULE({libraryName}, {rootModuleName}))",
	    fmt::arg("includes", fmt::join(m_typeInfo.m_includes, "\n")),
	    fmt::arg("extraFunctions", getExtraFunctions(m_typeInfo)),
	    fmt::arg("libraryName", m_libraryName),
	    fmt::arg("rootModuleName", m_rootModuleName));

	out += " {\n";
	for (auto const& m : m_modules) {
		out += m.getObjcSwift();
	}
	out += '}';

	return out;
}

std::string ModuleFile::getObjcHeader() const {
	return "";
}

std::string ModuleFile::getObjcSource() const {
	return "";
}

std::string ModuleFile::getSwift() const {
	return "";
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

}    // namespace ObjcSwift::Proxy
