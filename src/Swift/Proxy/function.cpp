#include "Swift/Proxy/function.hpp"
#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include "ObjcSwift/Helpers/string.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <string>

namespace Swift::Proxy {

std::string Function::getFunctionDeclaration() const {
	if (m_isClassFunction) {
		if (m_isConstructor) {
			return fmt::format(R"(public{static} {functionName}({arguments}))",
			                   fmt::arg("static", m_isStatic ? "static " : ""),
			                   fmt::arg("functionName", m_name),
			                   fmt::arg("arguments", getArguments()));
		}
		// public static func f() -> Int32
		return fmt::format(
		    R"(public {static}func {functionName}({arguments}) -> {returnType})",
		    fmt::arg("static", m_isStatic ? "static " : ""),
		    fmt::arg("returnType", m_returnType),
		    fmt::arg("functionName", m_name),
		    fmt::arg("arguments", getArguments()));
	}
	return "// Swift bare function not implemented";
}

std::string Function::getFunctionCall() const {
	if (m_isClassFunction) {
		if (m_isStatic) {
			return fmt::format(R"({objcClassName}.{name}({arguments}))",
			                   fmt::arg("objcClassName", m_objcClassName),
			                   fmt::arg("name", m_name),
			                   fmt::arg("arguments", getArgumentNames()));
		}
		// Class function
		return fmt::format("m_object.{name}({arguments})",
		                   fmt::arg("name", m_name),
		                   fmt::arg("arguments", getArgumentNames()));
	}
	return fmt::format("{name}({args})",
	                   fmt::arg("name", m_name),
	                   fmt::arg("args", getArgumentNames()));
}

std::string Function::getFunctionBody() const {
	if (m_isClassFunction) {
		if (m_isConstructor) {
			return fmt::format(R"(m_object = {objcClassName}({arguments}))",
			                   fmt::arg("objcClassName", m_objcClassName),
			                   fmt::arg("arguments", getArgumentNames()));
		}

		// Class function
		return fmt::format(
		    "{maybeReturn}{functionCall};",
		    fmt::arg("maybeReturn", m_returnType == "Void" ? "" : "return "),
		    fmt::arg("functionCall", getFunctionCall()));
	}
	// Simple function
	return "// Swift function body not implemented";
}

std::string Function::getSwift() const {
	// public func f() -> String {
	//   return m_object.f()
	// }
	return fmt::format(
	    R"(
    {declaration} {{
        {body}
    }}
)",
	    fmt::arg("declaration", getFunctionDeclaration()),
	    fmt::arg("body", getFunctionBody()));
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
	std::vector<std::string> names;
	// The first argument is special in Objc
	bool isFirst = true;
	for (auto const& arg : m_arguments) {
		std::string pre = arg.name + ": ";
		if (isFirst) {
			pre = "";
			// Constructors with parameters are special in Objc
			// This is how the automatic Objc->Swift translation
			// works for initWithXXX in Objc
			if (m_isConstructor) {
				pre = m_objcFirstConstructorParam + ": ";
			}
		}
		names.push_back(fmt::format(
		    "{pre}{name}", fmt::arg("pre", pre), fmt::arg("name", arg.name)));
		isFirst = false;
	}

	return fmt::format("{args}", fmt::arg("args", fmt::join(names, ", ")));
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

void Function::setAsClassFunction(std::string const& objcClassName) {
	m_objcClassName = objcClassName;
	m_isClassFunction = true;
}

void Function::setAsConstructor() {
	m_isConstructor = true;
}

void Function::setObjcFirstConstructorParameter(
    std::string const& variableName) {
	m_objcFirstConstructorParam = variableName;
}
}    // namespace Swift::Proxy
