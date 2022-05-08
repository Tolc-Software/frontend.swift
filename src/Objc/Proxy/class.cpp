#include "Objc/Proxy/class.hpp"
#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include "ObjcSwift/Helpers/wrapInFunction.hpp"
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

std::string getUnderlyingObject(std::string const& fullyQualifiedName) {
	// NOTE: The object is actually only public for the implementation file
	//       Used for copy constructors
	return fmt::format(R"( {{
  // The corresponding C++ object
  @public std::unique_ptr<{fullyQualifiedName}> m_object;
}}

// Copy constructor
-(instancetype) Tolc_initWithCppObject:({fullyQualifiedName} const&)cppClass {{
  if (self = [super init]) {{
    m_object = std::unique_ptr<{fullyQualifiedName}>(new {fullyQualifiedName}(cppClass));
  }}
  return self;
}}

-({fullyQualifiedName}&) Tolc_getCppObject {{
  return *m_object.get();
}}
)",
	                   fmt::arg("fullyQualifiedName", fullyQualifiedName));
}

}    // namespace

std::string Class::getObjcSource() const {
	bool isSource = true;
	std::string out = fmt::format(
	    R"(
@implementation {className}{underlyingObject}
{constructors}
{functions}
{memberVariables}
@end)",
	    fmt::arg("className", m_name),
	    fmt::arg("underlyingObject",
	             m_isPurelyStatic ? "" :
                                    getUnderlyingObject(m_fullyQualifiedName)),
	    fmt::arg("constructors", joinFunctions(m_constructors, isSource)),
	    fmt::arg("functions", joinFunctions(m_functions, isSource)),
	    fmt::arg("memberVariables", joinMemberVariables(isSource)));

	return out;
}

std::string Class::getObjcHeader() const {
	bool isSource = false;
	std::string out = fmt::format(
	    R"(
@interface {className} : NSObject
{constructors}
{functions}
{memberVariables}
@end)",
	    fmt::arg("className", m_name),
	    fmt::arg("constructors", joinFunctions(m_constructors, isSource)),
	    fmt::arg("functions", joinFunctions(m_functions, isSource)),
	    fmt::arg("memberVariables", joinMemberVariables(isSource)));

	return out;
}

Class::Class(std::string const& name, std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName), m_constructors(),
      m_functions(), m_memberVariables(), m_enums(), m_isManagedByShared(false),
      m_isPurelyStatic(false) {}

void Class::addEnum(Enum const& e) {
	m_enums.push_back(e);
}

void Class::addFunction(Function const& function) {
	m_functions.push_back(function);
}

void Class::addConstructor(Function const& constructor) {
	m_constructors.push_back(constructor);
}

void Class::addMemberVariable(Attribute const& variable) {
	m_memberVariables.push_back(variable);
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

std::string Class::joinMemberVariables(bool isSource) const {
	std::string out;
	if (isSource) {
		for (auto const& variable : m_memberVariables) {
			out += variable.getObjcSource();
		}
	} else {
		for (auto const& variable : m_memberVariables) {
			out += variable.getObjcHeader();
		}
	}
	return out;
}

void Class::setAsPurelyStatic() {
	m_isPurelyStatic = true;
}

}    // namespace Objc::Proxy
