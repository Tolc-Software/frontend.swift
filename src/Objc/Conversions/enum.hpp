#pragma once

#include "Objc/Proxy/enum.hpp"
#include <IR/ir.hpp>
#include <array>
#include <string>
#include <vector>

namespace Objc::Conversions {

// {convert from, convert to}
// {function name, function}
std::array<std::pair<std::string, std::string>, 2>
getEnumConversions(IR::Enum const& cppEnum, Objc::Proxy::Enum const& objcEnum);
}    // namespace Objc::Conversions
