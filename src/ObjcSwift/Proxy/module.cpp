#include "ObjcSwift/Proxy/module.hpp"
#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <string>

namespace ObjcSwift::Proxy {

std::string Module::getObjcSource() const {
	std::string out;
	for (auto const& cls : m_classes) {
		out += fmt::format("{}\n", cls.getObjcSource(m_variableName));
	}
	return out;
}

std::string Module::getObjcHeader() const {
	std::string out;
	for (auto const& cls : m_classes) {
		out += fmt::format("{}\n", cls.getObjcHeader(m_variableName));
	}
	return out;
}

std::string Module::getSwift() const {
	std::string out;
	for (auto const& cls : m_classes) {
		out += fmt::format("{}\n", cls.getSwift(m_variableName));
	}
	return out;
}

std::string Module::getObjcSwift() const {
	std::string out;
	for (auto const& cls : m_classes) {
		out += fmt::format("{}\n", cls.getObjcSwift(m_variableName));
	}

	for (auto const& e : m_enums) {
		out += fmt::format("{}\n", e.getObjcSwift(m_variableName));
	}

	for (auto const& attribute : m_attributes) {
		out += fmt::format("{}.{}\n", m_variableName, attribute.getObjcSwift());
	}

	for (auto const& function : m_functions) {
		out +=
		    fmt::format("\t{}.{};\n", m_variableName, function.getObjcSwift());
	}

	// Define all the children
	for (auto const& [submoduleName, submoduleVariable, documentation] :
	     m_submodules) {
		out += fmt::format(
		    "\tauto {submoduleVariable} = {variableName}.def_submodule(\"{submoduleName}\", {docs});\n",
		    fmt::arg("submoduleVariable", submoduleVariable),
		    fmt::arg("submoduleName", submoduleName),
		    fmt::arg("variableName", m_variableName),
		    fmt::arg(
		        "docs",
		        ObjcSwift::Helpers::getDocumentationParameter(documentation)));
	}

	return out;
}

Module::Module(std::string const& variableName)
    : m_variableName(variableName), m_submodules({}), m_functions(), m_enums(),
      m_attributes() {}

void Module::addFunction(Function const& function) {
	m_functions.push_back(function);
}

void Module::addClass(Class const& c) {
	m_classes.push_back(c);
}

void Module::addEnum(Enum const& e) {
	m_enums.push_back(e);
}

void Module::addAttribute(Attribute const& a) {
	m_attributes.push_back(a);
}

std::string const& Module::getVariableName() const {
	return m_variableName;
}

void Module::addSubmodule(std::string const& name,
                          std::string const& variableName,
                          std::string const& documentation) {
	m_submodules.push_back({name, variableName, documentation});
}

}    // namespace ObjcSwift::Proxy
