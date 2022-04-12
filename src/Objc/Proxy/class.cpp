#include "Objc/Proxy/class.hpp"
#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include <fmt/format.h>
#include <string>

namespace Objc::Proxy {

namespace {
std::string joinFunctions(std::vector<Objc::Proxy::Function> const& functions,
                          bool isSource) {
	std::string out;
	if (isSource) {
		for (auto const& f : functions) {
			out += f.getObjcSource();
		}
	} else {
		for (auto const& f : functions) {
			out += f.getObjcHeader();
		}
	}
	return out;
}
}    // namespace

std::string Class::getObjcSource() const {
	bool isSource = true;
	std::string out = fmt::format(
	    R"(
@implementation {className} {{
	// The corresponding C++ object
	std::unique_ptr<{fullyQualifiedName}> m_object;
}}
{constructors}
{functions}

@end)",
	    fmt::arg("className", m_name),
	    fmt::arg("fullyQualifiedName", m_fullyQualifiedName),
	    fmt::arg("constructors", joinFunctions(m_constructors, isSource)),
	    fmt::arg("functions", joinFunctions(m_functions, isSource)));

	return out;
}

std::string Class::getObjcHeader() const {
	bool isSource = false;
	std::string out = fmt::format(
	    R"(
@interface {className} : NSObject
{constructors}
{functions}

@end)",
	    fmt::arg("className", m_name),
	    fmt::arg("constructors", joinFunctions(m_constructors, isSource)),
	    fmt::arg("functions", joinFunctions(m_functions, isSource)));

	return out;
}

Class::Class(std::string const& name, std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName), m_constructors(),
      m_functions(), m_memberVariables(), m_enums(),
      m_isManagedByShared(false) {}

void Class::addEnum(Enum const& e) {
	m_enums.push_back(e);
}

void Class::addFunction(Function const& function) {
	m_functions.push_back(function);
}

void Class::addConstructor(Function const& constructor) {
	m_constructors.push_back(constructor);
}

void Class::addMemberVariable(std::string const& variableName,
                              std::string const& documentation,
                              bool isConst,
                              bool isStatic) {
	m_memberVariables.push_back(
	    {variableName, documentation, isConst, isStatic});
}

std::string const& Class::getName() const {
	return m_name;
}

void Class::setAsManagedByShared() {
	m_isManagedByShared = true;
}

void Class::setDocumentation(std::string const& documentation) {
	m_documentation = documentation;
}

void Class::setInherited(std::vector<std::string> const& inherited) {
	for (auto const& i : inherited) {
		m_inherited.push_back(i);
	}
}

}    // namespace Objc::Proxy
