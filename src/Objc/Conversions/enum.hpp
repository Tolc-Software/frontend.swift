#pragma once

#include "Objc/Conversions/conversion.hpp"
#include "Objc/Proxy/enum.hpp"
#include <IR/ir.hpp>
#include <array>
#include <string>
#include <vector>

namespace Objc::Conversions {

// {convert from, convert to}
// {function name, function}
Conversion getEnumConversions(IR::Enum const& cppEnum,
                              Objc::Proxy::Enum const& objcEnum);
}    // namespace Objc::Conversions
