#pragma once

#include "Objc/Conversions/conversion.hpp"
#include <IR/ir.hpp>
#include <string>

namespace Objc::Conversions {

/**
* The function name to take this enum to the equivalent Objective-C type
* and the other way around
*/
Objc::Conversions::Conversion
getConversionEnumName(std::string const& moduleName,
                      std::string const& conversionNamespace,
                      std::string const& fullyQualifiedEnumName,
                      std::string const& enumName);

/**
* The function name to take this base type to the equivalent Objective-C type
* and the other way around
*/
Objc::Conversions::Conversion
getConversionBaseName(std::string const& conversionNamespace,
                      IR::BaseType baseType);
}
