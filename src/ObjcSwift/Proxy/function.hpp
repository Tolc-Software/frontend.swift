#pragma once

#include "ObjcSwift/Type.hpp"
#include <optional>
#include <string>
#include <vector>

namespace ObjcSwift::Proxy {
class Function {
public:
	Function(std::string const& name, std::string const& fullyQualifiedName);

	/**
	* Creates a string corresponding to the pybind11 conversion of this function.
	* E.g.
	*   def("myFunction", &SomeNamespace::myFunction)
	*/
	std::string getObjcSwift() const;

	std::string getObjcSource(bool isClassFunction) const;
	std::string getObjcHeader(bool isClassFunction) const;

	std::string getSwift(bool isClassFunction) const;

	struct Argument {
		// E.g.
		//   f(int i)
		// Would result in
		//   typeName = "int"
		//   name = "i"
		std::string name;
		ObjcSwift::Type type;
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

	void setAsConstructor();

	// Is there another function with the same name but with different arguments?
	void setAsOverloaded();

	void setAsStatic();

	void setReturnType(ObjcSwift::Type const& type);

	// As it will be called in python
	void setPythonName(std::string const& name);
	std::string getPythonName() const;

	std::string getName() const;

	ObjcSwift::Type getReturnType() const;

	std::string getArgumentTypes(bool withNames = false) const;
	std::string getArgumentNames() const;

private:
	enum class Language { Objc, Swift };
	std::string getSignature() const;
	std::string getArguments(Language lang) const;

	std::string getFunctionCall(Language lang) const;

	// User defined name of the function
	std::string m_name;
	// Override the C++ name
	// E.g. for operator names
	std::optional<std::string> m_pythonName;
	std::string m_fullyQualifiedName;
	std::string m_documentation;
	// Defaults to void
	ObjcSwift::Type m_returnType;
	std::vector<Argument> m_arguments;
	bool m_isConstructor;
	bool m_isOverloaded;
	bool m_isStatic;
};
}    // namespace ObjcSwift::Proxy
