#pragma once

#include "ObjcSwift/Type.hpp"
#include <optional>
#include <string>
#include <vector>

namespace Swift::Proxy {
class Function {
public:
	Function(std::string const& name, std::string const& fullyQualifiedName);

	std::string getSwift() const;

	struct Argument {
		// E.g.
		//   f(int i)
		// Would result in
		//   typeName = "Int32"
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

	void setAsClassFunction(std::string const& objcClassName);

	void setAsConstructor();

	void setReturnType(std::string const& type);

	std::string getName() const;

	std::string getArgumentTypes(bool withNames = false) const;
	std::string getArgumentNames() const;
	std::vector<Argument> const& getArgumentsRaw() const;

	/**
	* An overloaded init with parameters is named
	*   initWithIntDouble(int i, double d)
	* for a constructor which takes an int and a double
	* To call this constructor from Swift you need to know how the first variable is called
	* as it takes on whatever comes after 'With' in the init name
	* In this case:
	*   MyClass(intDouble: 5, d: 1.0)
	* The input to this function is therefore 'intDouble'
	* Wild.
	*/
	void setObjcFirstConstructorParameter(std::string const& variableName);

private:
	std::string getSignature() const;
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

	// Is empty when not a class function
	std::string m_objcClassName;

	std::string m_objcFirstConstructorParam;
	std::vector<Argument> m_arguments;
	bool m_isOverloaded;
	bool m_isStatic;
	bool m_isClassFunction;
	bool m_isConstructor;
};
}    // namespace Swift::Proxy
