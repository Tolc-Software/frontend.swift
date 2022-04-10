#include "Objc/Proxy/function.hpp"
#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include "ObjcSwift/Helpers/string.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <string>

namespace Objc::Proxy {

namespace {
namespace Objc {

std::string getClassFunctionDeclaration(std::string const& returnType,
                                        std::string const& name,
                                        std::string const& arguments) {
	return fmt::format(
	    R"(- ({returnType}){functionName}{arguments})",
	    fmt::arg("returnType", returnType),
	    fmt::arg("functionName", name),
	    fmt::arg("arguments", arguments.empty() ? arguments : ':' + arguments));
}
}    // namespace Objc
}    // namespace

std::string Function::getFunctionCall() const {
	return m_name + '(' + getArgumentNames() + ')';
}

std::string Function::getObjcSource(bool isClassFunction) const {
	if (isClassFunction) {
		return fmt::format(
		    R"(
{functionDeclaration} {{
	{maybeReturn}m_object->{functionCall};
}})",
		    fmt::arg("functionDeclaration",
		             Objc::getClassFunctionDeclaration(
		                 m_returnType, m_name, getArguments())),
		    fmt::arg("maybeReturn", m_returnType == "void" ? "" : "return "),
		    fmt::arg("functionCall", getFunctionCall()));
	}
	return "// ObjcSource bare function not implemented";
}

std::string Function::getObjcHeader(bool isClassFunction) const {
	if (isClassFunction) {
		// - (int)add:(int)x y:(int)y;
		return Objc::getClassFunctionDeclaration(
		           m_returnType, m_name, getArguments()) +
		       ';';
	}
	return "// ObjcHeader bare function not implemented";
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
	return out;
}

std::vector<Function::Argument> const& Function::getArgumentsRaw() const {
	return m_arguments;
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

std::string Function::getSignature() const {
	return fmt::format(R"(({returnType}({namespace}*)({arguments})))",
	                   fmt::arg("returnType", m_returnType),
	                   fmt::arg("namespace",
	                            ObjcSwift::Helpers::removeSubString(
	                                m_fullyQualifiedName, m_name)),
	                   fmt::arg("arguments", getArgumentTypes()));
}

std::string Function::getName() const {
	return m_name;
}

}    // namespace Objc::Proxy
