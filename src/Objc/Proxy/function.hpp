#pragma once

#include "Objc/Proxy/type.hpp"
#include <optional>
#include <string>
#include <vector>

namespace Objc::Proxy {
class Function {
public:
	Function(std::string const& name,
	         std::string const& fullyQualifiedName,
	         std::string const& objcClass,
	         std::string const& cppClass);

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

	void setAsConstructor();

	void setReturnType(Objc::Proxy::Type const& type);

	std::string getName() const;

	struct Argument {
		std::string m_name;
		Objc::Proxy::Type m_type;
	};

	void addArgument(Argument const& arg);
	void setAsStandalone();

private:
	std::string getFunctionCall() const;
	std::string getFunctionDeclaration() const;
	std::string getFunctionBody() const;

	// User defined name of the function
	std::string m_name;
	std::string m_fullyQualifiedName;
	// Containing class
	std::string m_objcClass;
	std::string m_cppClass;
	std::string m_documentation;
	// Defaults to void
	Objc::Proxy::Type m_returnType;
	std::vector<Argument> m_arguments;
	bool m_isOverloaded;
	bool m_isStatic;
	bool m_isConstructor;
	// If it needs wrappers or not
	bool m_isStandalone;
};
}    // namespace Objc::Proxy
