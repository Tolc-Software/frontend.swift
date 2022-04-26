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
		for (auto const& m : m_memberVariables) {
			auto stat = m.m_isStatic ? "+" : "-";
			// How to access the variable
			auto access =
			    m.m_isStatic ? m_fullyQualifiedName + "::" : "m_object->";
			out += fmt::format(R"(
{static}({type}) {name} {{
  return {access}{name};
}}
)",
			                   fmt::arg("static", stat),
			                   fmt::arg("type", m.m_type.m_name),
			                   fmt::arg("name", m.m_name),
			                   fmt::arg("access", access));
			if (!m.m_isConst) {
				// Not const -> Add a setter
				out += fmt::format(R"(
{static}(void) {name}:({type})new{name} {{
  {access}{name} = new{name};
}}
)",
				                   fmt::arg("static", stat),
				                   fmt::arg("name", m.m_name),
				                   fmt::arg("type", m.m_type.m_name),
				                   fmt::arg("access", access));
			}
		}
	} else {
		for (auto const& m : m_memberVariables) {
			out += fmt::format("\n@property {options} {type} {name};\n",
			                   fmt::arg("options", getPropertyOptions(m)),
			                   fmt::arg("name", m.m_name),
			                   fmt::arg("type", m.m_type.m_name));
		}
	}
	return out;
}

}    // namespace Objc::Proxy
