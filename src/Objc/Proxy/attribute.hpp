#pragma once

#include "Objc/Proxy/type.hpp"
#include <string>
#include <vector>

namespace Objc::Proxy {
/**
* An attribute is either a global value or a member variable
*/
struct Attribute {
	// User defined name of the member variable
	std::string m_name;
	Objc::Proxy::Type m_type;
	std::string m_documentation;
	bool m_isConst;
	bool m_isStatic;
	// Determines which memory property it should have
	bool m_isObject;
};
}    // namespace Objc::Proxy
