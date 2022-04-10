#include "Swift/Proxy/function.hpp"
#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include "ObjcSwift/Helpers/string.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <string>

namespace Swift::Proxy {

std::string Function::getFunctionCall() const {
	std::vector<std::string> names;
	bool isFirst = true;
	for (auto const& arg : m_arguments) {
		names.push_back(
		    fmt::format("{maybePre}{name}",
		                fmt::arg("maybePre", !isFirst ? arg.name + ": " : ""),
		                fmt::arg("name", arg.name)));
		isFirst = false;
	}

	return fmt::format("{name}({args})",
	                   fmt::arg("name", m_name),
	                   fmt::arg("args", fmt::join(names, ", ")));
}

std::string Function::getSwift(bool isClassFunction) const {
	if (isClassFunction) {
		// public func f() -> String { }
		return fmt::format(
		    R"(
public func {name}({arguments}) -> {returnType} {{
	{maybeReturn}m_object.{functionCall};
}}
		)",
		    fmt::arg("name", m_name),
		    fmt::arg("arguments", getArguments()),
		    fmt::arg("returnType", m_returnType),
		    fmt::arg("maybeReturn", m_returnType == "void" ? "" : "return "),
		    fmt::arg("functionCall", getFunctionCall()));
	}
	return "// Swift bare function not implemented";
}

Function::Function(std::string const& name,
                   std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName), m_returnType(),
      m_arguments({}), m_isConstructor(false), m_isOverloaded(false),
      m_isStatic(false) {}

void Function::addArgument(Argument const& argument) {
	m_arguments.push_back(argument);
};

void Function::setReturnType(std::string const& type) {
	m_returnType = type;
}

void Function::setAsConstructor() {
	m_isConstructor = true;
};

void Function::setAsStatic() {
	m_isStatic = true;
};

void Function::setAsOverloaded() {
	m_isOverloaded = true;
};

void Function::setDocumentation(std::string const& documentation) {
	m_documentation = documentation;
}

std::string Function::getArgumentNames() const {
	// Get the typenames of the arguments
	std::vector<std::string> names;
	std::transform(m_arguments.begin(),
	               m_arguments.end(),
	               std::back_inserter(names),
	               [=](auto const& argument) { return argument.name; });
	return fmt::format("{}", fmt::join(names, ", "));
}

std::string Function::getArguments() const {
	// person: String, alreadyGreeted: Bool
	std::vector<std::string> out;
	for (auto const& arg : m_arguments) {
		out.push_back(fmt::format("{name}: {type}",
		                          fmt::arg("type", arg.type),
		                          fmt::arg("name", arg.name)));
	}
	return fmt::format("{}", fmt::join(out, ", "));
}

std::vector<Function::Argument> const& Function::getArgumentsRaw() const {
	return m_arguments;
}

std::string Function::getName() const {
	return m_name;
}

}    // namespace Swift::Proxy
