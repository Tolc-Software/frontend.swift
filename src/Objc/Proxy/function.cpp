#include "Objc/Proxy/function.hpp"
#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include "ObjcSwift/Helpers/string.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <string>

namespace Objc::Proxy {

std::string Function::getFunctionDeclaration() const {
	if (m_isClassFunction) {
		// - (int)add:(int)x y:(int)y;
		auto arguments = getArguments();
		return fmt::format(
		    R"({static} ({returnType}){functionName}{arguments})",
		    fmt::arg("static", m_isStatic ? "+" : "-"),
		    fmt::arg("returnType", m_returnType),
		    fmt::arg("functionName", m_name),
		    fmt::arg("arguments",
		             arguments.empty() ? arguments : ':' + arguments));
	}
	return "// Objc function declaration for bare function not implemented";
}

std::string Function::getFunctionCall() const {
	if (m_isClassFunction) {
		if (m_isStatic) {
			return fmt::format(
			    R"({fullyQualifiedClassName}::{name}({arguments}))",
			    fmt::arg("fullyQualifiedClassName", m_fullyQualifiedClassName),
			    fmt::arg("name", m_name),
			    fmt::arg("arguments", getArgumentNames()));
		}
		return fmt::format("m_object->{name}({arguments})",
		                   fmt::arg("name", m_name),
		                   fmt::arg("arguments", getArgumentNames()));
	}
	return m_name + '(' + getArgumentNames() + ')';
}

std::string Function::getFunctionBody() const {
	if (m_isClassFunction) {
		if (m_isConstructor) {
			return fmt::format(
			    R"(    if (self = [super init]) {{
        m_object = std::unique_ptr<{qualifiedClassName}>(new {qualifiedClassName}({arguments}));
    }}
    return self;)",
			    fmt::arg("qualifiedClassName", m_fullyQualifiedClassName),
			    fmt::arg("arguments", getArgumentNames()),
			    fmt::arg("name", m_name));
		}

		// Class function
		return fmt::format(
		    "    {maybeReturn}{functionCall};",
		    fmt::arg("maybeReturn", m_returnType == "void" ? "" : "return "),
		    fmt::arg("functionCall", getFunctionCall()));
	}
	// Simple function
	return "// Objc function body not implemented";
}

std::string Function::getObjcSource() const {
	return fmt::format(
	    R"(
{declaration} {{
{body}
}}
)",
	    fmt::arg("declaration", getFunctionDeclaration()),
	    fmt::arg("body", getFunctionBody()));
}

std::string Function::getObjcHeader() const {
	// E.g.
	// - (int)add:(int)x y:(int)y;
	return '\n' + getFunctionDeclaration() + ";\n";
}

Function::Function(std::string const& name,
                   std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName), m_returnType(),
      m_arguments({}), m_isOverloaded(false), m_isStatic(false),
      m_isClassFunction(false), m_isConstructor(false) {}

void Function::addArgument(Argument const& argument) {
	m_arguments.push_back(argument);
};

void Function::setReturnType(std::string const& type) {
	m_returnType = type;
}

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
	// (int)x y:(int)y z:(int)z
	// Get the typenames of the arguments
	// The first one doesn't start with a name
	bool isFirst = true;
	std::string out;
	for (auto const& arg : m_arguments) {
		if (!isFirst) {
			out += arg.name + ':';
		}
		isFirst = false;
		out += fmt::format("({type}){name} ",
		                   fmt::arg("type", arg.type),
		                   fmt::arg("name", arg.name));
	}
	if (!m_arguments.empty()) {
		// Remove the last space
		out.pop_back();
	}
	return out;
}

std::string Function::getArgumentTypes(bool withNames) const {
	// Get the typenames of the arguments
	// The first one doesn't start with a name
	std::vector<std::string> typeNames;
	std::transform(m_arguments.begin(),
	               m_arguments.end(),
	               std::back_inserter(typeNames),
	               [=](auto const& argument) {
		               return withNames ? argument.type + " " + argument.name :
                                          argument.type;
	               });
	return fmt::format("{}", fmt::join(typeNames, ", "));
}

std::string Function::getName() const {
	return m_name;
}

void Function::setAsClassFunction(std::string const& fullyQualifiedClassName) {
	m_fullyQualifiedClassName = fullyQualifiedClassName;
	m_isClassFunction = true;
}

void Function::setAsConstructor() {
	m_isConstructor = true;
}
}    // namespace Objc::Proxy
