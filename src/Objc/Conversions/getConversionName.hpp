#pragma once

#include "Objc/Conversions/conversions.hpp"
#include <string>

namespace Objc::Conversions {

/**
* The name to take this enum to the equivalent Objective-C type
*/
Objc::Conversions::Conversion
getConversionEnumName(std::string const& moduleName,
                      std::string const& conversionNamespace,
                      std::string const& fullyQualifiedEnumName,
                      std::string const& enumName);
}
