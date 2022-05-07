#pragma once

#include "Objc/Conversions/conversion.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>

namespace Objc::Conversions {
Objc::Conversions::Conversion
getUserDefinedConversionNames(std::string const& fullyQualifiedName,
                              std::string const& ns);
}
