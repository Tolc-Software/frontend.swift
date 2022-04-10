#pragma once

#include "Objc/Proxy/enum.hpp"
#include <IR/ir.hpp>

namespace Objc::Builders {

/**
* Transforms a IR::Enum to a Objc::Proxy::Enum
*/
Objc::Proxy::Enum buildEnum(IR::Enum const& s);
}    // namespace Objc::Builders
