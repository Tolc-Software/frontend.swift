#include "ObjcSwift/Proxy/function.hpp"
#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include "ObjcSwift/Helpers/string.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <string>

namespace ObjcSwift::Proxy {

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

std::string Function::getFunctionCall(Language lang) const {
	switch (lang) {
		case Language::Swift: {
			std::vector<std::string> names;
			bool isFirst = true;
			for (auto const& arg : m_arguments) {
				names.push_back(fmt::format(
				    "{maybePre}{name}",
				    fmt::arg("maybePre", !isFirst ? arg.name + ": " : ""),
				    fmt::arg("name", arg.name)));
				isFirst = false;
			}

			return fmt::format("{name}({args})",
			                   fmt::arg("name", m_name),
			                   fmt::arg("args", fmt::join(names, ", ")));
		}
		case Language::Objc: {
			return m_name + '(' + getArgumentNames() + ')';
		}
	}
	return "";
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
		                 m_returnType.cppName,
		                 m_name,
		                 getArguments(Language::Objc))),
		    fmt::arg("maybeReturn",
		             m_returnType.cppName == "void" ? "" : "return "),
		    fmt::arg("functionCall", getFunctionCall(Language::Objc)));
	}
	return "// ObjcSource bare function not implemented";
}

std::string Function::getObjcHeader(bool isClassFunction) const {
	if (isClassFunction) {
		// - (int)add:(int)x y:(int)y;
		return Objc::getClassFunctionDeclaration(
		           m_returnType.cppName, m_name, getArguments(Language::Objc)) +
		       ';';
	}
	return "// ObjcHeader bare function not implemented";
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
		    fmt::arg("arguments", getArguments(Language::Swift)),
		    fmt::arg("returnType", m_returnType.swiftName),
		    fmt::arg("maybeReturn",
		             m_returnType.cppName == "void" ? "" : "return "),
		    fmt::arg("functionCall", getFunctionCall(Language::Swift)));
	}
	return "// Swift bare function not implemented";
}

std::string Function::getObjcSwift() const {
	std::string f;
	if (m_isConstructor) {
		// Results in
		// def(py::init<std::string, int, double>), "This is a constructor"
		f = fmt::format(
		    "def(py::init<{}>(), {}",
		    getArgumentTypes(),
		    ObjcSwift::Helpers::getDocumentationParameter(m_documentation));
	} else {
		// Results in
		// def("myFunction", (void (*)(int, double))&MyNamespace::myFunction, "This is a function"
		f = fmt::format(
		    R"(def{static}("{name}", {overload}&{fqName}, {docs})",
		    fmt::arg("static", m_isStatic ? "_static" : ""),
		    fmt::arg("name", m_name),
		    fmt::arg("overload", m_isOverloaded ? getSignature() : ""),
		    fmt::arg("fqName", m_fullyQualifiedName),
		    fmt::arg("docs",
		             ObjcSwift::Helpers::getDocumentationParameter(
		                 m_documentation)));
	}

	// Add named arguments
	// Allows for myFun(i=5, j=2) in python
	// If all arguments names are specified in C++, we can let the python user see them
	if (std::none_of(m_arguments.begin(),
	                 m_arguments.end(),
	                 [](const auto& arg) { return arg.name.empty(); })) {
		for (auto const& arg : m_arguments) {
			f += fmt::format(R"(, py::arg("{}"))", arg.name);
		}
	}
	f += ')';

	return f;
}

Function::Function(std::string const& name,
                   std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName), m_returnType(),
      m_arguments({}), m_isConstructor(false), m_isOverloaded(false),
      m_isStatic(false) {}

void Function::addArgument(Argument const& argument) {
	m_arguments.push_back(argument);
};

void Function::setReturnType(ObjcSwift::Type const& type) {
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

std::string Function::getArguments(Language lang) const {
	switch (lang) {
		case Language::Swift: {
			// person: String, alreadyGreeted: Bool
			std::vector<std::string> out;
			for (auto const& arg : m_arguments) {
				out.push_back(fmt::format("{name}: {type}",
				                          fmt::arg("type", arg.type.swiftName),
				                          fmt::arg("name", arg.name)));
			}
			return fmt::format("{}", fmt::join(out, ", "));
		}
		case Language::Objc: {
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
				                   fmt::arg("type", arg.type.cppName),
				                   fmt::arg("name", arg.name));
			}
			return out;
		}
	}
	return "";
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
		               return withNames ?
                                  argument.type.cppName + " " + argument.name :
                                  argument.type.cppName;
	               });
	return fmt::format("{}", fmt::join(typeNames, ", "));
}

std::string Function::getSignature() const {
	return fmt::format(R"(({returnType}({namespace}*)({arguments})))",
	                   fmt::arg("returnType", m_returnType.cppName),
	                   fmt::arg("namespace",
	                            ObjcSwift::Helpers::removeSubString(
	                                m_fullyQualifiedName, m_name)),
	                   fmt::arg("arguments", getArgumentTypes()));
}

std::string Function::getName() const {
	return m_name;
}

ObjcSwift::Type Function::getReturnType() const {
	return m_returnType;
}

}    // namespace ObjcSwift::Proxy
