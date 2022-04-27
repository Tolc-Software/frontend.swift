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
{memberVariables}
@end)",
	    fmt::arg("className", m_name),
	    fmt::arg("fullyQualifiedName", m_fullyQualifiedName),
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

void Class::addMemberVariable(MemberVariable const& variable) {
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

std::string getPropertyOptions(Class::MemberVariable const& v) {
	std::vector<std::string> options;
	if (v.m_isConst) {
		options.push_back("readonly");
	} else {
		// Add a setter
		options.push_back(fmt::format("setter={}:", v.m_name));
	}

	if (v.m_isStatic) {
		options.push_back("class");
	}

	return fmt::format("({})", fmt::join(options, ", "));
}

std::string Class::joinMemberVariables(bool isSource) const {
	std::string out;
	if (isSource) {
		for (auto const& variable : m_memberVariables) {
			std::string staticAttribute = "-";
			std::string callAccess = "m_object->";
			if (variable.m_isStatic) {
				staticAttribute = "+";
				callAccess = m_fullyQualifiedName + "::";
			}
			// Maybe need a conversion to Objective-C type
			auto call = ObjcSwift::Helpers::wrapInFunction(
			    callAccess + variable.m_name,
			    variable.m_type.m_conversions.m_toObjc);
			out += fmt::format(R"(
{staticAttribute}({type}) {name} {{
  return {call};
}}
)",
			                   fmt::arg("staticAttribute", staticAttribute),
			                   fmt::arg("type", variable.m_type.m_name),
			                   fmt::arg("name", variable.m_name),
			                   fmt::arg("call", call));
			if (!variable.m_isConst) {
				// Not const -> Add a setter
				// Maybe need a conversion to C++ type
				auto convertedVariable = ObjcSwift::Helpers::wrapInFunction(
				    "new" + variable.m_name,
				    variable.m_type.m_conversions.m_toCpp);
				out += fmt::format(
				    R"(
{staticAttribute}(void) {name}:({type})new{name} {{
  {callAccess}{name} = {convertedVariable};
}}
)",
				    fmt::arg("staticAttribute", staticAttribute),
				    fmt::arg("name", variable.m_name),
				    fmt::arg("type", variable.m_type.m_name),
				    fmt::arg("callAccess", callAccess),
				    fmt::arg("convertedVariable", convertedVariable));
			}
		}
	} else {
		for (auto const& variable : m_memberVariables) {
			out +=
			    fmt::format("\n@property {options} {type} {name};\n",
			                fmt::arg("options", getPropertyOptions(variable)),
			                fmt::arg("name", variable.m_name),
			                fmt::arg("type", variable.m_type.m_name));
		}
	}
	return out;
}

}    // namespace Objc::Proxy
