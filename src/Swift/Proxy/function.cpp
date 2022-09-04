#include "Swift/Proxy/function.hpp"
#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include "ObjcSwift/Helpers/string.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <string>

namespace Swift::Proxy {

std::string Function::getFunctionDeclaration() const {
	// public static func f() -> Int32
	return fmt::format(
	    R"(public {static}func {functionName}({arguments}){returnType})",
	    fmt::arg("static", m_isStatic ? "static " : ""),
	    fmt::arg("returnType",
	             m_returnType.empty() ? "" : " -> " + m_returnType),
	    fmt::arg("functionName", m_name),
	    fmt::arg("arguments", getArguments()));
}

std::string Function::getFunctionCall() const {
	return fmt::format(R"({callFrom}.{objcName}({arguments}))",
	                   fmt::arg("callFrom", m_callFrom),
	                   fmt::arg("objcName", m_objcName),
	                   fmt::arg("arguments", getArgumentNames()));
}

std::string Function::getFunctionBody() const {
	if (m_isConstructor) {
		return fmt::format(R"(m_object = {functionCall}({arguments}))",
		                   fmt::arg("functionCall", getFunctionCall()),
		                   fmt::arg("arguments", getArgumentNames()));
	}

	// Class function
	return fmt::format(
	    "{maybeReturn}{functionCall}",
	    fmt::arg("maybeReturn", m_returnType == "Void" ? "" : "return "),
	    fmt::arg("functionCall", getFunctionCall()));
}

std::string Function::getSwift() const {
	// public func f() -> String {
	//   return m_object.f()
	// }
	fmt::print("{}\n", "Calling getSwift()");
	auto swift = fmt::format(
	    R"(
extension {className} {{
  {declaration} {{
    {body}
  }}
}}
)",
	    fmt::arg("className", m_className),
	    fmt::arg("declaration", getFunctionDeclaration()),
	    fmt::arg("body", getFunctionBody()));
	fmt::print("{}\n", swift);
	return swift;
}

Function::Function(std::string const& name,
                   std::string const& objcName,
                   std::string const& extensionClassName)
    : m_name(name), m_objcName(objcName), m_className(extensionClassName),
      m_callFrom(), m_returnType(), m_arguments({}), m_isStatic(false),
      m_isConstructor(false) {}

void Function::addArgument(Argument const& argument) {
	m_arguments.push_back(argument);
};

void Function::setReturnType(std::string const& type) {
	m_returnType = type;
}

void Function::setAsStatic() {
	m_isStatic = true;
};

void Function::setAsConstructor() {
	m_isConstructor = true;
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
		}
		names.push_back(fmt::format(
		    "{pre}{name}", fmt::arg("pre", pre), fmt::arg("name", arg.name)));
		isFirst = false;
	}

	return fmt::format("{args}", fmt::arg("args", fmt::join(names, ", ")));
}

std::string Function::getArguments() const {
	// person: String, alreadyGreeted: Bool
	// _ condition: Bool, _ message: String = ""
	std::vector<std::string> out;
	for (auto const& arg : m_arguments) {
		out.push_back(fmt::format("_ {name}: {type}",
		                          fmt::arg("type", arg.type),
		                          fmt::arg("name", arg.name)));
	}
	return fmt::format("{}", fmt::join(out, ", "));
}

std::string Function::getName() const {
	return m_name;
}

void Function::setCallFrom(std::string const& callFrom) {
	m_callFrom = callFrom;
}
}    // namespace Swift::Proxy
