#pragma once

#include "Objc/Proxy/type.hpp"
#include <optional>
#include <string>
#include <vector>

namespace Objc::Proxy {
class Function {
public:
	Function(std::string const& name, std::string const& fullyQualifiedName);

	std::string getObjcSource() const;
	std::string getObjcHeader() const;

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

	void setReturnType(Objc::Proxy::Type const& type);

	std::string getName() const;

	/**
	* The arguments to call this function from Objective-C++ to C++
	* Can contan calls to conversion functions etc.
	*/
	void setCppCallArguments(std::string const& arguments);

	/**
	* The arguments to declare this Objective-C function
	* Should only contain valid Objective-C types
	*/
	void setObjCDeclarationArguments(std::string const& arguments);

private:
	std::string getFunctionCall() const;
	std::string getFunctionDeclaration() const;
	std::string getFunctionBody() const;

	// User defined name of the function
	std::string m_name;
	std::string m_fullyQualifiedName;
	std::string m_documentation;
	// Defaults to void
	Objc::Proxy::Type m_returnType;
	// The full name of the class that contains this function
	std::string m_fullyQualifiedClassName;
	std::string m_cppCallArgs;
	std::string m_objCDeclArgs;
	bool m_isOverloaded;
	bool m_isStatic;
	bool m_isClassFunction;
	bool m_isConstructor;
};
}    // namespace Objc::Proxy
