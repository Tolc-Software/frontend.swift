#pragma once

#include "ObjcSwift/Proxy/enum.hpp"
#include <IR/ir.hpp>

namespace ObjcSwift::Builders {

/**
* Transforms a IR::Enum to a ObjcSwift::Proxy::Enum
*/
ObjcSwift::Proxy::Enum buildEnum(IR::Enum const& s);
}    // namespace ObjcSwift::Builders
