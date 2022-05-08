#pragma once

#include "Objc/Proxy/type.hpp"
#include <string>
#include <vector>

namespace Objc::Proxy {
/**
* An attribute is either a global value or a member variable
*/
struct Attribute {
	explicit Attribute(std::string const& cppClass,
	                   std::string const& objcClass,
	                   std::string const& name);

	std::string getObjcHeader() const;
	std::string getObjcSource() const;

	void setType(Objc::Proxy::Type const& type);
	void setDocumentation(std::string const& documentation);
	void setAsConst();
	void setAsStatic();
	void setAsObject();
	void setAsStandalone();

private:
	// private?
	std::string getPropertyOptions() const;
	// User defined name of the member variable
	std::string m_name;
	// The name of the surrounding class
	std::string m_cppClass;
	std::string m_objcClass;
	Objc::Proxy::Type m_type;
	std::string m_documentation;
	bool m_isConst;
	bool m_isStatic;
	// Determines which memory property it should have
	bool m_isObject;
	// If it needs wrappers or not
	bool m_isStandalone;
};
}    // namespace Objc::Proxy
