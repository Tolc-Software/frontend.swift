#include "Swift/Proxy/class.hpp"
#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include <fmt/format.h>
#include <string>

namespace Swift::Proxy {

std::string Class::getSwift(std::string const& moduleName) const {
	std::string out = fmt::format(R"(
public class {className} {{
    private var m_object: {moduleName}{className}

    public init() {{
        m_object = {moduleName}{className}()
    }}

{functions}
}})",
	                              fmt::arg("moduleName", moduleName),
	                              fmt::arg("className", m_name),
	                              fmt::arg("functions", joinSwiftFunctions()));

	return out;
}

std::string Class::joinSwiftFunctions() const {
	bool isClassFunction = true;
	std::string out;
	for (auto const& f : m_functions) {
		out += f.getSwift(isClassFunction);
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

}    // namespace Swift::Proxy
