#include "Objc/Proxy/module.hpp"
#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <string>

namespace Objc::Proxy {

std::string Module::getObjcSource() const {
	std::string out;
	for (auto const& cls : m_classes) {
		out += fmt::format("{}\n", cls.getObjcSource());
	}
	return out;
}

std::string Module::getObjcHeader() const {
	std::string out;
	for (auto const& cls : m_classes) {
		out += fmt::format("{}\n", cls.getObjcHeader());
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

}    // namespace Objc::Proxy
