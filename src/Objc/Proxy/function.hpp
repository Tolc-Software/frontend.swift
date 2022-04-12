#pragma once

#include "ObjcSwift/Type.hpp"
#include <optional>
#include <string>
#include <vector>

namespace Objc::Proxy {
class Function {
public:
	Function(std::string const& name, std::string const& fullyQualifiedName);

	std::string getObjcSource() const;
	std::string getObjcHeader() const;

	struct Argument {
		// E.g.
		//   f(int i)
		// Would result in
		//   typeName = "int"
		//   name = "i"
		std::string name;
		std::string type;
	};

	/**
	* Adds an argument name.
	* E.g.
	*   f(int i) would require addArgument("int", "i")
	*/
	void addArgument(Argument const& argument);

	/**
	* Documentation for the corresponding C++ function
	* E.g. Doxygen comment
	*/
	void setDocumentation(std::string const& documentation);

	// Is there another function with the same name but with different arguments?
	void setAsOverloaded();

	void setAsStatic();

	void setAsClassFunction(std::string const& fullyQualifiedClassName);

	void setAsConstructor();

	void setReturnType(std::string const& type);

	std::string getName() const;

	std::string getArgumentTypes(bool withNames = false) const;
	std::string getArgumentNames() const;

private:
	std::string getArguments() const;

	std::string getFunctionCall() const;
	std::string getFunctionDeclaration() const;
	std::string getFunctionBody() const;

	// User defined name of the function
	std::string m_name;
	std::string m_fullyQualifiedName;
	std::string m_documentation;
	// Defaults to void
	std::string m_returnType;
	std::string m_fullyQualifiedClassName;
	std::vector<Argument> m_arguments;
	bool m_isOverloaded;
	bool m_isStatic;
	bool m_isClassFunction;
	bool m_isConstructor;
};
}    // namespace Objc::Proxy
