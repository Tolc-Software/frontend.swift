#pragma once

#include <string>

namespace Objc::Conversions {

struct Conversion {
	std::string m_toCpp;
	std::string m_toObjc;
};

/**
* The name to take this enum to the equivalent Objective-C type
*/
Conversion getConversionEnumName(std::string const& moduleName,
                                 std::string const& fullyQualifiedEnumName,
                                 std::string const& enumName);
}
