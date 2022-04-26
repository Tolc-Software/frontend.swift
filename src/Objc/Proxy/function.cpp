#include "Objc/Proxy/function.hpp"
#include "Objc/Proxy/type.hpp"
#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include "ObjcSwift/Helpers/string.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <string>

namespace Objc::Proxy {

std::string Function::getFunctionDeclaration() const {
	if (m_isClassFunction) {
		// - (int)add:(int)x y:(int)y;
		return fmt::format(
		    R"({static} ({returnType}){functionName}{arguments})",
		    fmt::arg("static", m_isStatic ? "+" : "-"),
		    fmt::arg("returnType", m_returnType.m_name),
		    fmt::arg("functionName", m_name),
		    fmt::arg("arguments",
		             m_objCDeclArgs.empty() ? m_objCDeclArgs :
                                              ':' + m_objCDeclArgs));
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
			    fmt::arg("arguments", m_cppCallArgs));
		}
		return fmt::format("m_object->{name}({arguments})",
		                   fmt::arg("name", m_name),
		                   fmt::arg("arguments", m_cppCallArgs));
	}
	return m_name + '(' + m_cppCallArgs + ')';
}

std::string Function::getFunctionBody() const {
	if (m_isClassFunction) {
		if (m_isConstructor) {
			return fmt::format(
			    R"(  if (self = [super init]) {{
    m_object = std::unique_ptr<{qualifiedClassName}>(new {qualifiedClassName}({arguments}));
  }}
  return self;)",
			    fmt::arg("qualifiedClassName", m_fullyQualifiedClassName),
			    fmt::arg("arguments", m_cppCallArgs),
			    fmt::arg("name", m_name));
		}

		// Class function
		return fmt::format(
		    "    {maybeReturn}{functionCall};",
		    fmt::arg("maybeReturn",
		             m_returnType.m_name == "void" ? "" : "return "),
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
      m_isOverloaded(false), m_isStatic(false), m_isClassFunction(false),
      m_isConstructor(false) {}

void Function::setReturnType(Objc::Proxy::Type const& type) {
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

void Function::setCppCallArguments(std::string const& arguments) {
	m_cppCallArgs = arguments;
}

void Function::setObjCDeclarationArguments(std::string const& arguments) {
	m_objCDeclArgs = arguments;
}

}    // namespace Objc::Proxy
