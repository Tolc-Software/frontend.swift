#pragma once

#include <optional>
#include <string>
#include <vector>

namespace Swift::Proxy {
class Function {
public:
	Function(std::string const& name,
	         std::string const& objcName,
	         std::string const& className);

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

	void setAsStatic();

	void setAsConstructor();

	void setReturnType(std::string const& type);

	std::string getName() const;

	std::string getArgumentNames() const;

private:
	std::string getArguments() const;

	std::string getFunctionCall() const;
	std::string getFunctionDeclaration() const;
	std::string getFunctionBody() const;

	// User defined name of the function
	std::string m_name;
	std::string m_objcName;
	// The name of the class this function will be added to
	std::string m_className;

	std::string m_documentation;
	std::string m_returnType;

	std::vector<Argument> m_arguments;
	bool m_isStatic;
	bool m_isConstructor;
};
}    // namespace Swift::Proxy
