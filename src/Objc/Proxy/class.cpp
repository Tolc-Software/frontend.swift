#include "Objc/Proxy/class.hpp"
#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include <fmt/format.h>
#include <string>

namespace Objc::Proxy {

std::string getInitNamePadding(Objc::Proxy::Function const& constructor) {
	auto args = constructor.getArgumentsRaw();
	if (args.empty()) {
		return "";
	}
	std::string out = "With";
	return out;
}

std::string Class::getObjcSource(std::string const& moduleName) const {
	bool isSource = true;
	std::string out =
	    fmt::format(R"(
@implementation {moduleName}{className} {{

// The corresponding C++ object
std::unique_ptr<{fullyQualifiedName}> m_object;

}}

{constructors}

{functions}
@end)",
	                fmt::arg("moduleName", moduleName),
	                fmt::arg("className", m_name),
	                fmt::arg("fullyQualifiedName", m_fullyQualifiedName),
	                fmt::arg("constructors", getObjcConstructors()),
	                fmt::arg("functions", joinObjcFunctions(isSource)));

	return out;
}

std::string Class::getObjcConstructors() const {
	std::string out;
	for (auto const& constructor : m_constructors) {
		out += fmt::format(
		    R"(
-(instancetype)init{namePadding} {{
  if (self = [super init]) {{
    m_object = std::unique_ptr<{fullyQualifiedName}>(new {fullyQualifiedName}());
  }}
  return self;
}})",
		    fmt::arg("fullyQualifiedName", m_fullyQualifiedName),
		    fmt::arg("namePadding", getInitNamePadding(constructor)));
	}
	return out;
}

std::string Class::getObjcHeader(std::string const& moduleName) const {
	bool isSource = false;
	std::string out =
	    fmt::format(R"(
@interface {moduleName}{className} : NSObject

{functions}
@end)",
	                fmt::arg("moduleName", moduleName),
	                fmt::arg("className", m_name),
	                fmt::arg("functions", joinObjcFunctions(isSource)));

	return out;
}

std::string Class::joinObjcFunctions(bool isSource) const {
	bool isClassFunction = true;
	std::string out;
	if (isSource) {
		for (auto const& f : m_functions) {
			out += f.getObjcSource(isClassFunction);
		}
	} else {
		for (auto const& f : m_functions) {
			out += f.getObjcHeader(isClassFunction);
		}
	}
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
